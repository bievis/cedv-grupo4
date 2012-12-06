#ifndef UTILS_H
#define UTILS_H

#include <iostream>

using namespace std;

// Clase de utilidades
class Utils {
public:
  /*
  Indica si el caracter es un salto de linea o nos
  */
  static bool isSaltoLinea (char caracter) {
    return (caracter == '\n');
  }
  
  /*
  Vacia una palabra
  */
  static void vaciarPalabra (char *palabra) {
    for (int i = 0;palabra[i]; i++) {
      palabra[i] = '\0';
    } 
  }

  /*
  Indica si la palabra es un palindromo
  */
  static bool isPalindromo (string palabra) {
    int j = palabra.size() - 1;
    bool palindromo = (j > 0);
    for (int i = 0;i < j && palindromo; i++, j--) {
      palindromo = (palabra[i] == palabra[j]);
    }
    return palindromo;
  }
};

#endif
