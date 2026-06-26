#ifndef EVALUADOR_H
#define EVALUADOR_H

#include "Instancia.h"
#include "Solucion.h"

/* Clase que se encarga de guardar aquellos resultados de la evaluación usados para comparar distintas soluciones*/
struct ResultadoEvaluacion {

    double fitness;

    double distanciaTotal;

    double penalizacionTotal;

    double tiempoTotal;

    int vehiculosUsados;

    bool factible;
};

/* Definicion de parametros de como se evalua la solucion (FE), es posible modificar el alpha y beta.
alpha = coeficiente que pondera las penalizaciones
beta = coeficiente que pondera la cantidad de vehículos utilizados */
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