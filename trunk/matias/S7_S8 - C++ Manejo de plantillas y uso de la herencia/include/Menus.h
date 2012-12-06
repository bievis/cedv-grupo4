#ifndef MENUS_H
#define MENUS_H

#include <iostream>
#include <stdlib.h>
#include "Lista.h"
#include "Utils.h"
#include "Persona.h"
#include "Canal.h"

using namespace std;

// Clase para gestionar los menus
class Menus {
public:

// Menú del ADMIN
static void menuAdministrador (Persona& persona, Lista<Persona> &listaPersona, Lista<Canal> &listaCanales) {
  bool salir = false;
  int opcion;
  Canal *pCanal;
  
  while (!salir) {
    cout << "########### Menu del Administrador #########" << endl << endl;
    cout << "1 - Insertar usuario." << endl;
    cout << "2 - Insertar administrador." << endl;
    cout << "3 - Ver personas insertadas." << endl;
    cout << "4 - Insertar un canal de noticias." << endl;
    cout << "5 - Eliminar un canal de noticias." << endl;
    cout << "6 - Ver ultimas noticias de los canales subcritos." << endl;
    cout << "7 - Insertar una noticia." << endl;
    cout << "8 - Suscribirse en canal de noticias." << endl;
    cout << "9 - Baja en canal de noticias." << endl;
    cout << "10 - Obtener Top 5 de un canal de noticias." << endl;
    cout << "11 - Salir." << endl << endl;
    opcion = Utils::leerEntero("Elija una opción: ");
    try {
      switch(opcion) {
        case 1:
          // Insertar usuario.
          listaPersona.insertarFinal (Utils::leerPersonaUsuario ("Inserte los datos de un usuario."));
          Utils::pause ("Se ha insertado el usuario correctamente....");
          break;
        case 2:
          // Insertar administrador.
          listaPersona.insertarFinal (Utils::leerPersonaAdmin ("Inserte los datos de un administrador."));
          Utils::pause ("Se ha insertado el administrador correctamente....");
          break;
        case 3:
          // Ver personas insertadas.
          Utils::mostrarListaPersonas (listaPersona);
          Utils::pause ("Pulse una INTRO para continuar....");
          break;
        case 4:
          // Insertar un canal de noticias.
          listaCanales.insertarFinal (Utils::leerCanal ("Inserte los datos del nuevo canal.", listaCanales));
          Utils::pause ("Se ha insertado el canal correctamente....");
          break;
        case 5:
          // Eliminar un canal de noticias.
          Utils::eliminarCanal (Utils::leerNombreCanal ("Inserte el nombre del canal que desea eliminar", listaCanales), listaCanales);
          Utils::pause ("Se ha eliminado el canal correctamente....");
          break;
        case 6: 
          // Ver ultimas noticias de los canales subcritos.
          persona.ultimasNoticias (listaCanales);
          Utils::pause ("Pulse una INTRO para continuar....");
          break;
        case 7: 
          // Insertar una noticia.
          pCanal = Utils::existeCanal (Utils::leerNombreCanal ("Nombre del canal donde desea inserta la noticia.", listaCanales), listaCanales);
          if (pCanal != NULL) {
            pCanal->insertarNoticia(Utils::leerNoticia("Inserte los datos de la nueva noticia: "));
            pCanal = NULL;
            Utils::pause ("Se ha insertado la noticia en el canal correctamente....");
          }
          break;
        case 8: 
          // Suscribirse en canal de noticias.
          pCanal = Utils::existeCanal (Utils::leerNombreCanal ("Nombre del canal al que desea subcribirse.", listaCanales), listaCanales);
          if (pCanal != NULL) {
            persona.altaCanal (pCanal->getNombre());
            pCanal = NULL;
            Utils::pause ("Se ha realizado la suscripcion al canal correctamente....");
          }
          break;
        case 9: 
          // Baja en canal de noticias.
          pCanal = Utils::existeCanal (Utils::leerNombreCanal ("Nombre del canal al que desea darse de baja.", listaCanales), listaCanales);
          if (pCanal != NULL) {
            persona.bajaCanal (pCanal->getNombre());
            pCanal = NULL;
            Utils::pause ("Se ha realizado la baja al canal correctamente....");
          }
          break;
        case 10: 
          // Obtener Top 5 de un canal de noticias.
          pCanal = Utils::existeCanal (Utils::leerNombreCanal ("Nombre del que quiere obtener Top 5 de un canal de noticias: ", listaCanales), listaCanales);
          if (pCanal != NULL) {
            cout << pCanal->toStringTop5 () << endl;
            pCanal = NULL;
            Utils::pause ("Pulse una INTRO para continuar....");
          }
          break;
        case 11:
          salir = true;
          break;
        default: ;
      };
    } catch (Excepcion &ex) {
      cout << "ERROR: " << ex.getError() << endl;
      Utils::pause ("Pulse una INTRO para continuar....");
    } catch (...) {
      cout << "ERROR: Se ha producido una excepcion desconocida al realidar la operacion." << endl;
      Utils::pause ("Pulse una INTRO para continuar....");
    }
  }
}

// Menú del USER
static void menuUser (Persona& persona, Lista<Persona> &listaPersona, Lista<Canal> &listaCanales) {
  bool salir = false;
  int opcion;
  Canal *pCanal;
  
  while (!salir) {
    cout << "########### Menu del Usuario #########" << endl << endl;
    cout << "1 - Ver ultimas noticias de los canales subcritos." << endl;
    cout << "2 - Insertar una noticia." << endl;
    cout << "3 - Suscribirse en canal de noticias." << endl;
    cout << "4 - Baja en canal de noticias." << endl;
    cout << "5 - Obtener Top 5 de un canal de noticias." << endl;
    cout << "6 - Salir." << endl << endl;
    opcion = Utils::leerEntero("Elija una opción: ");
    try {
      switch(opcion) {
        case 1: 
          // Ver ultimas noticias de los canales subcritos.
          persona.ultimasNoticias (listaCanales);
          Utils::pause ("Pulse una INTRO para continuar....");
          break;
        case 2: 
          // Insertar una noticia.
          pCanal = Utils::existeCanal (Utils::leerNombreCanal ("Nombre del canal donde desea inserta la noticia.", listaCanales), listaCanales);
          if (pCanal != NULL) {
            pCanal->insertarNoticia(Utils::leerNoticia("Inserte los datos de la nueva noticia: "));
            pCanal = NULL;
            Utils::pause ("Se ha insertado la noticia en el canal correctamente....");
          }
          break;
        case 3: 
          // Suscribirse en canal de noticias.
          pCanal = Utils::existeCanal (Utils::leerNombreCanal ("Nombre del canal al que desea subcribirse.", listaCanales), listaCanales);
          if (pCanal != NULL) {
            persona.altaCanal (pCanal->getNombre());
            pCanal = NULL;
            Utils::pause ("Se ha realizado la suscripcion al canal correctamente....");
          }
          break;
        case 4: 
          // Baja en canal de noticias.
          pCanal = Utils::existeCanal (Utils::leerNombreCanal ("Nombre del canal al que desea darse de baja.", listaCanales), listaCanales);
          if (pCanal != NULL) {
            persona.bajaCanal (pCanal->getNombre());
            pCanal = NULL;
            Utils::pause ("Se ha realizado la baja al canal correctamente....");
          }
          break;
        case 5: 
          // Obtener Top 5 de un canal de noticias.
          pCanal = Utils::existeCanal (Utils::leerNombreCanal ("Nombre del que quiere obtener Top 5 de un canal de noticias: ", listaCanales), listaCanales);
          if (pCanal != NULL) {
            cout << pCanal->toStringTop5 () << endl;
            pCanal = NULL;
            Utils::pause ("Pulse una INTRO para continuar....");
          }
          break;
        case 6:
          salir = true;
          break;
        default: ;
      };
    } catch (Excepcion &ex) {
      cout << "ERROR: " << ex.getError() << endl;
      Utils::pause ("Pulse una INTRO para continuar....");
    } catch (...) {
      cout << "ERROR: Se ha producido una excepcion desconocida al realidar la operacion." << endl;
      Utils::pause ("Pulse una INTRO para continuar....");
    }
  }
}

};

#endif
