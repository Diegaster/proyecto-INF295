#include "../include/LectorInstancias.h"
#include "../include/Evaluador.h"
#include "../include/ConstructorGreedy.h"
#include "../include/EscritorSolucion.h"
#include "../include/TabuSearch.h"
#include <iostream>

int main()
{
    Instancia instancia =
        LectorInstancias::leer(
            "data/C102.txt"
        );

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
        "output/C102_greedy.txt",
        instancia,
        solucion,
        resultado
    );

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
        "output/C102_tabu.txt",
        instancia,
        tabu,
        resultadoTabu
    );

    return 0;
}