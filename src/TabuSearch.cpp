#include "../include/TabuSearch.h"
#include "../include/Evaluador.h"
#include <random>
#include <ctime>
#include <deque>
#include <algorithm>
#include <limits>
#include <vector>

// Estructura auxiliar para aplanar las coordenadas de los clientes
struct PosicionCliente {
    int ruta;
    int posicion;
    int idCliente;
};

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
    std::mt19937 rng(config.semilla);
    Solucion mejorGlobal = inicial;
    Solucion actual = inicial;

    ResultadoEvaluacion mejorGlobalEval = Evaluador::evaluar(instancia, mejorGlobal);
    std::deque<MovimientoTabu> tabuList;

    for(int iter = 0; iter < config.factorIteraciones; iter++)
    {
        Solucion mejorVecino;
        ResultadoEvaluacion mejorVecinoEval;
        mejorVecinoEval.fitness = std::numeric_limits<double>::max();
        MovimientoTabu mejorMovimiento;
        bool encontrado = false;

        // Mapeo plano de clientes para los modos exhaustivos
        std::vector<PosicionCliente> posiciones;
        if (config.factorVecinos == 0) {
            for (int r = 0; r < (int)actual.rutas.size(); r++) {
                for (int p = 0; p < (int)actual.rutas[r].clientes.size(); p++) {
                    posiciones.push_back({r, p, actual.rutas[r].clientes[p]});
                }
            }
        }

        //--------------------------------------------------
        // SWAP
        //--------------------------------------------------
        if(config.usarSwap){
            bool exhaustivo = (config.factorVecinos == 0);
            
            if(!exhaustivo){
                // SWAP ALEATORIO
                for(int k = 0; k < config.factorVecinos; k++)
                {
                    int r1 = rng() % actual.rutas.size();
                    if(actual.rutas[r1].clientes.empty()) continue;

                    int r2 = rng() % actual.rutas.size();
                    if(actual.rutas[r2].clientes.empty()) continue;

                    int p1 = rng() % actual.rutas[r1].clientes.size();
                    int p2 = rng() % actual.rutas[r2].clientes.size();

                    if(r1 == r2 && p1 == p2) continue;

                    int clienteA = actual.rutas[r1].clientes[p1];
                    int clienteB = actual.rutas[r2].clientes[p2];

                    MovimientoTabu mov{clienteA, clienteB};

                    // Modificación in-place
                    std::swap(actual.rutas[r1].clientes[p1], actual.rutas[r2].clientes[p2]);
                    ResultadoEvaluacion eval = Evaluador::evaluar(instancia, actual);

                    bool tabu = esTabu(mov, tabuList);
                    bool aspiracion = eval.fitness < mejorGlobalEval.fitness;

                    if (!(tabu && !aspiracion) && eval.fitness < mejorVecinoEval.fitness) {
                        mejorVecino = actual; // Guardamos copia solo si es el mejor hasta ahora
                        mejorVecinoEval = eval;
                        mejorMovimiento = mov;
                        encontrado = true;
                    }
                    
                    // Rollback inmediato
                    std::swap(actual.rutas[r1].clientes[p1], actual.rutas[r2].clientes[p2]);
                }
            } else {
                // SWAP EXHAUSTIVO (Optimizado con 2 ciclos utilizando la lista plana)
                for(size_t i = 0; i < posiciones.size(); i++) {
                    for(size_t j = i + 1; j < posiciones.size(); j++) {
                        const auto& posA = posiciones[i];
                        const auto& posB = posiciones[j];

                        MovimientoTabu mov{posA.idCliente, posB.idCliente};

                        // Modificación in-place
                        std::swap(actual.rutas[posA.ruta].clientes[posA.posicion], actual.rutas[posB.ruta].clientes[posB.posicion]);
                        ResultadoEvaluacion eval = Evaluador::evaluar(instancia, actual);

                        bool tabu = esTabu(mov, tabuList);
                        bool aspiracion = eval.fitness < mejorGlobalEval.fitness;

                        if (!(tabu && !aspiracion) && eval.fitness < mejorVecinoEval.fitness) {
                            mejorVecino = actual;
                            mejorVecinoEval = eval;
                            mejorMovimiento = mov;
                            encontrado = true;
                        }

                        // Rollback inmediato
                        std::swap(actual.rutas[posA.ruta].clientes[posA.posicion], actual.rutas[posB.ruta].clientes[posB.posicion]);
                    }
                }
            }
        }

        //--------------------------------------------------
        // RELOCATE
        //--------------------------------------------------
        if(config.usarRelocate){
            bool exhaustivo = (config.factorVecinos == 0);
            
            if (!exhaustivo){
                // RELOCATE ALEATORIO
                for(int k = 0; k < config.factorVecinos; k++)
                {
                    int r1 = rng() % actual.rutas.size();
                    if(actual.rutas[r1].clientes.size() <= 1) continue;

                    int r2 = rng() % actual.rutas.size();
                    int p1 = rng() % actual.rutas[r1].clientes.size();
                    int p2 = rng() % (actual.rutas[r2].clientes.size() + 1);

                    int cliente = actual.rutas[r1].clientes[p1];
                    MovimientoTabu mov{cliente, -1};

                    // Modificación in-place (aplicar relocate de forma temporal)
                    actual.rutas[r1].clientes.erase(actual.rutas[r1].clientes.begin() + p1);
                    actual.rutas[r2].clientes.insert(actual.rutas[r2].clientes.begin() + std::min(p2, (int)actual.rutas[r2].clientes.size()), cliente);

                    ResultadoEvaluacion eval = Evaluador::evaluar(instancia, actual);

                    bool tabu = esTabu(mov, tabuList);
                    bool aspiracion = eval.fitness < mejorGlobalEval.fitness;

                    if (!(tabu && !aspiracion) && eval.fitness < mejorVecinoEval.fitness) {
                        mejorVecino = actual;
                        mejorVecinoEval = eval;
                        mejorMovimiento = mov;
                        encontrado = true;
                    }

                    // Rollback del Relocate (hacer el proceso inverso exacto)
                    actual.rutas[r2].clientes.erase(actual.rutas[r2].clientes.begin() + std::min(p2, (int)actual.rutas[r2].clientes.size() - 1));
                    actual.rutas[r1].clientes.insert(actual.rutas[r1].clientes.begin() + p1, cliente);
                }
            } else {
                // RELOCATE EXHAUSTIVO (Optimizado con Rollback)
                // =================================================================
// RELOCATE EXHAUSTIVO (Corregido para evitar desfases de índices)
// =================================================================
                for(const auto& posA : posiciones) {
                    if(actual.rutas[posA.ruta].clientes.size() <= 1) continue;

                    for(int r2 = 0; r2 < (int)actual.rutas.size(); r2++) {
                        // Determinamos el límite superior de inserción
                        int max_p2 = (int)actual.rutas[r2].clientes.size();
                        if (posA.ruta == r2) {
                            // Si es la misma ruta, al quitar un elemento el tamaño máximo se reduce en 1
                            max_p2--; 
                        }

                        for(int p2 = 0; p2 <= max_p2; p2++) {
                            // Evitar movimientos redundantes en la misma ruta
                            if(posA.ruta == r2 && (p2 == posA.posicion)) continue;

                            MovimientoTabu mov{posA.idCliente, -1};

                            // ---- OPERACIÓN IN-PLACE SEGURA ----
                            if (posA.ruta == r2) {
                                // Relocate dentro de la misma ruta de forma limpia
                                actual.rutas[posA.ruta].clientes.erase(actual.rutas[posA.ruta].clientes.begin() + posA.posicion);
                                actual.rutas[r2].clientes.insert(actual.rutas[r2].clientes.begin() + p2, posA.idCliente);
                            } else {
                                // Relocate entre rutas distintas
                                actual.rutas[posA.ruta].clientes.erase(actual.rutas[posA.ruta].clientes.begin() + posA.posicion);
                                actual.rutas[r2].clientes.insert(actual.rutas[r2].clientes.begin() + p2, posA.idCliente);
                            }

                            // Evaluar
                            ResultadoEvaluacion eval = Evaluador::evaluar(instancia, actual);

                            bool tabu = esTabu(mov, tabuList);
                            bool aspiracion = eval.fitness < mejorGlobalEval.fitness;

                            if (!(tabu && !aspiracion) && eval.fitness < mejorVecinoEval.fitness) {
                                mejorVecino = actual;
                                mejorVecinoEval = eval;
                                mejorMovimiento = mov;
                                encontrado = true;
                            }

                            // ---- ROLLBACK SIMÉTRICO SEGURO ----
                            if (posA.ruta == r2) {
                                // Deshacer el movimiento dentro de la misma ruta
                                actual.rutas[r2].clientes.erase(actual.rutas[r2].clientes.begin() + p2);
                                actual.rutas[posA.ruta].clientes.insert(actual.rutas[posA.ruta].clientes.begin() + posA.posicion, posA.idCliente);
                            } else {
                                // Deshacer el movimiento entre rutas distintas
                                actual.rutas[r2].clientes.erase(actual.rutas[r2].clientes.begin() + p2);
                                actual.rutas[posA.ruta].clientes.insert(actual.rutas[posA.ruta].clientes.begin() + posA.posicion, posA.idCliente);
                            }
                        }
                    }
                }
            }
        }

        if(!encontrado) break;

        // Avanzamos el estado actual al mejor vecino encontrado
        actual = mejorVecino;

        tabuList.push_back(mejorMovimiento);
        if((int)tabuList.size() > config.tabuTenure) {
            tabuList.pop_front();
        }

        if(mejorVecinoEval.fitness < mejorGlobalEval.fitness) {
            mejorGlobal = mejorVecino;
            mejorGlobalEval = mejorVecinoEval;
        }
    }

    return mejorGlobal;
}