
//*****************************************************************
// File:   Servidor.cpp
// Author: PSCD-Unizar
// Date:   noviembre 2015
// Coms:   Ejemplo de servidor con comunicación síncrona mediante sockets
//         Compilar el fichero "Makefile" asociado, mediante
//         "make".
//*****************************************************************

#include <iostream>
#include "GestorDeColas.hpp"

using namespace std;

const int MESSAGE_SIZE = 4001; //mensajes de no más 4000 caracteres
const int TAMANYO_MAX_COLA = 50; //tamaño maximo de cola

const int Cola_tareas = 1;
const int Cola_QOS = 2;
const int Cola_tags = 3;

static string colas[NUM_COLAS][TAMANYO_MAX_COLA];
static int front_colas[NUM_COLAS] = {-1,-1,-1};

static bool lleno(int cola){
	return front_colas[cola] == 49;
}

static bool vacio(int cola){
	return front_colas[cola] == -1;
}

static bool cola_tareas_anyadir(int cola, string mensaje){
	if (!lleno(cola)){
		front_colas[cola]++;
		colas[cola][front_colas[cola]] = mensaje;
		return true;
	}
	return false;
}

static bool leer(int cola, string& mensaje){
	if (vacio(cola)){ 
		return false;
	}
	mensaje = colas[cola][front_colas[cola]];
	return true;
}

static bool eliminar(int cola){
	if (vacio(cola)){ 
		return false;
	}
	front_colas[cola]--;
	return true;
}



//POST: Devuelve 3 tuplas <palabra clave, nombre_cola>
tupla getQueues(){
	struct tupla t = {{1,2,3}, {"Cola_tareas","Cola_QOS", "Cola_tags"}};
	return t;
}

//POST: Escribe mensaje en una cola existente
bool publish(int cola, string mensaje){
	return cola_tareas_anyadir(cola, mensaje);
}

//POST: Lee y elimina un mensaje de una cola
bool read(int cola, string& mensaje){
	if (leer(cola, mensaje)){
		eliminar(cola);
		return true;
	}
	return false;
}

