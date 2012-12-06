#include "Lista.h"
#include "Usuario.h"
#include "Canal.h"
#include "Noticia.h"
#include "Admin.h"
#include <iostream>
#include <string>
#include <stdio.h>
#include "ListaExcepcion.h"

using namespace std;

Lista<Canal>   lista_canales;
Lista<Admin>   lista_admins;
Lista<Usuario> lista_usuarios;

unsigned int   find_canal ( const string& nombreCanal );
unsigned int   find_usuario ( const string& nombre );
unsigned int   find_admin ( const string& nombre );
void           ver_top_noticias ( string usuario );
void           print_menu_usuario ( string usuario );
void           print_menu_admin ( string usuario );
void           print_menu_principal();
char           lee_caracter();
void           recibir_noticias_usuario ( string usuario );
void           lista_canales_suscritos ( string usuario );
void           menu_usuario ( string usuario );
void           menu_admin ( string usuario );

int main ( void )
{
  char ch = 0;
  string usuario = "", admin = "";
  Usuario u;
  Admin a;

  while ( ch != 'x' )
    {
      print_menu_principal();

      cout << "Seleccione opcion : ";
      ch = lee_caracter();

      switch (ch)
        {
	  case 'W':
	  case 'w':
		cout << "Usuario : ";
		getline ( cin, usuario );
                if ( find_usuario ( usuario ) )
		  {
                    menu_usuario ( usuario );
		  }
                else if ( find_admin ( usuario ) )
		  {
                    menu_admin ( usuario );
		  }
                else
		  {
		    cerr << "El usuario no existe" << endl;
		  }

                break;
	  case 'U':
	  case 'u':
		cout << "Nuevo Usuario : ";
		getline ( cin, usuario );
                u.setNombre ( usuario ); 
                try 
                  {
                    lista_usuarios.find ( u );
		    cerr << "El usuario existe" << endl;
                  }
                catch ( ListaExcepcion& exc )
		  {
                    lista_usuarios.inserta_n ( u, 1 );
		  }
                break;
	  case 'A':
	  case 'a':
		cout << "Nuevo Administrador : ";
		getline ( cin, admin );
                a.setNombre ( admin );
                try
                  {
                    lista_admins.find ( a );
                    cerr << "El adminsitrador existe" << endl;  
                  }
                catch ( ListaExcepcion& exc )
		  {                    
                    lista_admins.inserta_n ( a, 1 );
		  }
                break;
	  case 'P':
          case 'p':
	    lista_usuarios.print();
	    break;
	  case 'L':
          case 'l':
            lista_admins.print();
	    break;
	}
    }
}

//Devolverá 0 si no lo encuentra y != 0 si lo encuentra (posicion en la lista)
unsigned int find_canal ( const string& nombreCanal )
{
  unsigned int i = 0, pos = 0;
  Canal *c = 0;
  
  for ( i = 1; i <= lista_canales.getNumElems(); i++ )
    {
      lista_canales.getRefElem ( c, i );

      if ( c->getNombreCanal() == nombreCanal )
	{
          pos = i;
          break;
	}
    }
  return pos;
}

//Devolverá 0 si no lo encuentra y != 0 si lo encuentra (posicion en la lista)
unsigned int find_usuario ( const string& nombre )
{
  unsigned int i = 0, pos = 0;
  Usuario *u = 0;
  
  for ( i = 1; i <= lista_usuarios.getNumElems(); i++ )
    {
      lista_usuarios.getRefElem ( u, i );

      if ( u->getNombre() == nombre )
	{
          pos = i;
          break;
	}
    }
  return pos;
}

//Devolverá 0 si no lo encuentra y != 0 si lo encuentra (posicion en la lista)
unsigned int find_admin ( const string& nombre )
{
  unsigned int i = 0, pos = 0;
  Admin *a = 0;
  
  for ( i = 1; i <= lista_admins.getNumElems(); i++ )
    {
      lista_admins.getRefElem ( a, i );

      if ( a->getNombre() == nombre )
	{
          pos = i;
          break;
	}
    }
  return pos;
}

void ver_top_noticias ( string usuario )
{
  Canal *c = 0;
  Usuario *u = 0;
  Noticia *n = 0;
  unsigned int cont = 0;

  cout << "Top 5 noticias por canal" << endl;

  for ( unsigned int i = 1; i <= lista_canales.getNumElems(); i++ )
    {
      cont = 0;
      lista_canales.getRefElem ( c, i );
      for ( unsigned int j = 1; j <= c->getListaUsuarios()->getNumElems(); j++ )
	{
          //Si está suscrito
          c->getListaUsuarios()->getRefElem ( u, j );
          if ( u->getNombre() == usuario )
	    {
              for ( unsigned int k = 1; k <= c->getListaNoticias()->getNumElems(); k++ )
		{
                  if ( cont < 5 )
		    {
		      c->getListaNoticias()->getRefElem ( n, k );
		      if ( n->getInteresante() )
			{
                          if ( cont == 0 )
                            cout << "Canal " << c->getNombreCanal() << endl;
			  n->print();
			  cont++;
			}
		    }
		}
              break;
            }
	}
    }
}

