#ifndef EXCEPCION_H
#define EXCEPCION_H

#include <iostream>

using namespace std;

class Excepcion {
 string _error;
 public:
  Excepcion (const string &error) : _error (error) {
    
  }

  const string& getError () const {
    return _error;
  }
};

#endif
