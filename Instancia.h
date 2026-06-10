#ifndef INSTANCIA_H
#define INSTANCIA_H

#include "Nodo.h"
#include <string>
#include <vector>

struct Instancia {

    std::string nombre;

    int numDepositos;

    int vehiculosPorDeposito;

    int capacidad;

    std::vector<Nodo> depositos;

    std::vector<Nodo> clientes;
};

#endif
