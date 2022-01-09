//Fichero: MonitorCliente.hpp
#ifndef MONITOR_CLIENTE_HPP
#define MONITOR_CLIENTE_HPP

#include <mutex>
#include <condition_variable>
#include <iostream>
#include <thread>
#include <string>
#include <cstdlib>
#include <chrono>
#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <fstream>
//----------------------------------------------------------
using namespace std; //mutex, condition_variable, etc

class MonitorCliente{
private:
    string fichero;
    //int hanBajado;
    //añadir las que se consideren necesarias

    mutex mtx; //para la ejecucion de procs en exclusion mutua
    condition_variable okLeer; //para esperar a que el CLIENTE este vacio
    condition_variable okEscribir;
    int nLec;
    int nEsc;
public:
    //--------------------------- constructores
    //el parametro "N_W" se copiara sobre el atributo "N_W" del monitor
    MonitorCliente(string _fichero);
    //--------------------------- destructor
    ~MonitorCliente();
    //--------------------------- usuario
    //"i" es el identificador de usuario
    void append(string _palabra);
    void show();
    
};
#endif
