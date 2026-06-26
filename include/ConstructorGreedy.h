#ifndef CONSTRUCTOR_GREEDY_H
#define CONSTRUCTOR_GREEDY_H

#include "Instancia.h"
#include "Solucion.h"

/* Constructor del Greedy a partir de una solucion, se puede modificar el alpha para cambiar el peso de las
penalizaciones */
class ConstructorGreedy{
public:

    static Solucion construir(
        const Instancia& instancia,
        double alpha = 10.0
    );
};

#endif