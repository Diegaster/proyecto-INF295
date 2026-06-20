#include "../include/LectorInstancias.h"
#include "../include/Evaluador.h"
#include "../include/ConstructorGreedy.h"
#include "../include/EscritorSolucion.h"
#include "../include/TabuSearch.h"

#include <iostream>
#include <filesystem>
#include <vector>
#include <algorithm>

int main(){
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
        !config.usarSwap &&
        !config.usarRelocate
    ){
        std::cout
            << "Debe activar al menos un movimiento.\n";

        return 1;
    }

    //----------------------------------------
    // Buscar instancias
    //----------------------------------------

    std::vector<std::string> archivos;

    for(const auto& entry :
        std::filesystem::directory_iterator("data")){
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
    // Resolver todas las instancias
    //----------------------------------------

    for(const auto& archivo : archivos){
        std::string nombre =
            std::filesystem::path(archivo)
            .stem()
            .string();

        std::cout
            << "\n====================================\n";

        std::cout
            << "Procesando "
            << nombre
            << "\n";

        std::cout
            << "====================================\n";

        //------------------------------------
        // Leer instancia
        //------------------------------------

        Instancia instancia =
            LectorInstancias::leer(
                archivo
            );

        //------------------------------------
        // Greedy
        //------------------------------------

        Solucion solucion =
            ConstructorGreedy::construir(
                instancia
            );

        ResultadoEvaluacion resultado =
            Evaluador::evaluar(
                instancia,
                solucion
            );

        EscritorSolucion::escribir(
            "output/" +
            nombre +
            "_greedy.txt",
            instancia,
            solucion,
            resultado
        );

        //------------------------------------
        // Tabu Search
        //------------------------------------

        Solucion tabu =
            TabuSearch::optimizar(
                instancia,
                solucion,
                config
            );

        ResultadoEvaluacion resultadoTabu =
            Evaluador::evaluar(
                instancia,
                tabu
            );

        EscritorSolucion::escribir(
            "output/" +
            nombre +
            "_tabu.txt",
            instancia,
            tabu,
            resultadoTabu
        );

        //------------------------------------
        // Resumen
        //------------------------------------

        std::cout
            << "Greedy Fitness: "
            << resultado.fitness
            << "\n";

        std::cout
            << "Tabu Fitness: "
            << resultadoTabu.fitness
            << "\n";

        std::cout
            << "Mejora: "
            << resultado.fitness -
               resultadoTabu.fitness
            << "\n";
    }

    std::cout
        << "\nTodas las instancias fueron procesadas.\n";

    return 0;
}