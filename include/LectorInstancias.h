#ifndef LECTOR_INSTANCIAS_H
#define LECTOR_INSTANCIAS_H

#include "Instancia.h"
#include <string>

/* Unicamente se define la clase LectorInstancias, donde guarda una instancia luego de haberla leido en el
archivo de instancia*/
class LectorInstancias {

public:

    static Instancia leer(const std::string& archivo);

};

#endif
