#include <MonitorCliente.hpp>
//#include <iostream>
//#include <string>

MonitorCliente::MonitorCliente(string _fichero){
    this->fichero = _fichero;
    //nLec = 0;
    //nEsc = 0;
}

    //--------------------------- destructor
MonitorCliente::~MonitorCliente(){}
//--------------------------- usuario

void appendWord(const string _palabra,const string fichero){
    ofstream fileOUT (fichero, ios::app);
  
    fileOUT << _palabra << endl;
  
    fileOUT.close();
}

void MonitorCliente::append(string _palabra){
    unique_lock<mutex> lck(mtx);
    
    /*while (nLec > 0 || nEsc > 0){
      okEscribir.wait(lck);
    }
    nEsc ++;*/
    
    appendWord(_palabra, fichero);
    
    /*nEsc --;
    okEscribir.notify_one();
    okLeer.notify_all();*/
    
}

void mostrarFrecuencias(string fichero){
  map<string, int> frequency;
  
  std::ifstream infile(fichero);
  std::string palabra;
  while (std::getline(infile, palabra)){
    frequency[palabra]++;
  }
  
  map<string, int>::iterator it;
    for(it=frequency.begin(); it!=frequency.end(); ++it){
      cout << it->first << " => " << it->second << '\n';
    }
  
}

void MonitorCliente::show(){
  unique_lock<mutex> lck(mtx);
 
   /*while (nEsc > 0){
      okLeer.wait(lck);
    }
    nLec ++;*/
    
   mostrarFrecuencias(fichero);
   
   /*nLec --;
   if (nLec == 0){
     okEscribir.notify_one();
   }*/
  
}
    
