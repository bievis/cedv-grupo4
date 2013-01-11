#include <iostream>
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include "Sistema.h"
#include "Astro.h"
#include "Planeta.h"
#include "Satelite.h"
#include "Camara.h"

using namespace std;

#define TIEMPO_REFRESCO 100

// -------------------- Variables globales -------------------------
long hours = 0;
Camara camara (0.0, 0.0, 8.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
Sistema SistemaSolar;
bool parar = true;

// GLUT llama a esta funcion cuando se cambia el tamaño de la ventana 
// ------------------------------------------------------------------
void display() 
{
  glClear( GL_COLOR_BUFFER_BIT ); 
  glPushMatrix();
  
  SistemaSolar.pintar(hours);
  
  glutSwapBuffers();      
  glPopMatrix();
}

void timerPintar(int valor) {
  hours++;
  glutPostRedisplay();
  if (!parar) {
    glutTimerFunc(TIEMPO_REFRESCO, timerPintar, 1);
  }
}

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
    gluLookAt (camara.getPosx(), camara.getPosy(), camara.getPosz(),
               camara.getLookx(), camara.getLooky(), camara.getLookz(),
               camara.getUpx(), camara.getUpy(), camara.getUpz());
} 

void actualiza (unsigned char key, int x, int y) 
{
  if (key == 's' || key == 'S') {
    parar = !parar;
    if (!parar) {
      glutTimerFunc(TIEMPO_REFRESCO, timerPintar, 1);
    }
  }
}

/*
Funcion para inicializar el sistema
*/
void inicializarSistema() {
  Astro sol("Sol", 0.0, 255, 186, 0, 0.7, 16);
  Planeta tierra ("Tierra", 2.0, 0, 0, 255, 0.5, 8, 4, 60);
  Satelite luna ("Luna", 1.0, 255, 255, 255, 0.2, 5, 4, 0.0, tierra.getId());
  tierra.getSatelites().push_back(luna);
  Planeta marte ("Marte", 4.0, 666, 34, 255, 0.5, 8, 8, 60);
  Satelite lunaMarte1 ("Luna de Marte1", 1.0, 255, 0, 255, 0.2, 5, 10, 0.0, tierra.getId());
  Satelite lunaMarte2 ("Luna de Marte2", 1.0, 0, 255, 255, 0.2, 5, 20, 0.0, tierra.getId());
  marte.getSatelites().push_back(lunaMarte1);
  marte.getSatelites().push_back(lunaMarte2);
  
  Sistema sistemaSolar("Sistema Solar", sol);
  sistemaSolar.getPlanetas().insert(pair<string, Planeta>(tierra.getId(), tierra));
  sistemaSolar.getPlanetas().insert(pair<string, Planeta>(marte.getId(), marte));

  SistemaSolar = sistemaSolar;
}

/*
Funcion main
*/
int main (int argc, char **argv) {
  glutInit( &argc, argv ); 

  glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE ); 
  glutInitWindowSize( 480, 480 ); 
  glutCreateWindow( "Planetario" ); 

  // Inicializamos el sistema
  inicializarSistema();

  // Registro de funciones de callback
  glutDisplayFunc(display); 
  glutReshapeFunc(resize);
  glutKeyboardFunc (actualiza);

  glutMainLoop();  
  return 0;
}
