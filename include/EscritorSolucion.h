#ifndef ESCRITOR_SOLUCION_H
#define ESCRITOR_SOLUCION_H

#include "Instancia.h"
#include "Solucion.h"
#include "Evaluador.h"

#include <string>
/* Clase utilizada para la escritura de la solucion en un archivo .txt 
Se incluye la semilla con el objetivo de que los experimentos sean reproducibles */
class EscritorSolucion
{
public:

    static void escribir(
        const std::string& archivo,
        const Instancia& instancia,
        const Solucion& solucion,
        const ResultadoEvaluacion& resultado,
        unsigned int semilla
    );
};

#endif