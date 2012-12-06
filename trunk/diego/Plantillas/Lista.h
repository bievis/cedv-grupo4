#ifndef __LISTA_H__
#define __LISTA_H__

#include <new>
#include <iostream>
#include "Nodo.h"
#include "ListaExcepcion.h"

using namespace std;

template<class T>
class Lista {
private:
  Nodo<T>* _primero;
  Nodo<T>* _ultimo;
  unsigned int _numElems;

public:
  Lista() : _primero(0), _ultimo(0), _numElems(0) {}
  virtual ~Lista();

  inline unsigned int getNumElems() const { return _numElems; };
  inline Nodo<T>* getPrimero() const { return _primero; };
  inline Nodo<T>* getUltimo() const { return _ultimo; };

  bool inserta_n ( const T& elemNuevo, int pos );
  void modifica_n ( const T& elemModif, int pos );
  void borra_n ( T& elemBorrado, int pos );

  bool getRefElem ( T* &elem, unsigned int pos );
 
  void clear();

  void print();

  unsigned int find ( const T& elem );

};

template<class T>
bool Lista<T>::getRefElem ( T* &elem, unsigned int pos )
{
  Nodo<T> *ptrPos = _primero;
  bool result = false;

  while ( ( 0 != ptrPos->getSig() ) && ( --pos > 0 ) )
	{
	  ptrPos = ptrPos->getSig();
	}

  if ( ptrPos )
    result = true;

  elem = &(ptrPos->getDato());

  //  printf ( "elem = %p\n", elem );

  return result;
}

template<class T>
unsigned int Lista<T>::find ( const T& elem )
{
  Nodo<T> *ptr = _primero;
  unsigned int i = 0;
  unsigned int pos = 0;

  while ( ptr != 0 )
    {
      i++;
      if ( ptr->getDato() == elem )
	{
          pos = i;
          break;
	}
      ptr = ptr->getSig();
    }

  if ( pos == 0 )
    throw ListaExcepcion ( 1 );

  return pos;
}

template<class T>
bool Lista<T>::inserta_n ( const T& elemNuevo, int pos )
{
  Nodo<T>* ptrNew = 0;
  Nodo<T>* ptrPos = 0;

  try
    {
      ptrNew = new Nodo<T> ( elemNuevo );
      //      printf ( "crea nuevo nodo %p\n", ptrNew );
    }
  catch ( bad_alloc &e )
    {
      cerr << "Error at memory allocation for the new element" << endl;
      return false;
    }

  if ( 0 == _primero )
    {
      _primero = _ultimo = ptrNew;
    }
  else
    {
      ptrPos = _primero;

      while ( ( 0 != ptrPos->getSig() ) && ( --pos > 0 ) )
	{
	  ptrPos = ptrPos->getSig();
	}

      ptrNew->setAnt ( ptrPos->getAnt() );
      ptrNew->setSig ( ptrPos );

#ifdef __DEBUG__
      cout << "Datos ptrNew" << endl;
      ptrNew->print();
      cout << endl;

      cout << "Datos ptrPos" << endl;
      ptrPos->print();
      cout << endl;
#endif

      if ( ptrPos->getAnt() )
        ptrPos->getAnt()->setSig ( ptrNew );

      ptrPos->setAnt ( ptrNew );

      if ( ptrPos == _primero )
        _primero = ptrNew;
	
    }

  _numElems++;

  return true;
}

template<class T>
Lista<T>::~Lista() {
  clear();
}

template<class T>
void Lista<T>::clear() {
  Nodo<T> *ptr = 0;

  while ( _primero  )
    {
      ptr = _primero;
      _primero = _primero->getSig();

#ifdef __DEBUG__
      printf ( "Destruyendo dato de la posicion %p ... ", ptr );
#endif

      delete ptr;

#ifdef __DEBUG__
      cout << "Ok" << endl;
#endif
    }

  _ultimo = 0;

  _numElems = 0;
}

template<class T>
void Lista<T>::modifica_n ( const T& elemModif, int pos )

{
  Nodo<T>* ptrPos = 0;

  ptrPos = _primero;

  while ( ( 0 != ptrPos->getSig() ) && ( --pos > 0 ) )
    {
      ptrPos = ptrPos->getSig();
    }

  ptrPos->setDato ( elemModif );
}

template<class T>
void Lista<T>::borra_n ( T& elemBorrado, int pos )
{
  Nodo<T>* ptrPos = 0;

  if ( 1 == _numElems )
    {
      elemBorrado = _primero->getDato();
      ptrPos = _primero;
      _primero = _ultimo = 0;      
    }
  else
    {
      ptrPos = _primero;

      while ( ( 0 != ptrPos->getSig() ) && ( --pos > 0 ) )
        {
          ptrPos = ptrPos->getSig();
        }

     elemBorrado = ptrPos->getDato();

     if ( ( ptrPos != _primero ) && ( ptrPos != _ultimo ) )
       {
         ptrPos->getAnt()->setSig ( ptrPos->getSig() );
         ptrPos->getSig()->setAnt ( ptrPos->getAnt() );         
       }

     if ( ptrPos == _primero )
       {
         ptrPos->getSig()->setAnt ( ptrPos->getAnt() );
         _primero = _primero->getSig();
       }

     if ( ptrPos == _ultimo )
       {
         ptrPos->getAnt()->setSig ( ptrPos->getSig() );
         _ultimo = _ultimo->getAnt(); 
       }

   }

   delete ptrPos;

   _numElems--;

}

template<class T>
void Lista<T>::print ()
{
  Nodo<T> *ptr = _primero;

  cout << "=========================" << endl;
  cout << "Lista (Num. Elems = " << _numElems << ")" << endl;
#ifdef __DEBUG__
  printf ( "Primero->%p : Ultimo->%p\n", _primero, _ultimo );
#endif
  cout << "=========================" << endl;

  if ( _numElems == 0 )
    cout << "vacia" << endl;

  for ( unsigned int i = 1; i <= _numElems; i++ ) 
    {
#ifdef __DEBUG__
      printf( "Elemento %d [%p] : ", i, ptr );
#else
      cout << "Elemento " << i << " : ";
#endif
      ptr->print();
      if ( ptr->getSig() )
        ptr = ptr->getSig();
    }

  cout << "=========================" << endl;
}

#endif
