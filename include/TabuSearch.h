#ifndef TABU_SEARCH_H
#define TABU_SEARCH_H

#include "Instancia.h"
#include "Solucion.h"

/* Se define un movimiento Tabu como aquel intercambio entre dos clientes para el movimiento Swap.
En el caso de Relocate, cliente B valdrá -1.*/
struct MovimientoTabu
{
    int clienteA;
    int clienteB;
};

/* Se define la clase TabuSearch, que incluye la configuracion del Tabu con los parametros planteados en el
informe. Se dan valores predefinidos por completitud, pero estos mismos se reemplazan al momento de pedirse por
consola.

Tambien se guarda una Solucion del tabu como aquella que contiene la solucion inicial del Greedy, la instancia
y la misma configuracion del tabu para escribir todos los resultados en un archivo .csv*/
class TabuSearch
{
public:

    struct ConfigTabu{
        int factorIteraciones = 50000;

        int tabuTenure = 10;

        int factorVecinos = 0;

        bool usarSwap = true;
        bool usarRelocate = true;

        unsigned int semilla;
    };

    static Solucion optimizar(
        const Instancia& instancia,
        const Solucion& inicial,
        const ConfigTabu& config
    );
};

#endif