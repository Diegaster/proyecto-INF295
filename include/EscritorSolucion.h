#ifndef ESCRITOR_SOLUCION_H
#define ESCRITOR_SOLUCION_H

#include "Instancia.h"
#include "Solucion.h"
#include "Evaluador.h"

#include <string>

class EscritorSolucion
{
public:

    static void escribir(
        const std::string& archivo,
        const Instancia& instancia,
        const Solucion& solucion,
        const ResultadoEvaluacion& resultado
    );
};

#endif