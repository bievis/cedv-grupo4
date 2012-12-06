#include <iostream>
#include "FicheroPalindromo.h"

using namespace std;

/*
Funcion main
*/
int main (int argc, char **argv) {
  if (argc == 2) {
    char* nombreArchivo = argv[1];
    FicheroPalindromo ficheroPalindromo;
    ficheroPalindromo.obtenerPalindromos(nombreArchivo);
  } else {
    cout << "Debe de introducir el nombre de un archivo" << endl;
  }
}
