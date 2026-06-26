#include "../include/Evaluador.h"

#include <unordered_map>
#include <limits>

/* Funcion auxiliar que se encarga de evaluar la solucion, ocupada en TabuSearch */
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

    std::unordered_map<int,int> visitas;

    /* Revision de cada ruta */

    for(const Ruta& ruta : solucion.rutas){
        int carga = 0;

        double tiempoActual = 0.0;

        int depositoIdx = instancia.depositoToIndex.at(ruta.depositoId);

        int nodoAnterior = depositoIdx;

        /* Revision de cada cliente de la ruta */

        for(int clienteId : ruta.clientes){
            int clienteIdx = instancia.clienteToIndex.at(clienteId);

            const Nodo& cliente =
                instancia.nodos[clienteIdx];

            visitas[clienteId]++;

            /* Distancia del viaje entre clientes o deposito/cliente*/

            double distancia =
                instancia.distancias
                [nodoAnterior]
                [clienteIdx];

            r.distanciaTotal += distancia;

            tiempoActual += distancia;

            /* Actualizacion de ventana de tiempo + penalizacion */

            if(tiempoActual < cliente.e_i){
                tiempoActual = cliente.e_i;
            }

            else if(tiempoActual > cliente.l_i){
                r.penalizacionTotal += (tiempoActual - cliente.l_i);
            }

            /* Actualizacion del tiempo y carga por servicio*/

            tiempoActual += cliente.s_i;

            carga += cliente.demanda;

            nodoAnterior = clienteIdx;
        }

        /* Contar el retorno al deposito al final de la ruta */

        double regreso =
            instancia.distancias
            [nodoAnterior]
            [depositoIdx];

        r.distanciaTotal += regreso;

        tiempoActual += regreso;

        /* Venta de deposito que corresponde al cierre de la jornada laboral, si se atrasa el vehiculo
        se penaliza */

        const Nodo& deposito =
            instancia.nodos[depositoIdx];

        if(tiempoActual > deposito.l_i){
            r.penalizacionTotal += (tiempoActual - deposito.l_i);
        }

        /* Cumplimiento de la restriccion de capacidad */

        if(carga > instancia.capacidad){
            r.factible = false;
        }

        r.tiempoTotal += tiempoActual;
    }

    /* Restriccion de cobertura completa */

    for(const Nodo& cliente : instancia.clientes){
        if(visitas[cliente.id] != 1){
            r.factible = false;
        }
    }

    /* Calculo del fitness */

    if(!r.factible){
        r.fitness =
            std::numeric_limits<double>
            ::max();

        return r;
    }
    /* Calculo en base a la funcion de evaluacion presente en el informe */
    r.fitness =
        r.distanciaTotal
        + alpha * r.penalizacionTotal
        + beta * r.vehiculosUsados;

    return r;
}