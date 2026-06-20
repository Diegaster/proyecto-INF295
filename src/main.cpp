#include "../include/LectorInstancias.h"
#include "../include/Evaluador.h"
#include "../include/ConstructorGreedy.h"
#include "../include/EscritorSolucion.h"
#include "../include/TabuSearch.h"

#include <iostream>
#include <filesystem>
#include <vector>
#include <algorithm>
#include <fstream>
#include <chrono>

int main()
{
    //----------------------------------------
    // Configuración ejecución
    //----------------------------------------

    char modo;

    std::cout
        << "Modo de ejecucion:\n"
        << "G = Solo Greedy\n"
        << "T = Solo Tabu (desde solucion Greedy)\n"
        << "A = Ambos\n"
        << "Opcion: ";

    std::cin >> modo;

    modo = std::toupper(modo);

    //----------------------------------------
    // Configuración Tabu
    //----------------------------------------

    TabuSearch::ConfigTabu config;

    std::cout
        << "Iteraciones Tabu: ";

    std::cin
        >> config.maxIteraciones;

    std::cout
        << "Largo lista Tabu: ";

    std::cin
        >> config.tabuTenure;

    char opcion;

    std::cout
        << "Usar Swap? (y/n): ";

    std::cin
        >> opcion;

    config.usarSwap =
        (opcion == 'y' ||
         opcion == 'Y');

    std::cout
        << "Usar Relocate? (y/n): ";

    std::cin
        >> opcion;

    config.usarRelocate =
        (opcion == 'y' ||
         opcion == 'Y');

    if(
        modo != 'G' &&
        !config.usarSwap &&
        !config.usarRelocate
    )
    {
        std::cout
            << "Debe activar al menos un movimiento.\n";

        return 1;
    }

    //----------------------------------------
    // Directorios
    //----------------------------------------

    std::filesystem::create_directories(
        "output"
    );

    //----------------------------------------
    // CSV consolidado
    //----------------------------------------

    std::ofstream csv(
        "output/resultados.csv"
    );

    csv
    << "Instancia,"
    << "Clientes,"
    << "Modo,"
    << "Iteraciones,"
    << "TabuTenure,"
    << "Swap,"
    << "Relocate,"
    << "FitnessGreedy,"
    << "FitnessTabu,"
    << "Mejora,"
    << "PenalizacionGreedy,"
    << "PenalizacionTabu,"
    << "VehiculosGreedy,"
    << "VehiculosTabu,"
    << "TiempoTabuMs\n";

    //----------------------------------------
    // Buscar instancias
    //----------------------------------------

    std::vector<std::string> archivos;

    for(const auto& entry :
        std::filesystem::directory_iterator(
            "data"
        ))
    {
        if(entry.is_regular_file())
        {
            archivos.push_back(
                entry.path().string()
            );
        }
    }

    std::sort(
        archivos.begin(),
        archivos.end()
    );

    //----------------------------------------
    // Procesar instancias
    //----------------------------------------

    for(const auto& archivo : archivos)
    {
        Instancia instancia =
            LectorInstancias::leer(
                archivo
            );

        std::string nombre =
            std::filesystem::path(
                archivo
            ).stem().string();

        int cantidadClientes =
            instancia.clientes.size();

        std::string carpeta =
            "output/" +
            std::to_string(
                cantidadClientes
            ) +
            "_clientes";

        std::filesystem::create_directories(
            carpeta
        );

        std::cout
            << "\n====================================\n";

        std::cout
            << "Procesando "
            << nombre
            << "\n";

        std::cout
            << "Clientes: "
            << cantidadClientes
            << "\n";

        //------------------------------------
        // Greedy
        //------------------------------------

        Solucion solucionGreedy =
            ConstructorGreedy::construir(
                instancia
            );

        ResultadoEvaluacion resultadoGreedy =
            Evaluador::evaluar(
                instancia,
                solucionGreedy
            );

        if(
            modo == 'G' ||
            modo == 'A'
        )
        {
            EscritorSolucion::escribir(
                carpeta +
                "/" +
                nombre +
                "_greedy.txt",
                instancia,
                solucionGreedy,
                resultadoGreedy
            );
        }

        //------------------------------------
        // Tabu
        //------------------------------------

        ResultadoEvaluacion resultadoTabu;
        resultadoTabu.fitness =
            resultadoGreedy.fitness;

        resultadoTabu.penalizacionTotal =
            resultadoGreedy.penalizacionTotal;

        resultadoTabu.vehiculosUsados =
            resultadoGreedy.vehiculosUsados;

        double tiempoTabuMs = 0.0;

        if(
            modo == 'T' ||
            modo == 'A'
        )
        {
            auto inicio =
                std::chrono::high_resolution_clock::now();

            Solucion solucionTabu =
                TabuSearch::optimizar(
                    instancia,
                    solucionGreedy,
                    config
                );

            auto fin =
                std::chrono::high_resolution_clock::now();

            tiempoTabuMs =
                std::chrono::duration<double,
                std::milli>(
                    fin - inicio
                ).count();

            resultadoTabu =
                Evaluador::evaluar(
                    instancia,
                    solucionTabu
                );

            EscritorSolucion::escribir(
                carpeta +
                "/" +
                nombre +
                "_tabu.txt",
                instancia,
                solucionTabu,
                resultadoTabu
            );

            std::cout
                << "Greedy Fitness: "
                << resultadoGreedy.fitness
                << "\n";

            std::cout
                << "Tabu Fitness: "
                << resultadoTabu.fitness
                << "\n";

            std::cout
                << "Mejora: "
                << resultadoGreedy.fitness -
                   resultadoTabu.fitness
                << "\n";
        }

        //------------------------------------
        // CSV
        //------------------------------------

        csv
        << nombre << ","
        << cantidadClientes << ","
        << modo << ","
        << config.maxIteraciones << ","
        << config.tabuTenure << ","
        << (config.usarSwap ? 1 : 0) << ","
        << (config.usarRelocate ? 1 : 0) << ","
        << resultadoGreedy.fitness << ","
        << resultadoTabu.fitness << ","
        << (resultadoGreedy.fitness -
            resultadoTabu.fitness) << ","
        << resultadoGreedy.penalizacionTotal << ","
        << resultadoTabu.penalizacionTotal << ","
        << resultadoGreedy.vehiculosUsados << ","
        << resultadoTabu.vehiculosUsados << ","
        << tiempoTabuMs
        << "\n";
    }

    csv.close();

    std::cout
        << "\nTodas las instancias fueron procesadas.\n";

    return 0;
}