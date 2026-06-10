#ifndef EVALUADOR_H
#define EVALUADOR_H

#include "Instancia.h"
#include "Solucion.h"

class Evaluador {

public:

    static double evaluar(
        const Instancia& instancia,
        const Solucion& solucion,
        double alpha = 10.0,
        double beta = 50.0
    );

};

#endif
