//*****************************************************************
// File:   ControlTags.cpp
// Author: Adrian Fortea Valencia 815177
// Date:   Diciembre 2021
//
//*****************************************************************
#include "ControlTags.hpp"


ControlTags::ControlTags(const int n){
    N=n;
    Tags=new BoundedQueue<string>(N);
}

void ControlTags::Getqueues(){
    Tags->print();
}

void ControlTags::publish(string mensaje){
    unique_lock<mutex> lck(mtxMonitor); 

    while(Tags->length()>=N){
        esperandolibre.wait(lck);
    }

    Tags->enqueue(mensaje);

    if(Tags->length()>0){
        esperandovacia.notify_all();
    }
   
}


void ControlTags::read(string& mensaje){
    unique_lock<mutex> lck(mtxMonitor); 

    while(Tags->length()==0){
        esperandovacia.wait(lck);
    }

    mensaje=Tags->first();
    Tags->dequeue();

    if(Tags->length()<N){
        esperandolibre.notify_all();
    }
}
