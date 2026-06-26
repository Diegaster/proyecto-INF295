#ifndef INSTANCIA_H
#define INSTANCIA_H

#include "Nodo.h"

#include <string>
#include <vector>
#include <unordered_map>

/* Struct que se encarga de tener todo lo referente a la instancia una vez que se lee el archivo correspondiente.
nombre = nombre del archivo
numDepositos = cantidad de depositos del archivo
vehiculosPorDeposito = cantidad de vehiculos en cada deposito, incluida en el archivo
capacidad = la capacidad de carga maxima de los vehiculos
vector depositos que se encarga de obtener y guardar lo referente a cada deposito, como su id, ubicacion,
demanda, distintos sub i, entre otros. Esto es analogo para el vector clientes.
vector nodos se encarga de ser un vector global que guarda tanto depositos como clientes.
matriz distancias se encarga de guardar las distancias entre nodos una vez son calculadas, para cuando se
soliciten nuevamente, obtenerlas en un tiempo O(1).
mientras que las ultimas estructuras map se encargan de proveer un acceso rapido al index en que se encuentren
los depositos o clientes dentro del vector nodos.
*/
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