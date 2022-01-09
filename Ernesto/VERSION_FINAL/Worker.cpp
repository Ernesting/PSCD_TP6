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
#include <sstream>
using namespace std;

const int MESSAGE_SIZE = 4001; //mensajes de no más 4000 caracteres
const string READ = "PROCESAR_TAREA";
const string PUBLISH = "GUARDAR_DATOS";

//Pre:

bool extraer_tags (string tweets, string& resultado){
    static string aux;
    static char *aux4;
    static bool hay_hashtags = false;
    if (strlen(tweets.data()) == 0 && hay_hashtags == true){
      //cout<<aux<<endl;
      //cout<<aux4<<endl;
      //resultado = strtok(aux4,"#");
      //resultado = strtok(NULL, " ;,.\t\n");
      //if (aux4 == NULL){return 0;}
      if (aux.length()!= 0){
        
          aux4 = strtok(NULL, "#");
          if (aux4 == NULL){return 0;}
            resultado = aux4;
            //aux4 = strtok(NULL," ;,.\t\n");
            // cout<<aux<<endl;
            //cout<<aux4<<endl;
            return 1;
     }else{return 0;}
    }else{
      aux = " "+tweets;//tweets.copy(aux, sizeof tweets);
      
      size_t i = 0;
      size_t len = tweets.length();
      bool hashtag = false;
      while(i < len){
        if (hashtag == false){
          if (tweets[i] == '#'){
            i++;
            hashtag = true;
            
          }else{
           tweets.erase(i,1);
            len--;
          }
        }
        else{
          if (isalnum(tweets[i])){
            i++;
          }else{
            tweets.erase(i,1);
            len--;
            hashtag = false;
          }
        }
      }
      //cout<<len<<endl;
      if (len==0){
        hay_hashtags = false;
        return 0;
      }
      //cout<<tweets<<endl;
      else{
        aux = " "+tweets;
        aux4 = strdup(aux.c_str());
        aux4 = strtok(aux4, "#");
        //cout<<aux4<<endl;
        hay_hashtags = true;
        return 1;
      }
      
     /* aux = " "+tweets;
      aux4 = strdup(aux.c_str());
      aux4 = strtok(aux4, "#");
      aux4= strtok(NULL,"#");
      resultado = aux4;*/
    }
    

    
}
 
 
 
void servWorker(string SERVER_ADDRESS_GESTOR_COLAS, int SERVER_PORT_GESTOR_COLAS){
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
    //read
    string mensaje;
    int read_bytes;  
    int send_bytes;
    
    send_bytes = chan_GESTOR_COLAS.Send(socket_fd_GESTOR_COLAS, READ);

        if(send_bytes == -1) {
            cerr << "Error al enviar datos: " << strerror(errno) << endl;
            // Cerramos el socket
            chan_GESTOR_COLAS.Close(socket_fd_GESTOR_COLAS);
            exit(1);
        }
    //read cola tareas
    string buffer;
  
    read_bytes = chan_GESTOR_COLAS.Recv(socket_fd_GESTOR_COLAS, buffer, MESSAGE_SIZE);
    //Quitamos las 6 primeras letras que equivalen a "TAREA,"
    buffer.erase(0,6);
    //extraer tags
    string resultado;
    extraer_tags(buffer,resultado);
    while(extraer_tags("",resultado)){
      //publicar tags
      send_bytes = chan_GESTOR_COLAS.Send(socket_fd_GESTOR_COLAS, PUBLISH + "," + resultado);
      if(send_bytes == -1) {
            cerr << "Error al enviar datos: " << strerror(errno) << endl;
            // Cerramos el socket
            chan_GESTOR_COLAS.Close(socket_fd_GESTOR_COLAS);
            exit(1);
        }
      //leer ok
      read_bytes = chan_GESTOR_COLAS.Recv(socket_fd_GESTOR_COLAS, buffer, MESSAGE_SIZE);
    }
    

    
  }  
}
 
//N es el numero de procesos a la vez
//Primer parametro es la IP del server
//Segundo parametro es el puerto del server
int main(int argc, char* argv[]) {
  
  /*string resultado;
  extraer_tags("as uno 2dos; AS#KJDAKmelaacabodeputosacar",resultado);
  while(extraer_tags("",resultado)){
    cout<<resultado<<endl;
  }*/
  //cout <<resultado<<endl;
  const int N = 6;
  thread procesos[N];
  
  string SERVER_ADDRESS_GESTOR_COLAS = argv[1];
  //int SERVER_PORT = 2000;
  int SERVER_PORT_GESTOR_COLAS = atoi(argv[2]);
  
  for (int i=0; i<N; i++){
    procesos[i] = thread(&servWorker, SERVER_ADDRESS_GESTOR_COLAS, SERVER_PORT_GESTOR_COLAS ); 
  }
  for (int i=0; i<N; i++) {
    procesos[i].join();
  }
  cout << "Bye bye" << endl;
}