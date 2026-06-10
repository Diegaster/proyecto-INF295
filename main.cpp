#include "../include/LectorInstancias.h"
#include "../include/Evaluador.h"

#include <iostream>

int main()
{
    Instancia instancia =
        LectorInstancias::leer(
            "data/C102.txt"
        );

    Solucion solucion;

    double fitness =
        Evaluador::evaluar(
            instancia,
            solucion
        );

    std::cout
        << "Fitness: "
        << fitness
        << std::endl;

    return 0;
}
