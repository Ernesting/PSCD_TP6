//*****************************************************************
// File:   ControlTareas.hpp
// Author: Adrian Fortea Valencia 815177
// Date:   Diciembre 2021
//
//*****************************************************************
#include <mutex>
#include <condition_variable>
#include "BoundedQueue.hpp"
#include <string>
using namespace std;


class ControlTareas{
public:

    ControlTareas(const int n); 
    void Getqueues();
    void publish(string mensaje);
    void read(string& mensaje);

private:
    int N;
    BoundedQueue<string>* Tareas;
    mutex mtxMonitor;
    condition_variable esperandolibre;
    condition_variable esperandovacia;
};
