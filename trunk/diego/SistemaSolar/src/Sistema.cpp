#include "Sistema.h"

Sistema::~Sistema()
{
  planetas.clear();
  estrellas.clear();
}

void Sistema::print()
{
  cout << "=============" << endl;
  cout << "Planetas" << endl;
  cout << "=============" << endl;

  if ( planetas.size() == 0 )
    cout << "No hay planetas a mostrar" << endl;
  else
    {
      map<string,Planeta>::iterator it;
      
      for ( it = planetas.begin() ; it != planetas.end(); it++ )
	{
	  it->second.print();
	}
    }

  cout << "=============" << endl;
  cout << "Estrellas" << endl;
  cout << "=============" << endl;

  if ( estrellas.size() == 0 )
    cout << "No hay estrellas a mostrar" << endl;
  else
    {
      list<Astro>::iterator it;
      
      for ( it = estrellas.begin() ; it != estrellas.end(); it++ )
	{
	  it->print();
	}
    }

}
