#ifndef __NODO_H__
#define __NODO_H__

template<class T>
class Nodo
{
  T _elem;
  Nodo<T> *_sig;
  Nodo<T> *_ant;

 public:
  inline Nodo<T>* getSig() const { return _sig; };
  inline Nodo<T>* getAnt() const { return _ant; };

  inline T& getDato() { return _elem; };
  inline void setDato ( const T& datoNuevo ) { _elem = datoNuevo; };

  inline void setSig ( Nodo<T> *nodo ) { _sig = nodo; };
  inline void setAnt ( Nodo<T> *nodo ) { _ant = nodo; };

  Nodo() : _sig(0), _ant(0) {};
  Nodo ( const T& elem );
  Nodo ( const Nodo<T>& nodoOrigen );
  virtual ~Nodo() {};

  Nodo<T>& operator= ( const Nodo<T>& nodoOrigen );

  void print();
};

template<class T>
Nodo<T>::Nodo ( const T& elem )
{
  _sig = _ant = 0;
  _elem = elem;
}

template<class T>
Nodo<T>::Nodo ( const Nodo<T>& nodoOrigen )
{
  _sig = nodoOrigen->getSig();
  _ant = nodoOrigen->getAnt();
  _elem = nodoOrigen->getDato();
}

template<class T>
Nodo<T>& Nodo<T>::operator= ( const Nodo<T>& nodoOrigen )
{
  _sig = nodoOrigen->getSig();
  _ant = nodoOrigen->getAnt();
  _elem = nodoOrigen->getDato();

  return *this;
}

template<class T>
void Nodo<T>::print()
{
  //Para Noticias
#ifdef __DEBUG__
  printf ( "[ANT->%p:SIG->%p] : ", _ant, _sig );
#endif
  _elem.print();
}

#endif
