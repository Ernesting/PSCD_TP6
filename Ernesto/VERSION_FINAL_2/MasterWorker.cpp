//*****************************************************************
// File:   MasterWorker.cpp
// Author: Ernesto Bielsa 798799
// Date:   Diciembre 2021
// Coms:   ./Worker <ip gestor tweets> <puerto gestor tweets> <ip gestor colas> 
//					<puerto gestor colas master> <puerto gestor colas worker>
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


void servMaster(string SERVER_ADDRESS_TWEETS, int SERVER_PORT_TWEETS, string SERVER_ADDRESS_GESTOR_COLAS, int SERVER_PORT_GESTOR_COLAS){
  const string GET_TWEETS = "getTweets()";
	const string MENS_FIN   = "endOfService";
    const string GET_QUEUES = "getQueues()";
    const string PUBLISH = "ANYADIR_TAREA";
    const string READ = "read()";
    // la comunicación con el servidor.
    Socket chan_TWEETS(SERVER_ADDRESS_TWEETS, SERVER_PORT_TWEETS);
    Socket chan_GESTOR_COLAS(SERVER_ADDRESS_GESTOR_COLAS,SERVER_PORT_GESTOR_COLAS);

    // Conectamos con el servidor. Probamos varias conexiones
    const int MAX_ATTEMPS = 10;
    int count = 0;
    int socket_fd_TWEETS;
    int socket_fd_GESTOR_COLAS;
    do {
        // Conexión con el servidor
        socket_fd_TWEETS = chan_TWEETS.Connect();
        count++;
        
        // Si error --> esperamos 1 segundo para reconectar
        if(socket_fd_TWEETS == -1) {
            this_thread::sleep_for(chrono::seconds(1));
        }
    } while(socket_fd_TWEETS == -1 && count < MAX_ATTEMPS);
    
    if(socket_fd_TWEETS == -1) {
        exit(1);
    }
    count = 0;
    do {
        // Conexión con el servidor
        socket_fd_GESTOR_COLAS = chan_GESTOR_COLAS.Connect();
        count++;
        
        // Si error --> esperamos 1 segundo para reconectar
        if(socket_fd_GESTOR_COLAS == -1) {
            this_thread::sleep_for(chrono::seconds(1));
        }
    } while(socket_fd_GESTOR_COLAS == -1 && count < MAX_ATTEMPS);

    // Chequeamos si se ha realizado la conexión
    if(socket_fd_GESTOR_COLAS == -1) {
        exit(2);
    }

    string mensaje;
    int read_bytes;   //num de bytes recibidos en un mensaje
    int send_bytes;  //num de bytes enviados en un mensaje

    do {
        //getline(cin, mensaje);
        // Enviamos el mensaje
        send_bytes = chan_TWEETS.Send(socket_fd_TWEETS, GET_TWEETS);

        if(send_bytes == -1) {
            cerr << "Error al enviar datos: " << strerror(errno) <<errno<< endl;
            // Cerramos el socket
            chan_TWEETS.Close(socket_fd_TWEETS);
            exit(1);
        }
        // Buffer para almacenar la respuesta
        string buffer;
  
        // Recibimos la respuesta del servidor //sera un char[] en el que habra que separar 25 mensajes
        for (int i = 0; i< 5; i++){
          read_bytes = chan_TWEETS.Recv(socket_fd_TWEETS, buffer, MESSAGE_SIZE);
  
  		//	char *dup = strdup(buffer.c_str());
  		//	char *grupos[5];
        
        /*  istringstream iss(buffer);
          string grupos_de_5_tweets[5];
        
          for ( int i = 0; i < 5; i++){
              grupos_de_5_tweets[i] = "";
              for (int j = 0; j < 5; j++){
                  string aux;
                  getline (iss, aux, '\n');
                  aux += '\n';
                  grupos_de_5_tweets[i]+=aux;
            
              }
          }*/
        
          //for (int i = 0; i< 5; i++){
              mensaje = PUBLISH;
              //cout<< grupos_de_5_tweets[i] + "\n";
              send_bytes = chan_GESTOR_COLAS.Send(socket_fd_GESTOR_COLAS, mensaje+","+buffer);
              if(send_bytes == -1) {
                  cerr << "Error al enviar datos: " << strerror(errno) << errno << endl;
                  // Cerramos el socket
                  chan_GESTOR_COLAS.Close(socket_fd_GESTOR_COLAS);
                  exit(1);
              }
              read_bytes = chan_GESTOR_COLAS.Recv(socket_fd_GESTOR_COLAS, buffer, MESSAGE_SIZE);
              //cout << "Mensaje recibido: " << buffer <<endl;
          //} 
      }    

        //}
    } while(mensaje != MENS_FIN);

    // Cerramos el socket
    int error_code = chan_TWEETS.Close(socket_fd_TWEETS);
    if(error_code == -1) {
        cerr << "Error cerrando el socket: " << strerror(errno) << endl;
    }
    error_code = chan_GESTOR_COLAS.Close(socket_fd_GESTOR_COLAS);
    if(error_code == -1) {
        cerr << "Error cerrando el socket: " << strerror(errno) << endl;
    }

    // Despedida
    cout << "Bye bye" << endl;

}

bool extraer_tags (string tweets, string& resultado){
    static string aux;
    static char *aux4;
    static bool hay_hashtags = false;
    if (strlen(tweets.data()) == 0 && hay_hashtags == true){
      if (aux.length()!= 0){
        
          aux4 = strtok(NULL, "#");
          if (aux4 == NULL){return 0;}
            resultado = aux4;
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
      if (len==0){
        hay_hashtags = false;
        return 0;
      }
      else{
        aux = " "+tweets;
        aux4 = strdup(aux.c_str());
        aux4 = strtok(aux4, "#");
        hay_hashtags = true;
        return 1;
      }
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

  const int N = 6;
  thread workers[N];
  thread master;
  string SERVER_ADDRESS_TWEETS = argv[1];
  //int SERVER_PORT = 2000;
  int SERVER_PORT_TWEETS = atoi(argv[2]);
  string SERVER_ADDRESS_GESTOR_COLAS = argv[3];
  //int SERVER_PORT = 2000;
  int SERVER_PORT_GESTOR_COLAS_MASTER = atoi(argv[4]);
  int SERVER_PORT_GESTOR_COLAS_WORKER = atoi(argv[5]);
  
  master = thread(&servMaster, SERVER_ADDRESS_TWEETS, SERVER_PORT_TWEETS, SERVER_ADDRESS_GESTOR_COLAS, SERVER_PORT_GESTOR_COLAS_MASTER);
  for (int i=0; i<N; i++){
    workers[i] = thread(&servWorker, SERVER_ADDRESS_GESTOR_COLAS, SERVER_PORT_GESTOR_COLAS_WORKER ); 
  }
  master.join();
  for (int i=0; i<N; i++) {
    workers[i].join();
  }
  cout << "Bye bye" << endl;
}
