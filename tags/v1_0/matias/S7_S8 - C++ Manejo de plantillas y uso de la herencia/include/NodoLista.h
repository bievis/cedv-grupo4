#ifndef NODO_LISTA_H
#define NODO_LISTA_H

template<class T>
class NodoLista {
 public:
  NodoLista () : _datos() {
    _siguiente = NULL;
    _anterior = NULL;
  }

  NodoLista (const T &datos) {
    _datos = datos;
    _siguiente = NULL;
    _anterior = NULL;
  }

  // Constructor de copia
  NodoLista (const NodoLista<T> &n);

  T & getDatos ();

  NodoLista<T> * getSiguiente () const;
  NodoLista<T> * getAnterior () const;
  void setSiguiente (NodoLista<T> *siguiente);
  void setAnterior (NodoLista<T> *anterior);

  // Sobre carga de Operadores
  NodoLista<T>& operator= (const NodoLista<T> &n);

  ~NodoLista ();
 private:
  T _datos;
  NodoLista<T> *_siguiente;
  NodoLista<T> *_anterior;
};


// Implementacion
  
  // Constructor de copia
  template<class T>
  NodoLista<T>::NodoLista (const NodoLista<T> &n) {
    _datos = n.getNombre();
    _siguiente = n.getSiguiente();
    _anterior = n.getAnterior();
  }
  
  template<class T>
  T& NodoLista<T>::getDatos () {
    return _datos;
  }
  
  template<class T>
  NodoLista<T> * NodoLista<T>::getSiguiente () const {
    return _siguiente;
  }

  template<class T>
  NodoLista<T> * NodoLista<T>::getAnterior () const {
    return _anterior;
  }

  template<class T>
  void NodoLista<T>::setSiguiente (NodoLista<T> *siguiente) {
    _siguiente = siguiente;
  }

  template<class T>
  void NodoLista<T>::setAnterior (NodoLista<T> *anterior) {
    _anterior = anterior;
  }

  // Sobre carga de Operadores
  template<class T>
  NodoLista<T>& NodoLista<T>::operator= (const NodoLista<T> &n) {
    _datos = n.getNombre();
    _siguiente = n.getSiguiente();
    _anterior = n.getAnterior();
  }

  template<class T>
  NodoLista<T>::~NodoLista () {
    _siguiente = NULL;
    _anterior = NULL;
  }

#endif
