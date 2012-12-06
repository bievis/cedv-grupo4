#include <iostream>
#include <stdlib.h>
#include "Lista.h"
#include "Utils.h"
#include "Persona.h"
#include "Canal.h"
#include "Menus.h"

using namespace std;

/*
Funcion main
*/
int main (int argc, char **argv) {
  if (argc == 3) {
    Lista<Persona> listaPersona;
    listaPersona.insertarFinal (Persona("user", "user", USER));
    listaPersona.insertarFinal (Persona("admin", "admin", ADMIN));
    Lista<Canal> listaCanales;
    listaCanales.insertarFinal (Canal("Deportes"));
    listaCanales.insertarFinal (Canal("Actualidad"));
    string usuario = argv[1];
    
    Persona *pPersona = Utils::existePersona (argv[1], argv[2], listaPersona);

    if (pPersona == NULL) {
      cout << "El usuario o la clave introducidos no existen." << endl;
    } else if (pPersona->getTipo() == USER) {
      Menus::menuUser (*pPersona, listaPersona, listaCanales);
    } else if (pPersona->getTipo() == ADMIN) {
      Menus::menuAdministrador (*pPersona, listaPersona, listaCanales);
    }
  } else {
    cout << "Debe de introducir el nombre del usuario y la clave como parametros." << endl;
  }
}
