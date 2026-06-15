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

        for(const auto& deposito :
            instancia.depositos)
        {
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

    int clienteMasCercano(
        const Instancia& instancia,
        int nodoActualIdx,
        const std::unordered_set<int>& noVisitados,
        int cargaActual
    )
    {
        double mejorDist =
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

            if(cargaActual +
               cliente.demanda >
               instancia.capacidad)
            {
                continue;
            }

            double d =
                instancia.distancias
                [nodoActualIdx]
                [clienteIdx];

            if(d < mejorDist)
            {
                mejorDist = d;
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

    for(const auto& cliente :
        instancia.clientes)
    {
        noVisitados.insert(
            cliente.id
        );
    }

    //----------------------------------
    // Mientras falten clientes
    //----------------------------------

    while(!noVisitados.empty())
    {
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

        while(true)
        {
            int siguiente =
                clienteMasCercano(
                    instancia,
                    nodoActualIdx,
                    noVisitados,
                    cargaActual
                );

            if(siguiente == -1)
            {
                break;
            }

            int clienteIdx =
                instancia.clienteToIndex.at(
                    siguiente
                );

            const Nodo& cliente =
                instancia.nodos[clienteIdx];

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
        }

        //----------------------------------
        // Guardar ruta
        //----------------------------------

        if(!ruta.clientes.empty())
        {
            solucion.rutas.push_back(
                ruta
            );
        }
    }

    return solucion;
}