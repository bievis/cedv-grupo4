#ifndef __CANAL_H__
#define __CANAL_H__

#include "Noticia.h"
#include "Usuario.h"
#include "Lista.h"
#include <stdio.h>

class Canal {

 private:
  string _nombreCanal;
  Lista<Noticia> _noticias;
  Lista<Usuario> _usuarios;

  void Copia ( const Canal& origen );

 public:
  Canal();
  Canal ( const Canal& origen );
  virtual ~Canal();

  Canal& operator= ( const Canal& origen );

  inline string getNombreCanal() const { return _nombreCanal; };
  inline void setNombreCanal ( const string &nombreCanal ) { _nombreCanal = nombreCanal; };

  inline Lista<Noticia> *getListaNoticias() { return &_noticias; };
  inline Lista<Usuario> *getListaUsuarios() { return &_usuarios; };

  void inserta_noticia ( string noticia, bool interesante );
  void recibir_noticias ( string usuario );

  bool suscribir_canal ( string usuario );
  bool cancelar_suscripcion_canal ( string usuario );

  //  inline string getValorComparacion() const { return _nombreCanal; };

  void clear();

  void print();

  bool operator== ( const Canal& canal );
};

bool Canal::suscribir_canal ( string usuario )
{
  Usuario *u = 0;
  bool result = false;

  for ( unsigned int i = 1; i <= _usuarios.getNumElems(); i++ )
    {
      _usuarios.getRefElem ( u, i );
      if ( u->getNombre() == usuario )
	{
          result = true;
          break;
	}
    }

  if ( !result )
    {
      u = new Usuario;
      u->setNombre ( usuario );
      _usuarios.inserta_n ( *u, 1 );
      result = true;
      delete u;
    }

  return result;
}

bool Canal::cancelar_suscripcion_canal ( string usuario )
{
  Usuario *u = 0;
  bool result = false;
  Usuario borrado;

  for ( unsigned int i = 1; i <= _usuarios.getNumElems(); i++ )
    {
      _usuarios.getRefElem ( u, i );
      if ( u->getNombre() == usuario )
	{

          _usuarios.borra_n ( borrado, i );
          result = true;
          break;
	}
    }

  return result;
}

Canal::~Canal()
{
#ifdef __DEBUG__
  cout << __FUNCTION__ << " Se destruye el canal " << _nombreCanal;
  printf ( "[%p]\n", this );
#endif
  _noticias.clear();
  _usuarios.clear();
}

void Canal::clear()
{
  _nombreCanal = "";
  _noticias.clear();
  _usuarios.clear();
}

void Canal::inserta_noticia ( string noticia, bool interesante )
{
  Noticia nuevaNoticia ( noticia, interesante );
  _noticias.inserta_n ( nuevaNoticia, 1 );
}

void Canal::recibir_noticias ( string usuario )
{
  _noticias.print();
}

Canal::Canal()
{
  clear();
}

void Canal::Copia ( const Canal& origen )
{
  Noticia *n = 0;
  Usuario *usr = 0;
  unsigned int i = 0;
  _nombreCanal = origen.getNombreCanal();

  _noticias.clear();

  for ( i = const_cast<Canal&>(origen).getListaNoticias()->getNumElems(); i > 0; i-- )
    {
      if (  const_cast<Canal&>(origen).getListaNoticias()->getRefElem ( n, i ) )
        _noticias.inserta_n ( *n, 1 );
    }

  for ( i = const_cast<Canal&>(origen).getListaUsuarios()->getNumElems(); i > 0; i-- )
    {
      if (  const_cast<Canal&>(origen).getListaUsuarios()->getRefElem ( usr, i ) )
        _usuarios.inserta_n ( *usr, 1 );
    }
}

Canal::Canal ( const Canal& origen )
{
  Copia ( origen );
}

Canal& Canal::operator= ( const Canal& origen )
{
  Copia ( origen );
  return *this;
}

void Canal::print()
{
  cout << "Nombre del Canal : " << _nombreCanal << endl;
}

bool Canal::operator==( const Canal& canal )
{
  bool result = false;

  if ( _nombreCanal == canal.getNombreCanal() )
    result = true;

  return result;
}

#endif
