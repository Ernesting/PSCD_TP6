//*****************************************************************
// File:   Cliente.cpp
// Author: PSCD-Unizar
// Date:   noviembre 2015
// Coms:   Ejemplo de cliente con comunicación síncrona mediante sockets
//
//*****************************************************************
#include <iostream>
#include <chrono>
#include <thread>
#include "Socket.hpp"
#include <cstdlib>
using namespace std;

const int MESSAGE_SIZE = 4001; //mensajes de no más 4000 caracteres
 

 
 
 
 
void servWorker(, int PROCESS_ADDRESS, int SERVER_ADDRESS_GESTOR_COLAS, int SERVER_PORT_GESTOR_COLAS){
  string tweets;
  string *tags;
  Socket chan_GESTOR_COLAS(SERVER_ADDRESS_GESTOR_COLAS,SERVER_PORT_GESTOR_COLAS);
  
  const int MAX_ATTEMPS = 10;
  int count = 0;
  int socket_fd_GESTOR_COLAS;
  do {
    // Conexión con el servidor
    socket_fd_GESTOR_COLAS = chan_GESTOR_COLAS.Connect();
    count++;
    
    

    // Si error --> esperamos 1 segundo para reconectar
    if(socket_fd_GESTOR_COLAS == -1) {
        this_thread::sleep_for(chrono::seconds(1));
    }
  } while(socket_fd_GESTOR_COLAS == -1 && count < MAX_ATTEMPS);
  
  while(1){
    //read cola tareas
    //extraer tags
    //publicar tags
  }
    
    
    
    
    
 
  
  
}
 
 
 
 
 
//Primer parametro es la IP del server
//Segundo parametro es el puerto del server
int main(int argc, char* argv[]) {

  const int N = 6;
  thread procesos[N];
  
  string SERVER_ADDRESS_GESTOR_COLAS = argv[1];
  //int SERVER_PORT = 2000;
  int SERVER_PORT_GESTOR_COLAS = atoi(argv[2]);
  
  for (int i=0; i<N; i++){
    procesos[i] = thread(&servWorker, SERVER_ADDRESS_GESTOR_COLAS, SERVER_PORT_GESTOR_COLAS); 
  }
  for (int i=0; i<N; i++) {
    procesos_fd[i].join();
  }
  cout << "Bye bye" << endl;

}
