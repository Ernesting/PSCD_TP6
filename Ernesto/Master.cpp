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
 
//Primer parametro es la IP del server
//Segundo parametro es el puerto del server
int main(int argc, char* argv[]) {
    const string GET_TWEETS = "getTweets()";
	  const string MENS_FIN   = "endOfService";
    const string GET_QUEUES = "getQueues()";
    const string PUBLISH = "publish()";
    const string READ = "read()";
    // Dirección y número donde escucha el proceso servidor
    //string SERVER_ADDRESS = "localhost";
    string SERVER_ADDRESS_TWEETS = argv[1];
    //int SERVER_PORT = 2000;
    int SERVER_PORT_TWEETS = atoi(argv[2]);

    string SERVER_ADDRESS_GESTOR_COLAS = argv[3];
    //int SERVER_PORT = 2000;
    int SERVER_PORT_GESTOR_COLAS = atoi(argv[4]);
    // Creación del socket con el que se llevará a cabo
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
        return socket_fd_TWEETS;
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
        return socket_fd_GESTOR_COLAS;
    }

    string mensaje;
    int read_bytes;   //num de bytes recibidos en un mensaje
    int send_bytes;  //num de bytes enviados en un mensaje

    do {
        // Leer mensaje de la entrada estandar
        cout << "Escriba 'getTweets()' o 'endOfService' : " <<endl;
        //getline(cin, mensaje);
        // Enviamos el mensaje
        send_bytes = chan_TWEETS.Send(socket_fd_TWEETS, GET_TWEETS);

        if(send_bytes == -1) {
            cerr << "Error al enviar datos: " << strerror(errno) << endl;
            // Cerramos el socket
            chan_TWEETS.Close(socket_fd_TWEETS);
            exit(1);
        }
        // Buffer para almacenar la respuesta
        string buffer;
  
        // Recibimos la respuesta del servidor //sera un char[] en el que habra que separar 25 mensajes
        read_bytes = chan_TWEETS.Recv(socket_fd_TWEETS, buffer, MESSAGE_SIZE);
			
		//	char *dup = strdup(buffer.c_str());
		//	char *grupos[5];
      
      istringstream iss(buffer);
      string grupos_de_5_tweets[5];
      
      for ( int i = 0; i < 5; i++){
      grupos_de_5_tweets[i] = "";
        for (int j = 0; j < 5; j++){
          string aux;
          getline (iss, aux, '\n');
          aux += '\n';
          grupos_de_5_tweets[i]+=aux;
          
        }
      }
      
		/*	grupos[0] = strtok (dup,"\n");
			
			for (int i = 0; i <5; i++){
				  grupos[i] = grupos[i]
					grupos[i] =  strtok(NULL, "\n");
				
			}
			
			for (int i = 0; i < 25; i++){
				cout << grupos[i] << " " << i <<endl;
				
			}*/
   cout << grupos_de_5_tweets[0] <<endl;
           
    mensaje = GET_QUEUES;
    send_bytes = chan_GESTOR_COLAS.Send(socket_fd_GESTOR_COLAS, mensaje);
    if(send_bytes == -1) {
        cerr << "Error al enviar datos: " << strerror(errno) << endl;
        // Cerramos el socket
        chan_GESTOR_COLAS.Close(socket_fd_GESTOR_COLAS);
        exit(1);
    }
            

            // Recibimos la respuesta del servidor //sera un char[] en el que habra que separar 25 mensajes
            read_bytes = chan_GESTOR_COLAS.Recv(socket_fd_GESTOR_COLAS, buffer, MESSAGE_SIZE);
            cout << "Mensaje recibido: " << buffer <<endl;
    
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

    return error_code;
}