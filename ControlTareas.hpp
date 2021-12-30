#include <mutex>
#include <condition_variable>
#include "BoundedQueue.hpp"
#include <string>
using namespace std;


class ControlTareas{
public:

    ControlTareas(const int n); 
    void Getqueues();
    void publish(string mensaje);
    void read(string& mensaje);

private:

    BoundedQueue<string> Tareas(N);
    mutex mtxMonitor;
    condition_variable esperandolibre;
    condition_variable esperandovacia;
};
