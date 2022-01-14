//------------------------------------------------------------------------------
// File:   Gestor_Colas.cpp
// Author: Adrián Fortea Valencia 815177
//         Saúl Daniel Soriano 815743
// Date:   Diciembre 2021
// Coms: ./Gestor_Colas <puerto gestor master> <puerto gestor worker> <puerto gestor cliente>
//------------------------------------------------------------------------------

#include "Socket.hpp"
#include "ControlTareas.hpp"
#include "ControlTags.hpp"
#include <iostream>
#include <sstream> //Para separar la cadenas
#include <string>
#include <ctime>
#include <chrono>
#include <thread>
const int MESSAGE_SIZE = 10001; //mensajes de no más 4000 caracteres
using namespace std;

void partesMensaje(string buffer, string& accion, string& resto){

    stringstream input_stringstream(buffer);
    getline(input_stringstream, accion, ',');
    getline(input_stringstream, resto, '\0');
}

void GestorMaster(int SERVER_PORT, ControlTareas& tareas){

    string A_TAREA= "ANYADIR_TAREA";
    string OK = "OK";

    // Puerto donde escucha el proceso servidor

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
    int master_fd = chan.Accept();
    if (master_fd == -1) {
        string mensError(strerror(errno));
        cerr << "Error en el accept: " + mensError + "\n";
        // Cerramos el socket
        chan.Close(socket_fd);
        exit(1);
    }

    // Buffer para recibir el mensaje
    int rcv_bytes;   //num de bytes recibidos en un mensaje
    int send_bytes;  //num de bytes enviados en un mensaje

    bool out = false; // Inicialmente no salir del bucle
    while (!out) {
        // Recibimos el mensaje del cliente
        string buffer;
        rcv_bytes = chan.Recv(master_fd, buffer, MESSAGE_SIZE);
        if(rcv_bytes == -1) {
            string mensError(strerror(errno));
            cerr << "Error al recibir datos: " + mensError + "\n";
            // Cerramos los sockets
            chan.Close(master_fd);
            chan.Close(socket_fd);
        }

        string accion;
        string tweets;

        partesMensaje(buffer,accion,tweets);
        
        if(accion == A_TAREA){
            //Añadir una tarea a la cola
            tareas.publish(tweets);
            //tareas.Getqueues();
            send_bytes = chan.Send(master_fd, OK );
            if(send_bytes == -1) {
                cerr << "Error al enviar datos: " + string(strerror(errno)) + "\n";
                // Cerramos los sockets
                chan.Close(master_fd);
                exit(1);
            }

        }
    }

    // Cerramos el socket del cliente
    error_code = chan.Close(master_fd);
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

    // Despedida
    cout << "Bye bye Master" << endl;

}