void print_menu_usuario ( string usuario )
{
  cout << "=======================" << endl;
  cout << "Menu de usuario (" << usuario << ")" << endl;
  cout << "=======================" << endl << endl;

  cout << "c - suscribir a un canal" << endl;
  cout << "t - listar todos los canales" << endl;
  cout << "l - listar canales suscritos" << endl;
  cout << "b - darse de baja de un canal" << endl << endl;

  cout << "e - enviar noticia" << endl;
  cout << "r - recibir noticias" << endl << endl;

  cout << "m - ver top noticias" << endl << endl;

  cout << "v - volver menu principal" << endl << endl;
  cout << "=======================" << endl;
}

void print_menu_admin(string usuario)
{
  cout << "=======================" << endl;
  cout << "Menu de administrador (" << usuario << ")" << endl;
  cout << "=======================" << endl << endl;

  cout << "c - crear canal" << endl;
  cout << "l - listar canales" << endl;
  cout << "b - borrar canal" << endl << endl;

  cout << "e - enviar noticia" << endl;
  cout << "r - recibir noticias" << endl << endl;

  cout << "v - volver menu principal" << endl << endl;
  cout << "=======================" << endl;
}

void print_menu_principal()
{
  cout << "=======================" << endl;
  cout << "Menu" << endl;
  cout << "=======================" << endl << endl;

  cout << "w - loguearte" << endl << endl;

  cout << "u - nuevo usuario" << endl;
  cout << "a - nuevo administrador" << endl << endl;

  cout << "p - listado de usuarios" << endl;
  cout << "l - listado de administradores" << endl << endl;

  cout << "x - salir" << endl << endl;
  cout << "=======================" << endl;
}

char lee_caracter()
{
  string aux = "";
  char ch = 0;

  while (true) 
    {
      getline ( cin, aux );

      if (aux.length() == 1) 
        {
          ch = aux[0];
          break;
        }

      cout << "Valor erroneo, vuelva a intentarlo : ";
    }

  return ch;
}

void recibir_noticias_usuario ( string usuario )
{
  Canal *c = 0;
  Usuario *u = 0;
  bool ninguno = true;

  for ( unsigned int i = 1; i <= lista_canales.getNumElems(); i++ )
    {
      lista_canales.getRefElem ( c, i );
      for ( unsigned int j = 1; j <= c->getListaUsuarios()->getNumElems(); j++ )
	{
          c->getListaUsuarios()->getRefElem ( u, j );
          if ( u->getNombre() == usuario )
	    {
              cout << "Canal " << c->getNombreCanal() << endl;
              c->getListaNoticias()->print();
              ninguno = false;
              break;
            }
	}
    }

  if ( ninguno )
    cout << "No hay noticias a mostrar" << endl;
}

void lista_canales_suscritos ( string usuario )
{
  Canal *c = 0;
  Usuario *u = 0;
  bool ninguno = true;

  for ( unsigned int i = 1; i <= lista_canales.getNumElems(); i++ )
    {
      lista_canales.getRefElem ( c, i );
      for ( unsigned int j = 1; j <= c->getListaUsuarios()->getNumElems(); j++ )
	{
          c->getListaUsuarios()->getRefElem ( u, j );
          if ( u->getNombre() == usuario )
	    {
              cout << "Canal suscrito : " << c->getNombreCanal() << endl;
              ninguno = false;
              break;
            }
	}
    }

  if ( ninguno )
    cout << "No esta suscrito a ningun canal" << endl;
}

