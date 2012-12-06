#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h> 
#include <stdio.h>

// -------------------- Variables globales -------------------------
long hours = 0;

// -------------------- Funcion de dibujado ------------------------
void display() 
{ 
  float RotEarthDay=0.0, RotEarth=0.0, RotMoon=0.0;
  glClear( GL_COLOR_BUFFER_BIT ); 
  glPushMatrix();

  RotEarthDay = (hours % 24) * (360/24.0);
  RotEarth = (hours / 24.0) * (360 / 365.0) * 10; // x10 rapido!
  RotMoon = (hours / 64.8) * (360 / 365.0); //64.8 = 2.7 dias x 24h

  glColor3ub (255, 186, 0);
  glutWireSphere (1, 16, 16);             // Sol (radio 1 y 16 div)
 
  glRotatef (RotEarth, 0.0, 0.0, 1.0);   

  glTranslatef(3, 0.0, 0.0);              // Distancia Sol, Tierra
  glRotatef (RotEarthDay, 0.0, 0.0, 1.0);   
  glColor3ub (0, 0, 255);
  glutWireSphere (0.5, 8, 8);             // Tierra (radio 0.5)

  glTranslatef(1, 0.0, 0.0);              // Distancia Tierra, Luna
  glRotatef (RotMoon, 0.0, 0.0, 1.0);   
  glColor3ub (255, 255, 255);
  glutWireSphere (0.2, 8, 8);             // Luna (radio 0.2)

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
    gluLookAt (0.0, 0.0, 8.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
} 

void actualiza (unsigned char key, int x, int y) 
{ 
  hours++;
  glutPostRedisplay();
}

int main( int argc, char** argv ) 
{ 
    glutInit( &argc, argv ); 

    glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE ); 
    glutInitWindowSize( 480, 480 ); 
    glutCreateWindow( "Planetario" ); 

    // Registro de funciones de callback
    glutDisplayFunc(display); 
    glutReshapeFunc(resize);
    glutKeyboardFunc (actualiza);

    glutMainLoop();  
    return 0; 
} 
