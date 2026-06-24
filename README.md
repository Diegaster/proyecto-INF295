# Proyecto MDVRP-TW - Greedy + Tabu Search

## Autores

Diego Muñoz Carrillo
ROL: 202273578-3

## Requisitos

* Compilador compatible con C++17
* GNU Make

Probado con:

g++ 11+
make 4+

No requiere librerías externas.

## Compilación

Desde la carpeta raíz del proyecto ejecutar:

make

Se generará el ejecutable:

./mdvrptw

## Estructura del Proyecto

├── src/
├── include/
├── data/
├── output/
├── Makefile
└── README.md

* `src/`: implementación de clases y algoritmos.
* `include/`: archivos de cabecera.
* `data/`: instancias de entrada.
* `output/`: resultados generados.

## Configuración

Los parámetros del algoritmo son los siguientes:

factorIteraciones
factorVecinos
tabuTenure
usarSwap
usarRelocate
semilla

Descripción:

* `factorIteraciones`: factor utilizado para calcular las iteraciones máximas.
* `factorVecinos`: factor utilizado para calcular la cantidad máxima de vecinos evaluados. Si vale 0 se utiliza exploración exhaustiva.
* `tabuTenure`: tamaño de la lista Tabú.
* `usarSwap`: habilita operador Swap.
* `usarRelocate`: habilita operador Relocate.
* `semilla`: semilla utilizada para exploración aleatoria.


## Ejecución

Ejecutar:

./mdvrptw

El programa procesará las instancias disponibles en la carpeta:

data/

y almacenará los resultados en:

output/

Si se quiere ignorar alguna instancia en la ejecución sin perderla, se puede dejar en la carpeta

dont_use_data/

## Salidas Generadas

Para cada instancia se generan:

* Solución Greedy.
* Solución Tabu Search.
* Métricas de evaluación.
* Archivos CSV consolidados.
* Visualización de rutas en formato PPM, se ocupó Adobe Photoshop para poder visualizar los archivos PPM.