//*****************************************************************
// File:   mainBoundedQueue.cpp
// Author: PSCD-Unizar
// Date:   septiembre 2018
// Coms:   Parte del práctica 1 de PSCD
//         ejemplo de uso del TAD "BoundedQueue"
//*****************************************************************

#include <iostream>
#include <thread>
#include <string>
#include "Socket.hpp"

#include "BoundedQueue.hpp"

using namespace std;
const int MESSAGE_SIZE = 4001;
const string QUEUES = "1 -> Cola de tareas\n2 -> Cola de QOS\n3 -> Cola de tags\n";


//-----------------------------------------------------
int main(int argc, char *argv[]) {

    const int N = 50;
    BoundedQueue<string> cola_tareas(N);
    BoundedQueue<string> cola_qos(N);
    BoundedQueue<string> cola_tags(N);
	
	
	
    string MENS_FIN = "endOfService";
	  string GET_QUEUES = "getQueues()";
    string PUBLISH = "publish()";
    string READ = "read()";
    string str;
    // Puerto donde escucha el proceso servidor
    //int SERVER_PORT = 2000;
    int SERVER_PORT = atoi(argv[1]);
    // Creación del socket con el que se llevará a cabo
    // la comunicación con el servidor.
    Socket chan(SERVER_PORT);

    // Bind
    int socket_fd =chan.Bind();
    if (socket_fd == -1) {
        string mensError(strerror(errno));
        cerr << "Error en el bind: " + mensError + "\n";
        exit(1);
    }

    // Listen
    int max_connections = 1;
    int error_code = chan.Listen(max_connections);
    if (error_code == -1) {
        string mensError(strerror(errno));
        cerr << "Error en el listen: " + mensError + "\n";
        // Cerramos el socket
        chan.Close(socket_fd);
        exit(1);
    }

    // Accept
    int client_fd = chan.Accept();
    if (client_fd == -1) {
        string mensError(strerror(errno));
        cerr << "Error en el accept: " + mensError + "\n";
        // Cerramos el socket
        chan.Close(socket_fd);
        exit(1);
    }

    // Buffer para recibir el mensaje
    int length = 100;
    string buffer;
    int rcv_bytes;   //num de bytes recibidos en un mensaje
    int send_bytes;  //num de bytes enviados en un mensaje

    bool out = false; // Inicialmente no salir del bucle
    while (!out) {
        // Recibimos el mensaje del cliente
        rcv_bytes = chan.Recv(client_fd, buffer, MESSAGE_SIZE);
        if(rcv_bytes == -1) {
            string mensError(strerror(errno));
            cerr << "Error al recibir datos: " + mensError + "\n";
            // Cerramos los sockets
            chan.Close(client_fd);
            chan.Close(socket_fd);
        }

        cout << "Mensaje recibido: '" + buffer + "'\n";

        // Si recibimos "END OF SERVICE" --> Fin de la comunicación
        if (buffer == MENS_FIN) {
            out = true; // Salir del bucle
        } else if (buffer == GET_QUEUES) {
            str = QUEUES;
            send_bytes = chan.Send(client_fd, str);
            if(send_bytes == -1) {
                string mensError(strerror(errno));
                cerr << "Error al enviar datos: " + mensError + "\n";
                // Cerramos los sockets
                chan.Close(client_fd);
                chan.Close(socket_fd);
                exit(1);
            }
        }
        else if (buffer == PUBLISH){
           // if (x == 1) {cola_tareas.enqueue(y); }
           //else if (x == 2) {cola_qos.enqueue(y); }
           //else if (x == 3) {cola_tags.enqueue(y); }
        }
        else if (buffer == READ){
          // if (x == 1) {str = cola_tareas.first(); cola_tareas.dequeue(); }
           //else if (x == 2) {str = cola_qos.first(); cola_qos.dequeue(); }
           //else if (x == 3) {str = cola_tags.first(); cola_tags.dequeue(); }
        }
    }

    // Cerramos el socket del cliente
    error_code = chan.Close(client_fd);
    if (error_code == -1) {
        string mensError(strerror(errno));
        cerr << "Error cerrando el socket del cliente: " + mensError + "\n";
    }

    // Cerramos el socket del servidor
    error_code = chan.Close(socket_fd);
    if (error_code == -1) {
        string mensError(strerror(errno));
        cerr << "Error cerrando el socket del servidor: " + mensError + "\n";
    }
    // Mensaje de despedida
    cout << "Bye bye" << endl;

    return error_code;

    return 0;
}