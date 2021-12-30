#include "ControlTareas.hpp"


ControlTareas::ControlTareas(const int n){
    N=n;
    Tareas=new BoundedQueue<string>(N);
}

void ControlTareas::Getqueues(){
    Tareas->print();
}

void ControlTareas::publish(string mensaje){
    unique_lock<mutex> lck(mtxMonitor); 

    while(Tareas->length()>=N){
        esperandolibre.wait(lck);
    }

    Tareas->enqueue(mensaje);

    if(Tareas->length()>0){
        esperandovacia.notify_all();
    }
   
}


void ControlTareas::read(string& mensaje){
    unique_lock<mutex> lck(mtxMonitor); 

    while(Tareas->length()==0){
        esperandovacia.wait(lck);
    }

    mensaje=Tareas->first();
    Tareas->dequeue();

    if(Tareas->length()<N){
        esperandolibre.notify_all();
    }
}




