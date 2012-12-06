#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <vector>

using namespace std;

vector <string> palabras;

bool esPalindromo ( const char *cadena );
unsigned int getNumPalindromos();
unsigned int insertaPalabras ( const char *cadena );

int main(int argc, char *argv[]) {

  string cadena;
  string buff;
  unsigned int cont = 0, cont_palabras_linea = 0, cont_palabras = 0;

  if ( argc != 2 )
    {
      cerr << "Numero de argumentos erroneo" << endl;
      cerr << "parser <fichero>" << endl;
      return -1;
    }

  ifstream f ( argv[1] );
  
  if ( !f.good() )
    cerr << "Error al abrir el fichero" << endl;

  while ( !f.eof() ) 
    {
      getline ( f, cadena );
      cont_palabras_linea = insertaPalabras ( cadena.c_str() );
      buff+=cadena;
      cont++;
      cont_palabras += cont_palabras_linea;
    }

  f.close();

  ofstream f_write ( "resultados.txt" );

  //Buscamos lineas y palindromos

  f_write << "Nombre: " << argv[1] << endl;
  f_write << "Tamanyo: "<< buff.length()+cont-1 << endl;
  f_write << "Nº líneas: " << cont-1 << endl;
  f_write << "Nº Palíndromos / Nº Palabras: " << getNumPalindromos() << " / " << cont_palabras << endl;

  if ( f_write.good() )
    f_write.close();

  return 0;
}

unsigned int getNumPalindromos()
{
  unsigned int cont = 0;

  for ( unsigned int i = 0; i < palabras.size(); i++ )
    {
      if ( esPalindromo ( palabras[i].c_str() ) )
        {
          cont++;
        }
    }
  
  return cont;
}

unsigned int insertaPalabras ( const char *cadena )
{
  char *parte = NULL;
  unsigned int cont = 0;
 
  parte = strtok ( (char*)cadena, " " );
  while ( parte )
    {
      palabras.push_back ( parte );
      cont++;
      parte = strtok ( NULL, " " );
    }

  return cont;
}

bool esPalindromo ( const char *palabra )
{
  char cadenaf[250]={0}, *parte;
  int c, len, car=0;
  bool ret = false;
 
  len = strlen ( palabra );
  for ( c = 0; c < len; c++ )
    {
      if ( palabra[len-1-c] == palabra[c] )
        car++;
    }

  if ( car == len )
    ret = true;
  else
    ret = false;
 
  return ret;
}
