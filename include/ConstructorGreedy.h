#ifndef CONSTRUCTOR_GREEDY_H
#define CONSTRUCTOR_GREEDY_H

#include "Instancia.h"
#include "Solucion.h"

class ConstructorGreedy
{
public:

    static Solucion construir(
        const Instancia& instancia,
        double alpha = 10.0
    );
};

#endif