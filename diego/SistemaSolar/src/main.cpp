#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h> 
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include "Camara.h"
#include "Sistema.h"

Sistema sistema_solar;
Camara camara;

void define_mundo();
void repintar(int value);

bool centinela = false;

#define _TIME_REFRESH_ 50

// -------------------- Variables globales -------------------------
long hours = 0;

// -------------------- Funcion de dibujado ------------------------
// void display() 
// { 
//   float RotEarthDay=0.0, RotEarth=0.0, RotMoon=0.0;
//   glClear( GL_COLOR_BUFFER_BIT ); 
//   glPushMatrix();

//   RotEarthDay = (hours % 24) * (360/24.0);
//   RotEarth = (hours / 24.0) * (360 / 365.0) * 10; // x10 rapido!
//   RotMoon = (hours / 64.8) * (360 / 365.0); //64.8 = 2.7 dias x 24h

//   glColor3ub (255, 186, 0);
//   glutWireSphere (1, 16, 16);             // Sol (radio 1 y 16 div)
 
//   glRotatef (RotEarth, 0.0, 0.0, 1.0);   

//   glTranslatef(3, 0.0, 0.0);              // Distancia Sol, Tierra
//   glRotatef (RotEarthDay, 0.0, 0.0, 1.0);   
//   glColor3ub (0, 0, 255);
//   glutWireSphere (0.5, 8, 8);             // Tierra (radio 0.5)

//   glTranslatef(1, 0.0, 0.0);              // Distancia Tierra, Luna
//   glRotatef (RotMoon, 0.0, 0.0, 1.0);   
//   glColor3ub (255, 255, 255);
//   glutWireSphere (0.2, 8, 8);             // Luna (radio 0.2)

//   glutSwapBuffers();      
  
//   glPopMatrix();
// } 

void display() 
{ 
  //  float RotEarthDay=0.0, RotEarth=0.0, RotMoon=0.0;
  //float RotEarthDay=0.0;

  glClear( GL_COLOR_BUFFER_BIT ); 

  glPushMatrix();

  //RotEarthDay = (hours % 24) * (360/24.0);
  //  RotEarth = (hours / 24.0) * (360 / 365.0) * 10; // x10 rapido!
  //  RotMoon = (hours / 64.8) * (360 / 365.0); //64.8 = 2.7 dias x 24h

  list<Astro>::iterator it_est;

  for ( it_est = sistema_solar.estrellas.begin(); it_est != sistema_solar.estrellas.end(); it_est++ )
    {      
      glColor3ub ( it_est->getRojo(), it_est->getVerde(), it_est->getAzul() );
      glutWireSphere ( it_est->getRadio(), 16, 16 );             // Sol (radio 1 y 16 div)
    }
 
  map<string,Planeta>::iterator it_pla;
      
  for ( it_pla = sistema_solar.planetas.begin() ; it_pla != sistema_solar.planetas.end(); it_pla++ )
    {
      //      glRotatef ( RotEarth, 0.0, 0.0, 1.0 );
      glRotatef ( (*it_pla).second.getTraslacion ( hours ), 0.0, 0.0, 1.0 );
      
      glTranslatef ( (*it_pla).second.getPosX(), (*it_pla).second.getPosY(), (*it_pla).second.getPosZ() );              // Distancia Sol, Tierra

      (*it_pla).second.setRotacion ( (hours % 24) * (360/24.0) );
      //glRotatef ( RotEarthDay, 0.0, 0.0, 1.0 );   
      glRotatef ( (*it_pla).second.getRotacion(), 0.0, 0.0, 1.0 );   
      glColor3ub ( (*it_pla).second.getRojo(), (*it_pla).second.getVerde(), (*it_pla).second.getAzul() );
      glutWireSphere ( (*it_pla).second.getRadio(), 8, 8 );             // Tierra (radio 0.5)

      if ( (*it_pla).second.satelites.size() > 0 )
      	{
      	  for ( list<Satelite>::const_iterator it_sat = (*it_pla).second.satelites.begin(); it_sat != (*it_pla).second.satelites.end(); it_sat++ )
      	    {
      	      glTranslatef ( it_sat->getPosX(), it_sat->getPosY(), it_sat->getPosZ() );
      	      glRotatef ( it_sat->getTraslacion ( hours ), 0.0, 0.0, 1.0 );
              glColor3ub ( it_sat->getRojo(), it_sat->getVerde(), it_sat->getAzul() );
              glutWireSphere ( it_sat->getRadio(), 8, 8 );             // Luna (radio 0.2) 
      	    }
      	}

      glPopMatrix();
      glPushMatrix();
    }

  glutSwapBuffers();      
  
  glPopMatrix();
} 

