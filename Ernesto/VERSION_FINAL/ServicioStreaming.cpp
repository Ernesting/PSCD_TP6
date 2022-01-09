//BIBLIOTECAS
#include <fstream>
#include <istream>
#include <sstream> //Para separar las letras
#include <ostream>
#include <string>
#include <iostream>
#include "Socket.hpp"

using namespace std;

// Compilación: g++ -std=c++11 ServicioStreaming.cpp -o ServicioStreaming

//Miramos si es un tweet válido
bool tweetvalido(string& tweet){
    int puntoscomas = 0; //Contador de puntos y comas
    char c;
    string valido = "";
    stringstream input_stringstream(tweet);
    c = input_stringstream.get();
    for(int i = 0; i<tweet.length(); i++){
        //while(!input_stringstream.eof()){
        if(((c>='a')&&(c<='z'))||((c>='A')&&(c<='Z'))||((c>='0')&&(c<='9'))||(c=='.')||(c==',')||(c==';')||(c==':')||(c==' ')||(c=='_')||(c=='-')||(c=='#')||(c=='@')||(c=='/')){
            if(c==';'){
                puntoscomas++;
            }
            valido = valido + c;
        }
        c = input_stringstream.get();
    }
    if(puntoscomas>=3){
        tweet = valido;
        return true; //Dará que es mensaje válido
    }
    else{
        return false; //Dará que no es mensaje válido
    }
}


int main(){
    //tratartweets();
    //servidorStrming();
    string MENS_FIN = "FIN";
    string MENS_GET =  "getTweets()";
    int numTweets = 25;
    // Puerto donde escucha el proceso servidor
    int SERVER_PORT = 31141; //Podemos coger puertos entre [31141, 31150]

    // Creación del socket con el que se llevará a cabo
    // la comunicación con el servidor.
    Socket chan(SERVER_PORT);

    // Bind
    int socket_fd = chan.Bind();
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
    ifstream fOriginal;
    fOriginal.open("tweets-sinProcesar.csv");
    if(fOriginal.is_open()){

        // Buffer para recibir el mensaje
        int length = 100;
        string buffer;
        int rcv_bytes;   //num de bytes recibidos en un mensaje
        int send_bytes;  //num de bytes enviados en un mensaje
        string linea; //Línea del fichero de entrada
        int completo; //Indica el número de íneas aceptadas hasta el momento

        string basura;
        getline(fOriginal, basura); //La primera línea no sirve

        bool out = false; // Inicialmente no salir del bucle
        while (!out) {
            // Recibimos el mensaje del cliente
            rcv_bytes = chan.Recv(client_fd, buffer, length);
            if(rcv_bytes == -1) {
                string mensError(strerror(errno));
                cerr << "Error al recibir datos: " + mensError + "\n";
                // Cerramos los sockets
                chan.Close(client_fd);
                chan.Close(socket_fd);
            }

            //cout << "Mensaje recibido: '" + buffer + "'\n";

            // Si recibimos "FIN" --> Fin de la comunicación
            if (buffer == MENS_FIN) {
                out = true; // Salir del bucle
            } 
            else if(buffer == MENS_GET){
                string resp = "";
                completo = 0;
                while(completo < numTweets){
                    getline(fOriginal, linea);
                    if(tweetvalido(linea)){
                        resp = resp + linea + "\n";
                        completo++;
                    }
                }

                send_bytes = chan.Send(client_fd, resp);
                if(send_bytes == -1) {
                    string mensError(strerror(errno));
                    cerr << "Error al enviar datos: " + mensError + "\n";
                    // Cerramos los sockets
                    chan.Close(client_fd);
                    chan.Close(socket_fd);
                    exit(1);
                }
            }
        }
        fOriginal.close(); //Cerramos la lectur del fichero
    }
    else{
        cerr << "No se pude abrir el fichero entrada" << endl;
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

    return error_code;
    return 0;
}
