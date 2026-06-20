#include "../include/LectorInstancias.h"
#include "../include/Evaluador.h"
#include "../include/ConstructorGreedy.h"
#include "../include/EscritorSolucion.h"
#include "../include/TabuSearch.h"

int main()
{
    Instancia instancia =
        LectorInstancias::leer(
            "data/C102.txt"
        );

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
            500,
            10
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