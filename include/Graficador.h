#ifndef GRAFICADOR_H
#define GRAFICADOR_H

#include "Instancia.h"
#include "Solucion.h"

#include <string>

class Graficador
{
public:

    static void graficar(
        const std::string& archivo,
        const Instancia& instancia,
        const Solucion& solucion
    );
};

#endif