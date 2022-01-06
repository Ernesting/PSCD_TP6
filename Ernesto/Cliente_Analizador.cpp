#include <iostream>
#include <chrono>
#include <thread>
#include "Socket.hpp"
#include <map>
#include <string>

using namespace std;
// Compilación: g++ -std=c++11 Cliente.cpp -o Cliente

const int MESSAGE_SIZE = 10000; //mensajes de no más 10000 caracteres

int main(int argc, char* argv[]) {
    
    map<string,int> frequency;
    
    string S_DATOS= "SACAR_DATOS";
    string SHOW ="MOSTRAR";
    //string OK = "OK";
    string DATOS= "DATOS,";
    const string MENS_FIN = "FIN";

    // Dirección y número donde escucha el proceso servidor
    string SERVER_ADDRESS = argv[1];
    int SERVER_PORT = atoi(argv[2]);

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
        cout << "Escriba 'SACAR_DATOS' para analizar tweets o 'MOSTRAR' para obtener por pantalla los resultados: ";
        // "SACAR_DATOS"
        getline(cin, mensaje);
        //mensaje = S_DATOS;
        if (mensaje == S_DATOS){
          // Enviamos el mensaje
          send_bytes = chan.Send(socket_fd, mensaje);
  
          if(send_bytes == -1) {
              cerr << "Error al enviar datos: " << strerror(errno) << endl;
              // Cerramos el socket
              chan.Close(socket_fd);
              exit(1);
          }
  
          
          // Buffer para almacenar la respuesta
          string buffer;

          // Recibimos la respuesta del servidor
          read_bytes = chan.Recv(socket_fd, buffer, MESSAGE_SIZE);
          buffer.erase(0,6);
          frequency[buffer] ++;
          cout << "buffer es: "<<buffer << endl; //Así se salta "DATOS,"
          cout << "y su frecuencia es: " << frequency[buffer] <<endl;
          cout << "---------------------\n\n---------------------\n";
        }else if (mensaje == SHOW){
          map<string, int>::iterator it;
          for(it=frequency.begin(); it!=frequency.end(); ++it){
            cout << it->first << " => " << it->second << '\n';
          }
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
