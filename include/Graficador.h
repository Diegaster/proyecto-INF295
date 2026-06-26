#ifndef GRAFICADOR_H
#define GRAFICADOR_H

#include "Instancia.h"
#include "Solucion.h"

#include <string>

/* Clase que se encarga de generar un gráfico estilo mapa en .ppm para visualizar las soluciones.
Solucion se incluye para poder obtener los datos de esta misma y poder mapearla.
Mientras que archivo e instancia se incluyen para dar nombre al archivo y destinar a una correcta subcarpeta de
output/.*/
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