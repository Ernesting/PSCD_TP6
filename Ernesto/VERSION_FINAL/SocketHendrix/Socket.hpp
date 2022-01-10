//*****************************************************************
// File:   Socket.hpp
// Author: PSCD-Unizar
// Date:   noviembre 2015
// Coms:   Especificaci�n de una librer�a gen�rica para 
//         comunicaci�n s�ncrona utilizando sockets
//*****************************************************************

#ifndef _SOCKET_HPP_
#define _SOCKET_HPP_

#include <cstring>
#include <strings.h>
#include <iostream>
#include <unistd.h>

using namespace std;

class Socket {
private:
	// ATRIBUTOS
	// Para almacenar la direcci�n y n�mero de puerto
	// donde escucha el proceso servidor
	string SERVER_ADDRESS;
	int SERVER_PORT;

	// Descriptor de fichero asociado al socket;
	int socket_fd;

public:
	// CONSTRUCTORES
	// Constructor de la clase indicando la direcci�n
	// y el puerto de escucha del servidor. Para clientes.
	Socket(string address, int port);


	// Constructor de la clase indicando  el puerto de 
	// escucha del servidor. Para servidores.
	Socket(int port);

	// M�TODOS

	// Acepta la conexi�n de un nuevo cliente.
	// PRE:
	// POST: Abre una nueva conexi�n con un cliente.
	// Devuelve el descriptor de fichero del socket asociado
	// al nuevo cliente en caso de exito.
	// Devuelve -1 en caso de error.
	int Accept();

	// Binding de la nueva conexi�n.
	// PRE: 
	// POST: Asocia la nueva conexi�n con el programa actual.
	// Devuelve el descriptor de fichero del socket asociado
	// a la conexi�n en caso de �xito. 
	// Devuelve -1 en caso de error.
	int Bind();

	// Cierre de conexi�n.
	// PRE: 'fd' indica un descriptor de fichero asociado
	//      a un socket con una conexi�n abierta.
	// POST: Cierra la conexi�n asociada al descriptor de
	//       fichero indicado.
	// Devuelve 0 en caso de �xito.
	// Devuelve -1 en caso de error. 
	int Close(int fd);

	// Conexi�n con el servidor.
	// PRE: 
	// POST: La funci�n conecta con el servidor utilizando
	//       el socket especificado. 
	// En caso de �xito el m�todo devuelve el descriptor
	// de fichero del socket 
	// a utilizar para enviar y recibir mensages.
	// Devuelve -1 en caso de error. 
	int Connect();

	// Llamada a listen.
	// PRE: 'max_conexiones_espera' > 0 (n�mero de conexiones en
	//      que se mantienen. )
	// POST: El servidor comienza a escuchar en el
	//       puerto indicado.
	// Devuelve 0 en caso de �xito.
	// Devuelve -1 en caso de error.
	int Listen(int max_conexiones_espera);

	// Recepci�n de un nuevo mensaje a trav�s del 
	// socket abierto.
	// PRE: 'fd' indica un descriptor de fichero asociado
	//       a un socket con una conexi�n abierta.
	//       buffer: Puntero al buffer donde se almacenaran 
	//       los datos le�dos.
	//       buffer_length indica el tama�o del buffer.
	// POST: Recibe un nuevo mensaje que es almacenado 
	//       en la variable "buffer".
	// Devuelve el numero de bytes le�dos en caso de �xito.
	// Devuelve -1 en caso de error.
	int Recv(int fd, char* buffer, int buffer_length);
	int Recv(int fd, string &buffer, int buffer_length);

	// Env�a informaci�n a trav�s del socket abierto
	// PRE: 'fd' indica un descriptor de fichero asociado 
	//       a un socket con una conexi�n abierta.
	//       "message" ha de ser NO VAC�O
	// POST: Env�a el mensaje establecido a trav�s del 
	//       socket indicado
	// Devuelve el n�mero de bytes enviados en caso de �xito.
	// Devuelve -1 en caso de error.
	ssize_t Send(int fd, const char* message);
	ssize_t Send(int fd, const string message); 
};

#endif