void menu_usuario ( string usuario )
{
  char ch = 0, interesante = 0;
  string noticia = "";
  string nuevo_mensaje = "";
  string nombre_canal = ""; 
  unsigned int pos = 0;
  Canal *c = 0;

  while ( ch != 'v' )
    {
      print_menu_usuario(usuario);

      cout << "Seleccione opcion : ";
      ch = lee_caracter();
     
      switch (ch)
        {
          //Suscribirse a un canal
	  case 'C':
	  case 'c':
            cout << "Canal donde suscribirse : ";
            getline ( cin, nombre_canal );
  
            if ( "" != nombre_canal )
              {
                if ( !(pos = find_canal ( nombre_canal )) )
		  {
		    cerr << "El canal donde quiere suscribirse no existe" << endl;
		  }
                else
		  {
                    lista_canales.getRefElem ( c, pos );
                    c->suscribir_canal ( usuario );
                    cout << "El usuario " << usuario << " se ha suscrito al canal " << c->getNombreCanal() << endl;
		  }
	      }
	    else
	      {
	        cerr << "El nombre del canal no puede estar vacio" << endl;
                lista_canales.print();
	      }
            break;

	  //Darse de baja de un canal
	  case 'B':
  	  case 'b':
            cout << "Canal donde darse de baja : ";
            getline ( cin, nombre_canal );
  
            if ( "" != nombre_canal )
              {
                if ( !(pos = find_canal ( nombre_canal )) )
		  {
		    cerr << "El canal del que quiere darse de baja no existe" << endl;
		  }
                else
		  {
                    lista_canales.getRefElem ( c, pos );
                    c->cancelar_suscripcion_canal ( usuario );
                    cout << "El usuario " << usuario << " se ha dado de baja del canal " << c->getNombreCanal() << endl;
		  }
	      }
	    else
	      {
	        cerr << "El nombre del canal no puede estar vacio" << endl;
                lista_canales.print();
	      }
            break;
     
	  case 'T':
          case 't':
            lista_canales.print();
            break;

	  case 'L':
          case 'l':
            lista_canales_suscritos ( usuario );
            break;

	  case 'M':
          case 'm':
            ver_top_noticias ( usuario );
            break;

          case 'E':
          case 'e':
            cout << "Canal donde publicar noticia : ";
            getline ( cin, nombre_canal );
  
            if ( "" != nombre_canal )
              {
                if ( !(pos = find_canal ( nombre_canal )) )
		  {
		    cerr << "El canal donde publicar la noticia no existe" << endl;
		  }
                else
		  {
                    lista_canales.getRefElem ( c, pos );
		    
		    cout << "Noticia : ";
		    getline ( cin, noticia );

		    if ( "" != noticia ) 
		      {
		        cout << "Interesante? (s/n) : ";
		        interesante = lee_caracter();

		        c->inserta_noticia ( noticia, (interesante == 's' || interesante == 'S')?true:false );
		      }
		    else
		      {
		        cerr << "La noticia no puede estar vacia" << endl;
		      }
		  }
	      }
	    else
	      {
	        cerr << "El nombre del canal no puede estar vacio" << endl;
                lista_canales.print();
	      }
            break;

          case 'R':
          case 'r':
	    recibir_noticias_usuario ( usuario );
            break;
        }
    }
}

void menu_admin ( string usuario )
{
  //  Noticia *n = 0;
  char ch = 0, interesante = 0;
  string noticia = "";
  string nuevo_mensaje = "";
  string nombre_canal = ""; 
  unsigned int pos = 0;
  Canal *c = 0;

  while ( ch != 'v' )
    {
      print_menu_admin(usuario);

      cout << "Seleccione opcion : ";
      ch = lee_caracter();
     
      switch (ch)
        {
          //Crear un canal
	  case 'C':
	  case 'c':
            cout << "Nombre del nuevo Canal : ";
            getline ( cin, nombre_canal );

            if ( "" != nombre_canal )
              {
                if ( 0 == find_canal ( nombre_canal ) )
		  {
                    c = new Canal;
                    c->setNombreCanal ( nombre_canal );
                    lista_canales.inserta_n ( *c, 1 );
                    cout << "Canal creado correctamente." << endl;
                    delete c;
                  }
                else
                  {
                    cerr << "El canal ya existe" << endl;
                  }
	      }
            else
	      {
	        cerr << "El nombre del canal no puede estar vacio" << endl;               
	      }
            break;
	  //Borrar un canal
	  case 'B':
  	  case 'b':
            cout << "Nombre del Canal a borrar : ";
            getline ( cin, nombre_canal );

            if ( "" != nombre_canal )
              {
		pos = find_canal ( nombre_canal );
                if ( 0 != pos )
		  {                    
                    Canal canal_a_borrar;
                    lista_canales.borra_n ( canal_a_borrar, pos );
                    cout << "Canal << " << canal_a_borrar.getNombreCanal() << " borrado" << endl;
                  }
                else
                  {
                    cerr << "El canal NO existe" << endl;
                  }
	      }
            else
	      {
	        cerr << "El nombre del canal no puede estar vacio" << endl;               
	      }     
            break;
	  case 'L':
          case 'l':
            lista_canales.print();
            break;
          case 'E':
          case 'e':
            cout << "Canal donde publicar noticia : ";
            getline ( cin, nombre_canal );
  
            if ( "" != nombre_canal )
              {
                if ( !(pos = find_canal ( nombre_canal )) )
		  {
		    cerr << "El canal donde publicar la noticia no existe" << endl;
		  }
                else
		  {
                    lista_canales.getRefElem ( c, pos );
		    
		    cout << "Noticia : ";
		    getline ( cin, noticia );

		    if ( "" != noticia ) 
		      {
		        cout << "Interesante? (s/n) : ";
		        interesante = lee_caracter();

		        c->inserta_noticia ( noticia, ( interesante == 's' || interesante == 'S' )?true:false );
		      }
		    else
		      {
		        cerr << "La noticia no puede estar vacia" << endl;
		      }
		  }
	      }
	    else
	      {
	        cerr << "El tipo de canal no puede estar vacio" << endl;
                lista_canales.print();
	      }
            break;

          case 'R':
          case 'r':
            cout << "Recibir noticias del Canal : ";
            getline ( cin, nombre_canal );

            if ( !(pos = find_canal ( nombre_canal )) )
	      {
	        cerr << "El canal no existe" << endl;
	      }
            else
	      {
                lista_canales.getRefElem ( c, pos );
                c->getListaNoticias()->print();
	      }
            break;
        }
    }
}
