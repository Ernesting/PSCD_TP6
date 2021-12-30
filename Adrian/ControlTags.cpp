#include "ControlTags.hpp"


ControlTags::ControlTags(const int n){
    TAMANYO=n;
    BoundedQueue<string> Tags(TAMANYO);
}

void ControlTags::Getqueues(){

    Tags.print();

}

void ControlTags::publish(string mensaje){
    unique_lock<mutex> lck(mtxMonitor); 

    while(Tags.length()>=TAMANYO){
        esperandolibre.wait(lck);
    }

    Tags.enqueue(mensaje);

    if(Tags.length()>0){
        esperandovacia.notify_all();
    }
   
}


void ControlTags::read(string& mensaje){
    unique_lock<mutex> lck(mtxMonitor); 

    while(Tags.length()==0){
        esperandovacia.wait(lck);
    }

    mensaje=Tags.first();
    Tags.dequeue();

    if(Tags.length()<50){
        esperandolibre.notify_all();
    }
}




