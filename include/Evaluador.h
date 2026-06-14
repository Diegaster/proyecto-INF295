#ifndef EVALUADOR_H
#define EVALUADOR_H

#include "Instancia.h"
#include "Solucion.h"

struct ResultadoEvaluacion {

    double fitness;

    double distanciaTotal;

    double penalizacionTotal;

    double tiempoTotal;

    int vehiculosUsados;

    bool factible;
};

class Evaluador {

public:

    static ResultadoEvaluacion evaluar(
        const Instancia& instancia,
        const Solucion& solucion,
        double alpha = 10.0,
        double beta = 50.0
    );
};

#endif