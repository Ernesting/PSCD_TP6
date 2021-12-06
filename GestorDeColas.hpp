#ifndef GESTOR_DE_COLAS_HPP
#define GESTOR_DE_COLAS_HPP
#include <string>
#include <iostream>
using namespace std;
const int NUM_COLAS = 3;
struct tupla{
    int palabra_clave[NUM_COLAS];
    string nombre_cola[NUM_COLAS];
};
tupla getQueues();
bool publish(int cola, string mensaje);
bool read(int cola, string& mensaje);
#endif
