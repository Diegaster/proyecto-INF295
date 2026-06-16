#ifndef INSTANCIA_H
#define INSTANCIA_H

#include "Nodo.h"

#include <string>
#include <vector>
#include <unordered_map>

struct Instancia {

    std::string nombre;

    int numDepositos;
    int vehiculosPorDeposito;
    int capacidad;

    std::vector<Nodo> depositos;
    std::vector<Nodo> clientes;

    std::vector<Nodo> nodos;

    std::vector<std::vector<double>> distancias;

    std::unordered_map<int,int> clienteToIndex;
    std::unordered_map<int,int> depositoToIndex;
};

#endif