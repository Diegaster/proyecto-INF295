#include "../include/LectorInstancias.h"

#include <cmath>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <algorithm>

Instancia LectorInstancias::leer(const std::string& archivo){
    std::ifstream in(archivo);

    if (!in.is_open()){
        throw std::runtime_error(
            "No se pudo abrir el archivo: " + archivo
        );
    }

    Instancia instancia;
    std::string line;

    //------------------------------------------------
    // Nombre de instancia
    //------------------------------------------------

    while (std::getline(in, line)){
        if (!line.empty())
        {
            instancia.nombre = line;
            break;
        }
    }

    //------------------------------------------------
    // Parámetros generales
    //------------------------------------------------

    while (std::getline(in, line)){
        if (line.find("DEPOSITOS:") != std::string::npos)
        {
            break;
        }
    }

    {
        std::stringstream ss(line);

        std::string basura;

        ss >> basura; // DEPOSITOS:
        ss >> instancia.numDepositos;

        ss >> basura; // VEHICULOS_POR_DEPOSITO:
        ss >> instancia.vehiculosPorDeposito;

        ss >> basura; // CAPACIDAD:
        ss >> instancia.capacidad;
    }

    //------------------------------------------------
    // Buscar bloque DEPOSITOS
    //------------------------------------------------

    while (std::getline(in, line)){
        if (line.find("DEPOSITOS") != std::string::npos &&
            line.find(":") == std::string::npos)
        {
            break;
        }
    }

    //------------------------------------------------
    // Saltar encabezado ID X Y ...
    //------------------------------------------------

    std::getline(in, line);

    //------------------------------------------------
    // Leer depósitos
    //------------------------------------------------

    for (int i = 0; i < instancia.numDepositos;){
        if (!std::getline(in, line))
            break;

        if (line.empty())
            continue;

        Nodo d;

        std::stringstream ss(line);

        std::string idTexto;

        ss >> idTexto;

        if (idTexto[0] == 'D'){
            d.id = std::stoi(idTexto.substr(1));
        }
        else {
            d.id = std::stoi(idTexto);
        }

        ss >> d.x
           >> d.y
           >> d.demanda
           >> d.e_i
           >> d.l_i
           >> d.s_i;

        d.esDeposito = true;

        instancia.depositos.push_back(d);

        ++i;
    }

    //------------------------------------------------
    // Buscar CLIENTES
    //------------------------------------------------

    while (std::getline(in, line)){
        if (line.find("CLIENTES") != std::string::npos){
            break;
        }
    }

    //------------------------------------------------
    // Saltar encabezado ID X Y ...
    //------------------------------------------------

    std::getline(in, line);

    //------------------------------------------------
    // Leer clientes
    //------------------------------------------------

    while (std::getline(in, line)){
        if (line.empty())
            continue;

        Nodo c;

        std::stringstream ss(line);

        ss >> c.id
           >> c.x
           >> c.y
           >> c.demanda
           >> c.e_i
           >> c.l_i
           >> c.s_i;

        c.esDeposito = false;

        instancia.clientes.push_back(c);
    }

    //------------------------------------------------
    // Vector global de nodos
    //------------------------------------------------

    for (const auto& d : instancia.depositos)
        instancia.nodos.push_back(d);

    for (const auto& c : instancia.clientes)
        instancia.nodos.push_back(c);

    //------------------------------------------------
    // Índices rápidos
    //------------------------------------------------

    for (int i = 0; i < (int)instancia.nodos.size(); i++){
        const Nodo& n = instancia.nodos[i];

        if (n.esDeposito)
        {
            instancia.depositoToIndex[n.id] = i;
        }
        else{
            instancia.clienteToIndex[n.id] = i;
        }
    }

    //------------------------------------------------
    // Matriz de distancias
    //------------------------------------------------

    int N = instancia.nodos.size();

    instancia.distancias.assign(
        N,
        std::vector<double>(N, 0.0)
    );

    for (int i = 0; i < N; i++){
        for (int j = i + 1; j < N; j++){
            double dx =
                instancia.nodos[i].x -
                instancia.nodos[j].x;

            double dy =
                instancia.nodos[i].y -
                instancia.nodos[j].y;

            double d =
                std::sqrt(dx * dx + dy * dy);

            instancia.distancias[i][j] = d;
            instancia.distancias[j][i] = d;
        }
    }

    return instancia;
}
