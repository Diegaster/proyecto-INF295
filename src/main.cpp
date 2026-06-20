#include "../include/LectorInstancias.h"
#include "../include/Evaluador.h"
#include "../include/ConstructorGreedy.h"
#include "../include/EscritorSolucion.h"

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

    return 0;
}