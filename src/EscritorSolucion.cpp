#include "../include/EscritorSolucion.h"

#include <fstream>

void EscritorSolucion::escribir(
    const std::string& archivo,
    const Instancia& instancia,
    const Solucion& solucion,
    const ResultadoEvaluacion& resultado,
    unsigned int semilla
)
{
    std::ofstream out(archivo);

    if(!out.is_open())
        return;
    if(
        archivo.find("_tabu") !=
        std::string::npos
    )
    {
        out << "Semilla: "
            << semilla
            << "\n";
    }
    out << "Instancia: "
        << instancia.nombre
        << "\n\n";

    out << "Fitness: "
        << resultado.fitness
        << "\n";

    out << "Factible: "
        << (resultado.factible ? "SI" : "NO")
        << "\n";

    out << "Distancia Total: "
        << resultado.distanciaTotal
        << "\n";

    out << "Penalizacion: "
        << resultado.penalizacionTotal
        << "\n";

    out << "Vehiculos: "
        << resultado.vehiculosUsados
        << "\n\n";

    int vehiculo = 1;

    for(const auto& ruta : solucion.rutas)
    {
        out << "Vehiculo "
            << vehiculo++
            << "\n";

        out << "Ruta: D"
            << ruta.depositoId;

        int carga = 0;

        for(int clienteId : ruta.clientes)
        {
            out << " -> "
                << clienteId;

            int idx =
                instancia.clienteToIndex.at(
                    clienteId
                );

            carga +=
                instancia.nodos[idx].demanda;
        }

        out << " -> D"
            << ruta.depositoId
            << "\n";

        out << "Carga: "
            << carga
            << "\n\n";
    }

    out.close();
}