// GLUT llama a esta funcion cuando se cambia el tamaño de la ventana 
// ------------------------------------------------------------------
void resize(int w, int h) 
{ 
    // Viewport para dibujar en toda la ventana 
    glViewport( 0, 0, w, h ); 

    // Actualizamos en la matriz de proyección el ratio ancho/alto 
    glMatrixMode( GL_PROJECTION ); 
    glLoadIdentity(); 
    gluPerspective( 60., (double)w/(double)h, 1., 20. ); 
    // Volvemos al modo Vista de Modelo 
    glMatrixMode( GL_MODELVIEW ); 
    glLoadIdentity();
    //    gluLookAt (0.0, 0.0, 8.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    gluLookAt ( camara.getEyeX(), camara.getEyeY(), camara.getEyeZ(), 
		camara.getCenterX(), camara.getCenterY(), camara.getCenterZ(), 
		camara.getUpX(), camara.getUpY(), camara.getUpZ() );

} 

void actualiza (unsigned char key, int x, int y) 
{ 


  if ( 's' == key )
    {
      if ( !centinela )
	{
          centinela = true;
          glutTimerFunc ( _TIME_REFRESH_, repintar, 1 );
	}
      else
	{
          centinela = false;
	}

    }

}

int main( int argc, char** argv ) 
{ 
    glutInit( &argc, argv ); 

    glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE ); 
    //    glutInitWindowSize( 480, 480 ); 
    glutInitWindowSize( 800, 600 ); 
    glutCreateWindow( "Sistema Solar - Presione 'S' para arrancar/parar la animacion" ); 
    
    //    glutFullScreen();

    define_mundo();

    // Registro de funciones de callback
    glutDisplayFunc(display); 
    glutReshapeFunc(resize);
    glutKeyboardFunc (actualiza);

    glutMainLoop();  


    return 0; 
} 

void define_mundo()
{

  //Creamos la luna
  Satelite luna;
  luna.setID ( "Luna" );
  luna.setPosX ( 1 );
  luna.setRojo ( 255 );
  luna.setAzul ( 255 );
  luna.setVerde ( 255 );
  luna.setRadio ( 0.1 );
  // Resultado de : (360 / 365.0) / 64.8
  luna.setTraslacion ( 0.0152207 ); // (  (hours / 64.8) * (360 / 365.0)

  //Creamos la tierra
  Planeta tierra;
  tierra.setID ( "Tierra" );
  tierra.setAzul ( 255 );
  tierra.setRadio ( 0.25 );
  tierra.setPosX ( 3 );
  //Resultado de : ( (360 / 365.0) * 10 ) / 24
  tierra.setTraslacion ( 0.410958904 );  // (hours / 24.0) * (360 / 365.0) * 10; // x10 rapido!
  tierra.satelites.push_back ( luna );

  //Creamos marte
  Planeta marte;
  marte.setID ( "Marte" );
  marte.setRojo ( 255 );
  marte.setRadio ( 0.4 );
  marte.setTraslacion ( 0.205479452 ); 
  marte.setPosX ( 5 );

  //Creamos jupiter
  Planeta jupiter;
  jupiter.setID ( "Jupiter" );
  jupiter.setVerde ( 255 );
  jupiter.setRadio ( 0.3 );
  jupiter.setTraslacion ( 0.105479452 );
  jupiter.setPosX ( 7 );

  //Creamos saturno
  Planeta saturno;
  saturno.setID ( "Saturno" );
  saturno.setRojo ( 255 );
  saturno.setVerde ( 255 );
  saturno.setRadio ( 0.15 );
  saturno.setTraslacion ( 0.155479452 );
  saturno.setPosX ( 9 );

  //Creamos el sol  
  Astro sol;
  sol.setID ( "Sol" );
  sol.setRojo ( 255 );
  sol.setAzul ( 186 );
  sol.setRadio ( 1 );
  //  sol.print();
  
  sistema_solar.planetas.insert ( pair<string, Planeta> ( tierra.getID(), tierra ) );
  sistema_solar.planetas.insert ( pair<string, Planeta> ( marte.getID(), marte ) );
  sistema_solar.planetas.insert ( pair<string, Planeta> ( jupiter.getID(), jupiter ) );
  sistema_solar.planetas.insert ( pair<string, Planeta> ( saturno.getID(), saturno ) );
  sistema_solar.estrellas.push_back ( sol );
  sistema_solar.print();

  //Configuramos camara
  camara.setEyeX( 0.0 );
  camara.setEyeY( 0.0 );
  camara.setEyeZ( 18.0 );
  camara.setCenterX( 0.0 );
  camara.setCenterY( 0.0 );
  camara.setCenterZ( 0.0 );
  camara.setUpX ( 0.0 );
  camara.setUpY ( 1.0 );
  camara.setUpZ ( 0.0 );
  camara.print();

  //Pausa
  //  cin.get();
}

void repintar(int value)
{
  if (centinela)
    {
      hours++;
      glutTimerFunc ( _TIME_REFRESH_, repintar, 1 );
      glutPostRedisplay();
    }
}
