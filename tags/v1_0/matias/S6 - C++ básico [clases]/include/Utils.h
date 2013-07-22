#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <ctype.h>
#include "Triangle.h"

using namespace std;

// Clase de utilidades
class Utils {
public:
  // Lee un entero de teclado
  static int leerEntero (string texto) {
    string sNumero;
    bool esEntero = false;
    while (!esEntero) {
      cout << texto << endl;
      cin >> sNumero;
      esEntero = isEntero(sNumero);
    }
    return atoi(sNumero.c_str());
  }
  
  // Indica si la cadena contiene un numero entero
  static bool isEntero (string cadena) {
    bool es = (cadena.size() > 0);
  
    for (int i = 0; cadena[i] && es; i++) {
      es = isdigit(cadena[i]);
    } 
    
    return es;
  }
  
  // Lee un punto por teclado
  // Le ponemos el & para que no devuelve una copia sino el que acabamos de construir
  static Vec3& leerPunto (string texto) {
    cout << texto << endl;
    Vec3 *pPunto = new Vec3(leerEntero ("    Inserte X: "), 
                            leerEntero ("    Inserte Y: "), 
                            leerEntero ("    Inserte Z: "));
    return *pPunto;
  }
  
  // Lee un triangulo por teclado
  // Le ponemos el & para que no devuelve una copia sino el que acabamos de construir
  static Triangle& leerTriangulo (string texto) {
    cout << texto << endl;
    Triangle *pTriangulo = new Triangle(leerPunto ("  Inserte punto A: "), 
                                        leerPunto ("  Inserte punto B: "), 
                                        leerPunto ("  Inserte punto C: "));
    return *pTriangulo;
  }
};

#endif
