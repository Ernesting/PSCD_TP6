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
    //condition_variable okLeer; //para esperar a que se pueda leer el repositorio
    //condition_variable okEscribir; //para esperar a que se pueda escribir en el repositorio
    //int nLec; //para contar el numero de procesos leyendo el repositorio
    //int nEsc; //para contar el numero de procesos escribiendo en el repositorio
public:
    //--------------------------- constructores
    //el parametro "_fichero" se copiara sobre el atributo "fichero" del monitor
    MonitorCliente(string _fichero);
    //--------------------------- destructor
    ~MonitorCliente();
    //--------------------------- usuario
    //_palabra es la frase que se introducirá en la ultima linea del repositorio junto a un \n
    void append(string _palabra);
    //abre el repositorio, cuenta la frecuencia de cada palabra y la muestra
    void show();
    
};
#endif
