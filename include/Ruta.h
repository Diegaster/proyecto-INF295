#ifndef RUTA_H
#define RUTA_H

#include <vector>

/* El struct ruta se encarga de guardar el id del deposito del cual se inicia y finaliza el recorrido.
Mientras que el vector clientes se encarga de guardar la ruta en si */
struct Ruta {

    int depositoId;

    std::vector<int> clientes;

};

#endif
