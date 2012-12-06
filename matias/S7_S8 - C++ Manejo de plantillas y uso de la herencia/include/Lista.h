#ifndef LISTA_H
#define LISTA_H

#include "NodoLista.h"

template<class T>
class Lista {
 public:
  Lista () { _cabecera = NULL; limpiar(); }

  Lista (const T &datos) {
    _cabecera = new NodoLista<T> (datos);
    limpiar();
  }

  // Constructor de copia
  Lista (const Lista<T> &l);
  
  // Limpia la lista
  void limpiar ();

  // Tamanio de la lista
  int size () const;
  
  // Inserta al final de la lista un elemento
  void insertarFinal (const T &datos);
  
  // Obtiene el primer elemento de la lista
  T *getInicio ();

  // Obtiene el elemento de la posicion N de la lista.
  T *getN (int n) const;
  
  // Obtiene el ultimo elemento de la lista
  T *getFin ();

  // Inserta en la posición N un elemento en la lista, sino lo inserta al final
  void insertarN (const T &datos, int n);
  
  // Eliminar el elemento de la posicion N la lista, si N es mayor que el tamanio de la lista elimina el último
  void eliminarN (int n);

  // Modifica el elemento de la posición N de la lista, si N es mayor que la longitud de la lista lo inserta al final
  void modificarN (const T &datos, int n);

  // Sobre carga de Operadores
  Lista<T>& operator= (const Lista<T> &l);

  ~Lista ();
 private:
  NodoLista<T> *_cabecera;
};

// Implementacion
  
  // Constructor de copia
  template<class T>
  Lista<T>::Lista (const Lista<T> &l) {
    int sz = l.size();
    limpiar();
    for (int i = 0; i < sz; i++) {
      insertarFinal(*(l.getN(i)));
    }
  }

  // Tamanio de la lista
  template<class T>
  int Lista<T>::size () const {
    int tamanio = 0;
    NodoLista<T> *pAux = _cabecera;
    
    if (pAux != NULL) {
      tamanio++;
      while (pAux->getSiguiente()) {
        pAux = pAux->getSiguiente();
        tamanio++;
      }
    }
    return tamanio;
  }
  
  // Inserta al final de la lista un elemento
  template<class T>
  void Lista<T>::insertarFinal (const T &datos) {
    NodoLista<T> *pAux = _cabecera;
    if (pAux == NULL) {
      _cabecera = new NodoLista<T> (datos);
    } else {
      while (pAux->getSiguiente()) {
        pAux = pAux->getSiguiente();
      }
      pAux->setSiguiente(new NodoLista<T> (datos));
      pAux->getSiguiente()->setAnterior(pAux);
    }
  }
  
  // Obtiene el primer elemento de la lista
  template<class T>
  T* Lista<T>::getInicio () {
    T *pInicio = NULL;

    if (_cabecera != NULL) {
      pInicio = &(_cabecera->getDatos());
    }

    return pInicio;
  }

  // Obtiene el elemento de la posicion N de la lista.
  template<class T>
  T* Lista<T>::getN (int n) const {
    T *pN = NULL;
    int iN = 0;
    NodoLista<T> *pAux = _cabecera;

    if (pAux != NULL) {
      while (pAux->getSiguiente() && iN != n) {
        pAux = pAux->getSiguiente();
        iN++;
      }
      pN = &(pAux->getDatos());
    }

    return pN;
  }
  
  // Obtiene el ultimo elemento de la lista
  template<class T>
  T* Lista<T>::getFin () {
    T *pFin = NULL;
    NodoLista<T> *pAux = _cabecera;

    if (pAux != NULL) {
      while (pAux->getSiguiente()) {
        pAux = pAux->getSiguiente();
      }
      pFin = &(pAux->getDatos());
    }

    return pFin;
  }

  // Inserta en la posición N un elemento en la lista, sino lo inserta al final
  template<class T>
  void Lista<T>::insertarN (const T &datos, int n) {
    NodoLista<T> *pAux = _cabecera;
    NodoLista<T> *pAux2 = NULL;
    int iN = 0;
    
    if (n >= 0) {
      if (pAux == NULL) {
        _cabecera = new NodoLista<T> (datos);
      } else {
        while (pAux->getSiguiente() && iN != n) {
          pAux = pAux->getSiguiente();
          iN++;
        }
        pAux2 = pAux;
        pAux = new NodoLista<T> (datos);
        pAux->setAnterior(pAux2->getAnterior());
        pAux->setSiguiente(pAux2);
      }
    }
  }

  // Eliminar el elemento de la posicion N la lista, si N es mayor que el tamanio de la lista elimina el último
  template<class T>
  void Lista<T>::eliminarN (int n) {    
    NodoLista<T> *pAux = _cabecera;
    int iN = 0;
    
    if (n >= 0 && pAux != NULL) {
      while (pAux->getSiguiente() && iN != n) {
        pAux = pAux->getSiguiente();
        iN++;
      }
      // Si es el primero limpiamos la cabecera
      if (pAux->getAnterior() == NULL) {
        _cabecera = NULL;
      }else {
        pAux->getAnterior()->setSiguiente(pAux->getSiguiente());
      }
      if (pAux->getSiguiente() != NULL) {
        pAux->getSiguiente()->setAnterior(pAux->getAnterior());
      }
      delete pAux;
    }
  }

  // Modifica el elemento de la posición N de la lista, si N es mayor que la longitud de la lista lo inserta al final
  template<class T>
  void Lista<T>::modificarN (const T &datos, int n) {
    NodoLista<T> *pAux = _cabecera;
    NodoLista<T> *pAux2 = NULL;
    int iN = 0;
    
    if (n >= 0) {
      if (pAux == NULL) {
        _cabecera = new NodoLista<T> (datos);
      } else {
        while (pAux->getSiguiente() && iN != n) {
          pAux = pAux->getSiguiente();
          iN++;
        }
        pAux2 = new NodoLista<T> (datos);
        // Si hemos llegado al ultimo elemento y hemos metido un N mayor que la longitud
        if (iN < n) {
          pAux->setSiguiente(pAux2);
        } else {          
          if (pAux->getAnterior() != NULL) {
            pAux->getAnterior()->setSiguiente(pAux2);
          }
          if (pAux->getSiguiente() != NULL) {
            pAux->getSiguiente()->setAnterior(pAux2);
          }
          delete pAux;
        }
      }
    }
  }

  // Sobre carga de Operadores
  template<class T>
  Lista<T>& Lista<T>::operator= (const Lista<T> &l) {
    int sz = l.size();
    limpiar();
    for (int i = 0; i < sz; i++) {
      this->insertarFinal(*(l.getN(i)));
    }
    return *this;
  }
  
  template<class T>
  Lista<T>::~Lista () {
    limpiar();
  }
  
  template<class T>
  void Lista<T>::limpiar () {
    NodoLista<T> *pAux = _cabecera;
    NodoLista<T> *pAux2 = NULL;
    
    // Limpiamos todos los elementos
    while (pAux != NULL) {
      pAux2 = pAux->getSiguiente();
      
      delete pAux;
      pAux = pAux2;
    }
    
    _cabecera = NULL; // Limpiamos la cabecera
  }

#endif
