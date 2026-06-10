#ifndef NODO_H
#define NODO_H

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
