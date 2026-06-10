#include "../include/Evaluador.h"

double Evaluador::evaluar(
    const Instancia& instancia,
    const Solucion& solucion,
    double alpha,
    double beta
)
{
    double distanciaTotal = 0.0;

    double penalizacion = 0.0;

    int vehiculosUsados =
        solucion.rutas.size();

    // TODO:
    // Calcular distancia
    // Calcular tiempos
    // Calcular penalizaciones
    // Verificar capacidad

    return distanciaTotal
         + alpha * penalizacion
         + beta * vehiculosUsados;
}
