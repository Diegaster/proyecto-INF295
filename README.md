# Proyecto MDVRP-TW - Greedy + Tabu Search

## Autor

Diego Muñoz Carrillo
ROL: 202273578-3

## Requisitos

* Compilador compatible con C++17
* GNU Make

Probado con:

g++ 11+
make 4+

No requiere librerías externas.

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

Es importante mencionar que ambos factores se dividen por el numero de clientes de la instancia, con el
objetivo de optimizar el tiempo de ejecución, ya que a mayor cantidad de clientes, se expande el vecindario,
y por ende el tiempo de ejecución
## Compilación y Ejecución

El programa se puede compilar y ejecutar con los comandos incluidos en el Makefile, cerciorándose de que
se esté en la misma carpeta raíz del proyecto.

Primero se debe de ejecutar: make all

Luego: make run

Aquí es donde se ejecutará la implementación y se irán pidiendo valores de los parámetros de configuración
mediante la consola/terminal. Una vez que se ingresen todos, el programa ejecutará Greedy, Tabu Search o ambos
para todas las instancias que se encuentren en la carpeta data/.

Una vez que se haya terminado con las pruebas de ejecución, se puede ocupar el comando: make clean

El programa almacenará los resultados en:

output/

Si se quiere ignorar alguna instancia en la ejecución sin perderla, se puede dejar en la carpeta:

dont_use_data/

## Salidas Generadas

Para cada instancia se generan:

* Solución Greedy en un .txt. (Si se pide por consola)
* Solución Tabu Search en un .txt. (Si se pide por consola)
* Métricas de evaluación.
* Archivo CSV consolidado, con un "resumen" de todas las instancias ejecutadas.
* Visualización de rutas en formato PPM, se ocupó Adobe Photoshop para poder visualizar los archivos PPM.