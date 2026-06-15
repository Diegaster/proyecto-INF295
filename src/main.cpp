#include "../include/LectorInstancias.h"
#include "../include/Evaluador.h"
#include "../include/Solucion.h"
#include "../include/ConstructorGreedy.h"
#include <iostream>

int main()
{
    Instancia instancia =
    LectorInstancias::leer("data/C102.txt");

    std::cout << instancia.nombre << '\n';

    std::cout << "Depositos: "
            << instancia.depositos.size()
            << '\n';

    std::cout << "Clientes: "
            << instancia.clientes.size()
            << '\n';

    for(const auto& d : instancia.depositos)
    {
        std::cout
            << "D" << d.id
            << " (" << d.x << ", " << d.y << ")\n";
    }
    Solucion solucion =
        ConstructorGreedy::construir(
            instancia
        );

    ResultadoEvaluacion resultado =
        Evaluador::evaluar(
            instancia,
            solucion
        );

    std::cout
        << "Fitness: "
        << resultado.fitness
        << '\n';

    std::cout
        << "Factible: "
        << resultado.factible
        << '\n';

    std::cout
        << "Distancia: "
        << resultado.distanciaTotal
        << '\n';

    std::cout
        << "Penalizacion: "
        << resultado.penalizacionTotal
        << '\n';

    std::cout
        << "Vehiculos: "
        << resultado.vehiculosUsados
        << '\n';
    return 0;
}
