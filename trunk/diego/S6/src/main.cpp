#include "Triangle.h"
#include <vector>
#include <iostream>
#include <stdio.h>

using namespace std;

void print_menu();
void add_triangulo ( Triangle &t );
void add_vector ( Vec3 &v );
void print_vector();
void del_triangulo ( unsigned int index );

vector <Triangle> vec;

int main ()
{
  char ch = 0;
  Triangle t;

  do {
    print_menu();
    ch = cin.get();
 
    switch ( ch )
      {
      case 'A':
      case 'a':
        add_triangulo ( t );
        vec.push_back ( t );
        break;
      case 'D':
      case 'd':
        if ( vec.size() > 0 )
	  {
            unsigned int num = 0;
            cout << "Elija el triangulo a borrar (Valores validos entre 1 y " << vec.size() << ") : " << flush;
            scanf ( "%d", &num );
            del_triangulo ( num );
          }
        else
          cout << "No hay triangulos a eliminar" << endl;
        break;
      case 'P':
      case 'p':
        print_vector();
      }
    cin.get();
  } while ( ch != 'X' && ch != 'x' );

  if ( vec.size() > 0 )
    vec.clear();

}

void print_vector ()
{
  cout << "======================================" << endl;
  cout << "Listado de Triangulos" << endl;
  cout << "======================================" << endl;

  if ( vec.size() > 0 )
    {
      for ( unsigned int i = 0; i < vec.size(); i++ )
        {
          cout << " t[" << i+1 << "] : " 
	       << "v1(" << vec[i].getV1().getX() << "," 
  	       << vec[i].getV1().getY() << "," 
  	       << vec[i].getV1().getZ() << ") "
  	       << "v2(" << vec[i].getV2().getX() << ","
  	       << vec[i].getV2().getY() << ","
  	       << vec[i].getV2().getZ() << ") "
  	       << "v3(" << vec[i].getV3().getX() << ","
  	       << vec[i].getV3().getY() << ","
  	       << vec[i].getV3().getZ() << ") " << endl;
        }
    }
  else
    {
      cout << "<Vacio>" << endl;
    }

  cout << "======================================" << endl;

}

void del_triangulo ( unsigned int index )
{
  if ( vec.size() > 0 && 0 < index && index <= vec.size() )
    {
      vector<Triangle>::iterator it;
      it = vec.begin();
      it += index-1;
      vec.erase ( it );
    }
  else
    cout << "Fuera de rango" << endl;
}

void add_triangulo ( Triangle &t )
{
  Vec3 vec1, vec2, vec3;
  cout << "Introduzca vector 1 : " << endl;
  add_vector ( vec1 );
  cout << "Introduzca vector 2 : " << endl;
  add_vector ( vec2 );
  cout << "Introduzca vector 3 : " << endl;
  add_vector ( vec3 );

  t.setV1(vec1);
  t.setV2(vec2);
  t.setV3(vec3);
}

void add_vector ( Vec3 &v )
{
  int value[3];
  cout << "Valor X: " << flush;
  scanf ( "%d", &value[0] );
  cout << "Valor Y: " << flush;
  scanf ( "%d", &value[1] );
  cout << "Valor Z: " << flush;
  scanf ( "%d", &value[2] );

  v.setX(value[0]);
  v.setY(value[1]);
  v.setZ(value[2]);
}

void print_menu()
{
  cout << "============================" << endl;
  cout << " Menu" << endl;
  cout << "============================" << endl;
  cout << "A - Añadir triangulo" << endl;
  cout << "D - Eliminar triangulo" << endl;
  cout << "P - Imprimir datos triangulo" << endl;
  cout << "X - Salir" << endl;
  cout << "============================" << endl;
}
