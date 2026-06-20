#include "../include/TabuSearch.h"
#include "../include/Evaluador.h"

#include <deque>
#include <algorithm>
#include <limits>

static Solucion aplicarSwap(
    const Solucion& original,
    int rutaA,
    int posA,
    int rutaB,
    int posB
)
{
    Solucion vecino = original;

    std::swap(
        vecino.rutas[rutaA].clientes[posA],
        vecino.rutas[rutaB].clientes[posB]
    );

    return vecino;
}

static Solucion aplicarRelocate(
    const Solucion& original,
    int rutaOrigen,
    int posOrigen,
    int rutaDestino,
    int posDestino
)
{
    Solucion vecino = original;

    int cliente =
        vecino.rutas[rutaOrigen]
              .clientes[posOrigen];

    vecino.rutas[rutaOrigen]
          .clientes.erase(
              vecino.rutas[rutaOrigen]
                    .clientes.begin()
              + posOrigen
          );

    if(posDestino >
       (int)vecino.rutas[rutaDestino]
                      .clientes.size())
    {
        posDestino =
            vecino.rutas[rutaDestino]
                  .clientes.size();
    }

    vecino.rutas[rutaDestino]
          .clientes.insert(
              vecino.rutas[rutaDestino]
                    .clientes.begin()
              + posDestino,
              cliente
          );

    return vecino;
}

static bool esTabu(
    const MovimientoTabu& mov,
    const std::deque<MovimientoTabu>& lista
)
{
    for(const auto& t : lista)
    {
        if(
            (t.clienteA == mov.clienteA &&
             t.clienteB == mov.clienteB)
            ||
            (t.clienteA == mov.clienteB &&
             t.clienteB == mov.clienteA)
        )
        {
            return true;
        }
    }

    return false;
}

Solucion TabuSearch::optimizar(
    const Instancia& instancia,
    const Solucion& inicial,
    const ConfigTabu& config
)
{
    Solucion mejorGlobal = inicial;
    Solucion actual = inicial;

    ResultadoEvaluacion mejorGlobalEval =
        Evaluador::evaluar(
            instancia,
            mejorGlobal
        );

    std::deque<MovimientoTabu> tabuList;
    int vecinosExplorados = 0;
    for(int iter = 0;
        iter < config.factorIteraciones;
        iter++){
        Solucion mejorVecino;
        ResultadoEvaluacion mejorVecinoEval;

        mejorVecinoEval.fitness =
            std::numeric_limits<double>::max();

        MovimientoTabu mejorMovimiento;

        bool encontrado = false;

        //--------------------------------------------------
        // SWAP
        //--------------------------------------------------
        if(config.usarSwap){
            for(int r1 = 0;
                r1 < (int)actual.rutas.size();
                r1++)
            {
                for(int p1 = 0;
                    p1 < (int)actual.rutas[r1].clientes.size();
                    p1++)
                {
                    for(int r2 = r1;
                        r2 < (int)actual.rutas.size();
                        r2++)
                    {
                        for(int p2 = 0;
                            p2 < (int)actual.rutas[r2].clientes.size();
                            p2++)
                        {
                            if(r1 == r2 &&
                            p1 == p2)
                            {
                                continue;
                            }

                            int clienteA =
                                actual.rutas[r1]
                                    .clientes[p1];

                            int clienteB =
                                actual.rutas[r2]
                                    .clientes[p2];

                            MovimientoTabu mov;

                            mov.clienteA =
                                clienteA;

                            mov.clienteB =
                                clienteB;

                            Solucion vecino =
                                aplicarSwap(
                                    actual,
                                    r1,
                                    p1,
                                    r2,
                                    p2
                                );

                            ResultadoEvaluacion eval =
                                Evaluador::evaluar(
                                    instancia,
                                    vecino
                                );

                            bool tabu =
                                esTabu(
                                    mov,
                                    tabuList
                                );

                            bool aspiracion =
                                eval.fitness <
                                mejorGlobalEval.fitness;

                            if(
                                tabu &&
                                !aspiracion
                            )
                            {
                                continue;
                            }

                            if(
                                eval.fitness <
                                mejorVecinoEval.fitness
                            )
                            {
                                mejorVecino =
                                    vecino;

                                mejorVecinoEval =
                                    eval;

                                mejorMovimiento =
                                    mov;

                                encontrado =
                                    true;
                            }
                        }
                    }
                }
            }
        }

        //--------------------------------------------------
        // RELOCATE
        //--------------------------------------------------
        if(config.usarRelocate){
            for(int r1 = 0;
                r1 < (int)actual.rutas.size();
                r1++)
            {
                if(
                    actual.rutas[r1]
                        .clientes.size()
                    <= 1
                )
                {
                    continue;
                }

                for(int p1 = 0;
                    p1 < (int)actual.rutas[r1].clientes.size();
                    p1++)
                {
                    int cliente =
                        actual.rutas[r1]
                            .clientes[p1];

                    for(int r2 = 0;
                        r2 < (int)actual.rutas.size();
                        r2++)
                    {
                        for(int p2 = 0;
                            p2 <= (int)actual.rutas[r2]
                                        .clientes.size();
                            p2++)
                        {
                            if(
                                r1 == r2 &&
                                (p2 == p1 ||
                                p2 == p1 + 1)
                            )
                            {
                                continue;
                            }

                            MovimientoTabu mov;

                            mov.clienteA =
                                cliente;

                            mov.clienteB =
                                -1;

                            Solucion vecino =
                                aplicarRelocate(
                                    actual,
                                    r1,
                                    p1,
                                    r2,
                                    p2
                                );

                            ResultadoEvaluacion eval =
                                Evaluador::evaluar(
                                    instancia,
                                    vecino
                                );

                            bool tabu =
                                esTabu(
                                    mov,
                                    tabuList
                                );

                            bool aspiracion =
                                eval.fitness <
                                mejorGlobalEval.fitness;

                            if(
                                tabu &&
                                !aspiracion
                            )
                            {
                                continue;
                            }

                            if(
                                eval.fitness <
                                mejorVecinoEval.fitness
                            )
                            {
                                mejorVecino =
                                    vecino;

                                mejorVecinoEval =
                                    eval;

                                mejorMovimiento =
                                    mov;

                                encontrado =
                                    true;
                            }
                        }
                    }
                }
            }
        }

        if(!encontrado)
        {
            break;
        }

        actual = mejorVecino;

        tabuList.push_back(
            mejorMovimiento
        );

        if(
            (int)tabuList.size() >
            config.tabuTenure
        )
        {
            tabuList.pop_front();
        }

        if(
            mejorVecinoEval.fitness <
            mejorGlobalEval.fitness
        )
        {
            mejorGlobal =
                mejorVecino;

            mejorGlobalEval =
                mejorVecinoEval;
        }
    }

    return mejorGlobal;
}