void GestorCliente(int SERVER_PORT, ControlTags& tags){
    
    string S_DATOS= "SACAR_DATOS";
    string OK = "OK";
    string DATOS= "DATOS";
    // Puerto donde escucha el proceso servidor

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

        string accion;
        string tweets;

        partesMensaje(buffer,accion,tweets);

        string datos;
        
        if(accion == S_DATOS ){
            //Leer tags de la cola
            tags.read(datos);
            send_bytes = chan.Send(client_fd, DATOS + "," + datos );
            if(send_bytes == -1) {
                cerr << "Error al enviar datos: " + string(strerror(errno)) + "\n";
                // Cerramos los sockets
                chan.Close(client_fd);
                exit(1);
            }

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

    // Despedida
    cout << "Bye bye Master" << endl;

}

void GestorWorkers(Socket& soc, int worker_fd, ControlTareas& tareas, ControlTags& tags) {

    string PRO_TAREA= "PROCESAR_TAREA";
    string GUA_DATOS= "GUARDAR_DATOS";
    string OK = "OK";
    string TAREA= "TAREA";
    // Buffer para recibir el mensaje
    string buffer;
    int send_bytes;

    bool out = false; // Inicialmente no salir del bucle
    while(!out) {
        // Recibimos el mensaje del worker
        int rcv_bytes = soc.Recv(worker_fd,buffer,MESSAGE_SIZE);
        if (rcv_bytes == -1) {
            cerr << "Error al recibir datos: " + string(strerror(errno)) + "\n";
            // Cerramos los sockets
            soc.Close(worker_fd);
        }

        string accion;
        string datos;
        partesMensaje(buffer, accion, datos);

        if(accion == PRO_TAREA ){ 
            //cout<<accion + "\n";
            string tarea;
            tareas.read(tarea);
            //cout<<tarea+"\n";
            int send_bytes = soc.Send(worker_fd, TAREA + "," + tarea) ;
            if(send_bytes == -1) {
                cerr << "Error al recibir datos: " + string(strerror(errno)) + "\n";
                // Cerramos los sockets
                soc.Close(worker_fd);
                exit(1);
            }

        }
        if(accion == GUA_DATOS ){
            //cout<<accion + "\n";
            tags.publish(datos);
            //cout<<datos + "\n";
            int send_bytes = soc.Send(worker_fd, OK) ;
            if(send_bytes == -1) {
                cerr << "Error al recibir datos: " + string(strerror(errno)) + "\n";
                // Cerramos los sockets
                soc.Close(worker_fd);
                exit(1);
            }

        }
    }
    soc.Close(worker_fd);
}


void GestorWorker(int SERVER_PORT, ControlTareas& tareas, ControlTags& tags, int N){
    thread worker[N];
    int worker_fd[N];

    // Creación del socket con el que se llevará a cabo
    // la comunicación con el servidor.
    Socket chan(SERVER_PORT);

    // Bind
    int socket_fd = chan.Bind();
    if (socket_fd == -1) {
        cerr << "Error en el bind: " + string(strerror(errno)) + "\n";
        exit(1);
    }

    // Listen
    int max_connections = 2*N;
    int error_code = chan.Listen(max_connections);
    if (error_code == -1) {
        cerr << "Error en el listen: " + string(strerror(errno)) + "\n";
        // Cerramos el socket
        chan.Close(socket_fd);
        exit(1);
    }

    for (int i=0; i<N; i++) {
        // Accept
        worker_fd[i] = chan.Accept();

        if(worker_fd[i] == -1) {
            cerr << "Error en el accept: " + string(strerror(errno)) + "\n";
            // Cerramos el socket
            chan.Close(socket_fd);
            exit(1);
        }

        worker[i] = thread(&GestorWorkers, ref(chan), worker_fd[i], ref(tareas), ref(tags));
    }

    //¿Qué pasa si algún thread acaba inesperadamente?
    for (int i=0; i<N; i++) {
        worker[i].join();
    }

    // Cerramos el socket del servidor
    error_code = chan.Close(socket_fd);
    if (error_code == -1) {
        cerr << "Error cerrando el socket del servidor: " + string(strerror(errno)) + " aceptado" + "\n";
    }

    // Despedida
    cout << "Bye bye Workers" << endl;

}

int main(int argc, char* argv[]){
    const int N = 6; //Número de workers a atender
    const int N_COLA= 50;
    // Puerto donde escucha el proceso servidor
    int SERVER_PORT_1 = atoi(argv[1]); //puerto del servidor master
    int SERVER_PORT_2 = atoi(argv[2]); //puerto del servidor worker
    int SERVER_PORT_3 = atoi(argv[3]); //puerto del servidor cliente
    //int SERVER_PORT_1 = 31142; //puerto del servidor master
    //int SERVER_PORT_2 = 31143; //puerto del servidor worker
    //int SERVER_PORT_3 = 31144; //puerto del servidor cliente
    printf("Servidor iniciado en puertos: %d,%d,%d\n",SERVER_PORT_1,SERVER_PORT_2,SERVER_PORT_3);
    ControlTags tags(N_COLA);
    ControlTareas tareas(N_COLA);
    thread master;
    thread worker;
    thread cliente;
    master = thread(&GestorMaster, SERVER_PORT_1,ref(tareas));
    worker = thread(&GestorWorker, SERVER_PORT_2, ref(tareas), ref(tags), N);
    cliente = thread(&GestorCliente, SERVER_PORT_3, ref(tags));
    master.join();
    worker.join();
    cliente.join();

    return 0;
}

//-------------------------------------------------------------
