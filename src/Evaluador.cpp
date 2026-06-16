#include "../include/Evaluador.h"

#include <unordered_map>
#include <limits>

ResultadoEvaluacion Evaluador::evaluar(
    const Instancia& instancia,
    const Solucion& solucion,
    double alpha,
    double beta
)
{
    ResultadoEvaluacion r;

    r.distanciaTotal = 0.0;
    r.penalizacionTotal = 0.0;
    r.tiempoTotal = 0.0;

    r.vehiculosUsados = solucion.rutas.size();

    r.factible = true;

    //----------------------------------------
    // Cobertura
    //----------------------------------------

    std::unordered_map<int,int> visitas;

    //----------------------------------------
    // Revisar cada ruta
    //----------------------------------------

    for(const Ruta& ruta : solucion.rutas){
        int carga = 0;

        double tiempoActual = 0.0;

        int depositoIdx = instancia.depositoToIndex.at(ruta.depositoId);

        int nodoAnterior = depositoIdx;

        //------------------------------------
        // Clientes
        //------------------------------------

        for(int clienteId : ruta.clientes){
            int clienteIdx = instancia.clienteToIndex.at(clienteId);

            const Nodo& cliente =
                instancia.nodos[clienteIdx];

            visitas[clienteId]++;

            //--------------------------------
            // Distancia viaje
            //--------------------------------

            double distancia =
                instancia.distancias
                [nodoAnterior]
                [clienteIdx];

            r.distanciaTotal += distancia;

            tiempoActual += distancia;

            //--------------------------------
            // Ventana tiempo
            //--------------------------------

            if(tiempoActual < cliente.e_i){
                tiempoActual = cliente.e_i;
            }

            else if(tiempoActual > cliente.l_i){
                r.penalizacionTotal += (tiempoActual - cliente.l_i);
            }

            //--------------------------------
            // Servicio
            //--------------------------------

            tiempoActual += cliente.s_i;

            carga += cliente.demanda;

            nodoAnterior = clienteIdx;
        }

        //------------------------------------
        // Volver depósito
        //------------------------------------

        double regreso =
            instancia.distancias
            [nodoAnterior]
            [depositoIdx];

        r.distanciaTotal += regreso;

        tiempoActual += regreso;

        //------------------------------------
        // Ventana depósito
        //------------------------------------

        const Nodo& deposito =
            instancia.nodos[depositoIdx];

        if(tiempoActual > deposito.l_i){
            r.penalizacionTotal += (tiempoActual - deposito.l_i);
        }

        //------------------------------------
        // Capacidad
        //------------------------------------

        if(carga > instancia.capacidad){
            r.factible = false;
        }

        r.tiempoTotal += tiempoActual;
    }

    //----------------------------------------
    // Cobertura completa
    //----------------------------------------

    for(const Nodo& cliente : instancia.clientes){
        if(visitas[cliente.id] != 1){
            r.factible = false;
        }
    }

    //----------------------------------------
    // Fitness
    //----------------------------------------

    if(!r.factible){
        r.fitness =
            std::numeric_limits<double>
            ::max();

        return r;
    }

    r.fitness =
        r.distanciaTotal
        + alpha * r.penalizacionTotal
        + beta * r.vehiculosUsados;

    return r;
}