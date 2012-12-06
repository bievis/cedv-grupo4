#include <iostream>
#include <stdlib.h>
#include "Triangle.h"
#include "Utils.h"

using namespace std;

void menuPrincipal ();

/*
Funcion main
*/
int main (int argc, char **argv) {
  menuPrincipal ();
}

void menuPrincipal () {
  bool salir = false;
  int opcion;
  Triangle triangulo;
  
  while (!salir) {
    cout << "########### Menu de Triangulos #########" << endl << endl;
    cout << "1 - Insertar datos de un Triangulo." << endl;
    cout << "2 - Salir." << endl << endl;
    opcion = Utils::leerEntero("Elija una opción: ");
    
    switch(opcion) {
      case 1: 
        triangulo = Utils::leerTriangulo ("Inserte los datos del teclado.");
        cout << endl << "El triangulo que ha insertado es: " << triangulo.toString() << endl << endl << endl;
        break;
      case 2:
        salir = true;
        break;
      default: ;
    }; 
  }
}
