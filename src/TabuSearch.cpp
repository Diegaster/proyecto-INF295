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
    int maxIteraciones,
    int tabuTenure
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

    for(int iter = 0;
        iter < maxIteraciones;
        iter++)
    {
        Solucion mejorVecino;
        ResultadoEvaluacion mejorVecinoEval;

        mejorVecinoEval.fitness =
            std::numeric_limits<double>::max();

        MovimientoTabu mejorMovimiento;

        bool encontrado = false;

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
            tabuTenure
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