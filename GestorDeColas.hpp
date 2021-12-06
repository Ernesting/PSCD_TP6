#ifndef GESTOR_DE_COLAS.HPP
#define GESTOR_DE_COLAS.HPP

 void getQueues();
bool publish(int cola, string mensaje);
bool read(int cola, string& mensaje);
#endif
