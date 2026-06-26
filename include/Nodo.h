#ifndef NODO_H
#define NODO_H

/* El struct Nodo se encarga de guardar todo lo referente a un deposito o cliente, a partir de la lectura
del archivo de la instancia */
struct Nodo {

    int id;

    double x;
    double y;

    int demanda;

    int e_i;
    int l_i;

    int s_i;

    bool esDeposito;
};

#endif