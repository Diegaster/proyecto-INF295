#include "../include/ConstructorGreedy.h"

#include <unordered_set>
#include <limits>

namespace
{
    int depositoMasCercano(
        const Instancia& instancia,
        int clienteId
    )
    {
        int clienteIdx =
            instancia.clienteToIndex.at(clienteId);

        double mejorDist =
            std::numeric_limits<double>::max();

        int mejorDeposito = -1;

        for(const auto& deposito : instancia.depositos){
            int depIdx =
                instancia.depositoToIndex.at(
                    deposito.id
                );

            double d =
                instancia.distancias
                [depIdx]
                [clienteIdx];

            if(d < mejorDist)
            {
                mejorDist = d;
                mejorDeposito = deposito.id;
            }
        }

        return mejorDeposito;
    }

    int mejorClienteGreedy(
        const Instancia& instancia,
        int nodoActualIdx,
        const std::unordered_set<int>& noVisitados,
        int cargaActual,
        double tiempoActual,
        double alpha
    )
    {
        double mejorCosto =
            std::numeric_limits<double>::max();

        int mejorCliente = -1;

        for(int clienteId : noVisitados)
        {
            int clienteIdx =
                instancia.clienteToIndex.at(
                    clienteId
                );

            const Nodo& cliente =
                instancia.nodos[clienteIdx];

            //--------------------------------
            // capacidad
            //--------------------------------

            if(cargaActual +
            cliente.demanda >
            instancia.capacidad)
            {
                continue;
            }

            //--------------------------------
            // distancia
            //--------------------------------

            double distancia =
                instancia.distancias
                [nodoActualIdx]
                [clienteIdx];

            //--------------------------------
            // llegada
            //--------------------------------

            double llegada =
                tiempoActual + distancia;

            //--------------------------------
            // espera
            //--------------------------------

            if(llegada < cliente.e_i)
            {
                llegada =
                    cliente.e_i;
            }

            //--------------------------------
            // tardanza
            //--------------------------------

            double penalizacion = 0.0;

            if(llegada > cliente.l_i)
            {
                penalizacion =
                    llegada - cliente.l_i;
            }

            //--------------------------------
            // score greedy
            //--------------------------------

            double costo =
                distancia +
                alpha * penalizacion;

            if(costo < mejorCosto)
            {
                mejorCosto = costo;
                mejorCliente = clienteId;
            }
        }

        return mejorCliente;
    }
}

Solucion ConstructorGreedy::construir(
    const Instancia& instancia,
    double alpha
)
{
    Solucion solucion;

    //----------------------------------
    // Clientes sin visitar
    //----------------------------------

    std::unordered_set<int> noVisitados;

    for(const auto& cliente : instancia.clientes){
        noVisitados.insert(
            cliente.id
        );
    }

    //----------------------------------
    // Mientras falten clientes
    //----------------------------------

    while(!noVisitados.empty()){
        //----------------------------------
        // Elegir cliente semilla
        //----------------------------------

        int clienteInicial =
            *noVisitados.begin();

        int depositoId =
            depositoMasCercano(
                instancia,
                clienteInicial
            );

        Ruta ruta;

        ruta.depositoId =
            depositoId;

        int depIdx =
            instancia.depositoToIndex.at(
                depositoId
            );

        int nodoActualIdx =
            depIdx;

        int cargaActual = 0;

        //----------------------------------
        // Construcción greedy
        //----------------------------------
        double tiempoActual = 0.0;
        while(true){
            int siguiente =
                mejorClienteGreedy(
                    instancia,
                    nodoActualIdx,
                    noVisitados,
                    cargaActual,
                    tiempoActual,
                    alpha
                );

            if(siguiente == -1){
                break;
            }

            int clienteIdx =
                instancia.clienteToIndex.at(
                    siguiente
                );
            
            double distancia =
                instancia.distancias
                [nodoActualIdx]
                [clienteIdx];

            tiempoActual += distancia;

            const Nodo& cliente =
                instancia.nodos[clienteIdx];
            
            if(tiempoActual < cliente.e_i){
                tiempoActual =
                    cliente.e_i;
            }
            ruta.clientes.push_back(
                siguiente
            );

            cargaActual +=
                cliente.demanda;

            nodoActualIdx =
                clienteIdx;

            noVisitados.erase(
                siguiente
            );

            tiempoActual += cliente.s_i;
        }

        //----------------------------------
        // Guardar ruta
        //----------------------------------

        if(!ruta.clientes.empty()){
            solucion.rutas.push_back(
                ruta
            );
        }
    }

    return solucion;
}