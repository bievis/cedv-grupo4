#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <ctype.h>
#include "Persona.h"
#include "Lista.h"
#include "Noticia.h"
#include "Canal.h"
#include "Excepcion.h"

using namespace std;

// Clase de utilidades
class Utils {
public:
  // Lee un entero de teclado
  static void pause (string texto) {
    cout << texto << endl;
    cin.get();
  }

  // Lee un entero de teclado
  static int leerEntero (string texto) {
    string sNumero;
    bool esEntero = false;
    while (!esEntero) {
      cout << texto << endl;
      getline(cin, sNumero);
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

  // Lee una cadena de teclado
  static string leerCadena (string texto) {
    string sCadena;
    do {
      cout << texto << endl;
      getline(cin, sCadena);
    } while (sCadena.empty());
    return sCadena;
  }
  
  // Lee una persona del tipo usuario por teclado
  // Le ponemos el & para que no devuelve una copia sino el que acabamos de construir
  static Persona& leerPersonaUsuario (string texto) {
    cout << texto << endl;
    Persona *pPersona = new Persona(leerCadena ("    Nombre: "), 
                            leerCadena ("    Clave: "), 
                            USER);
    return *pPersona;
  }
  
  // Lee una persona del tipo administrador por teclado
  // Le ponemos el & para que no devuelve una copia sino el que acabamos de construir
  static Persona& leerPersonaAdmin (string texto) {
    cout << texto << endl;
    Persona *pPersona = new Persona(leerCadena ("    Nombre: "), 
                            leerCadena ("    Clave: "), 
                            ADMIN);
    return *pPersona;
  }
  
  // Muestra por pantalla la lista de personas que le pasemos
  static void mostrarListaPersonas (Lista<Persona> &lista) {
    int size = lista.size();
    Persona *pPersona = NULL;
    
    for (int i = 0; i < size; i++) {
      pPersona = lista.getN(i);
      if (pPersona != NULL) {
        cout << pPersona->toString() << endl;
      }
    } 
  }

  // Lee una noticia por teclado
  // Le ponemos el & para que no devuelve una copia sino el que acabamos de construir
  static Noticia& leerNoticia (string texto) {
    cout << texto << endl;
    Noticia *pNoticia = new Noticia(leerCadena ("    Titulo: "), 
                            leerCadena ("    Cuerpo: "));
    return *pNoticia;
  }

  // Lee un canal por teclado
  // Le ponemos el & para que no devuelve una copia sino el que acabamos de construir
  static Canal& leerCanal (string texto, Lista<Canal> &listaCanales) {
    cout << texto << endl;
    Canal *pCanal = new Canal(leerCadena ("    Nombre: "));
    return *pCanal;
  }

  // Lee un el nombre de un canal por teclado
  static string leerNombreCanal (string texto, Lista<Canal> &listaCanales) {
    string nombre = leerCadena (texto);
    Canal* pCanal = existeCanal (nombre, listaCanales);
    if (pCanal == NULL) {
      throw Excepcion("El canal introducido no existe.");
    }
    return nombre;
  }

  // Nos dice si existe un canal con ese nombre en la lista. Devuelve el Canal
  static Canal* existeCanal (string nombreCanal, Lista<Canal> &listaCanales) {
    int sizeCanales = listaCanales.size();
    Canal *pCanal = NULL;
    for (int i = 0; i < sizeCanales && pCanal == NULL; i++) {      
      pCanal = listaCanales.getN(i);      
      if (pCanal->getNombre() != nombreCanal) {
        pCanal = NULL;
      }
    }
    return pCanal;
  }

  // Nos dice si existe una Persona con ese nombre en la lista. Devuelve la Persona
  static Persona* existePersona (string nombre, string clave, Lista<Persona> &listaPersona) {
    int sizePersonas = listaPersona.size();
    Persona *pPersona = NULL;
    
    for (int i = 0; i < sizePersonas && pPersona == NULL; i++) {
      pPersona = listaPersona.getN(i);
      if (pPersona->getNombre() != nombre || pPersona->getClave() != clave) {
        pPersona = NULL;
      }
    }
    
    return pPersona;
  }

  // Elimina un canal de la lista
  static void eliminarCanal (string nombreCanal, Lista<Canal> &listaCanales) {
    bool eliminado = false;
    int sizeCanales = listaCanales.size();
  
    for (int i = 0; i < sizeCanales && !eliminado; i++) {
      if (listaCanales.getN(i)->getNombre() == nombreCanal) {
        listaCanales.eliminarN (i);
        eliminado = true;
      }
  }
}
};

#endif
