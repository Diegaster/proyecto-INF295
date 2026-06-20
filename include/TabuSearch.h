#ifndef TABU_SEARCH_H
#define TABU_SEARCH_H

#include "Instancia.h"
#include "Solucion.h"

struct MovimientoTabu
{
    int clienteA;
    int clienteB;
};

class TabuSearch
{
public:

    static Solucion optimizar(
        const Instancia& instancia,
        const Solucion& inicial,
        int maxIteraciones = 500,
        int tabuTenure = 10
    );
};

#endif