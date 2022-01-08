#include <iostream>
#include <chrono>
#include <thread>
#include "Socket.hpp"

using namespace std;
// Compilación: g++ -std=c++11 Cliente.cpp -o Cliente

const int MESSAGE_SIZE = 10000; //mensajes de no más 10000 caracteres

int main() {
    string S_DATOS= "SACAR_DATOS";
    string OK = "OK";
    string DATOS= "DATOS";
    const string MENS_FIN = "FIN";

    // Dirección y número donde escucha el proceso servidor
    string SERVER_ADDRESS = "hendrix02";
    int SERVER_PORT = 31144;

    // Creación del socket con el que se llevará a cabo
    // la comunicación con el servidor.
    Socket chan(SERVER_ADDRESS, SERVER_PORT);

    // Conectamos con el servidor. Probamos varias conexiones
    const int MAX_ATTEMPS = 10;
    int count = 0;
    int socket_fd;
    do {
        // Conexión con el servidor
        socket_fd = chan.Connect();
        count++;

        // Si error --> esperamos 1 segundo para reconectar
        if(socket_fd == -1) {
            this_thread::sleep_for(chrono::seconds(1));
        }
    } while(socket_fd == -1 && count < MAX_ATTEMPS);

    // Chequeamos si se ha realizado la conexión
    if(socket_fd == -1) {
        return socket_fd;
    }

    string mensaje;
    int read_bytes;   //num de bytes recibidos en un mensaje
    int send_bytes;  //num de bytes enviados en un mensaje

    do {
        // Leer mensaje de la entrada estandar
        //cout << "Frase para pedir tweets: ";
        // "SACAR_DATOS"
        //getline(cin, mensaje);
        mensaje = S_DATOS;
        // Enviamos el mensaje
        send_bytes = chan.Send(socket_fd, mensaje);

        if(send_bytes == -1) {
            cerr << "Error al enviar datos: " << strerror(errno) << endl;
            // Cerramos el socket
            chan.Close(socket_fd);
            exit(1);
        }

        if(mensaje != MENS_FIN){
            // Buffer para almacenar la respuesta
            string buffer;

            // Recibimos la respuesta del servidor
            read_bytes = chan.Recv(socket_fd, buffer, MESSAGE_SIZE);
            cout << &(buffer[6]) << endl; //Así se salta "DATOS,"
            cout << "---------------------\n\n---------------------\n";
        }

    } while(mensaje != MENS_FIN);

    // Cerramos el socket
    int error_code = chan.Close(socket_fd);
    if(error_code == -1) {
        cerr << "Error cerrando el socket: " << strerror(errno) << endl;
    }

    // Despedida
    cout << "Bye bye" << endl;

    return error_code;
}
