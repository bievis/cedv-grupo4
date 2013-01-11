#include "Planeta.h"

void Planeta::Copia ( const Planeta& origen )
{
  Astro::Copia ( origen );  

  _traslacion = origen.getTraslacion ( 1 );
  _rotInterna = origen.getRotacion();

  list<Satelite>::const_iterator it;

  satelites.clear();

  for ( it = origen.satelites.begin(); it != origen.satelites.end(); it++ )
    {	  
      satelites.push_back ( *it );
    }
}

Planeta::Planeta ( const Planeta& origen )
{
  Copia ( origen );
}

Planeta Planeta::operator= ( const Planeta& origen )
{
  Copia ( origen );
  return *this;
}

void Planeta::print()
{
  Astro::print();

  cout << "Traslacion  : " << _traslacion << endl;
  cout << "Rot.Interna : " << _rotInterna << endl;

  if ( satelites.size() == 0 )
    cout << "No hay satelistes a mostrar" << endl;
  else
    {
      list<Satelite>::iterator it;
      cout << "=============" << endl;
      cout << "Satelites del planeta" << endl;
      cout << "=============" << endl;
      for ( it = satelites.begin() ; it != satelites.end(); it++ )
	{	  
	  it->print();
	}
    }

}
