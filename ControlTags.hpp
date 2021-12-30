#include <mutex>
#include <condition_variable>
#include "BoundedQueue.hpp"
#include <string>
using namespace std;



class ControlTags{
public:

    ControlTags(const int n); 
    void Getqueues();
    void publish(string mensaje);
    void read(string& mensaje);

private:
    int TAMANYO;
  
    mutex mtxMonitor;
    condition_variable esperandolibre;
    condition_variable esperandovacia;
};
