#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h> 
#include <stdio.h>

// ==== Definicion de constantes y variables globales =============
static int hombro = 15;
static int codo = 30;
static int anillo = 0;
static double cono1 = 0.2, cono2 = -0.2;
GLfloat matVerde[] = {0.0, 1.0, 0.0, 1.0};
GLfloat matAzul[] = {0.0, 0.0, 1.0, 1.0};
GLfloat matRojo[] = {1.0, 0.0, 0.0, 1.0};
GLfloat matBlanco[] = {1.0, 1.0, 1.0, 1.0};

// ==== Función de callback del teclado ===========================
void teclado(unsigned char key, int x, int y) {
  switch (key) { 
  case 'q': hombro = (hombro++) % 360; break;
  case 'w': hombro = (hombro--) % 360; break;
  case 'a': codo = (codo++) % 360; break;
  case 's': codo = (codo--) % 360; break;
  case '1': anillo = (anillo++) % 360; break;
  case '2': anillo = (anillo--) % 360; break;
  case 'z': if ( cono1 < 0.325 ) 
              cono1 += 0.05; 
            if ( cono2 > -0.325 )
              cono2 -= 0.05;
            break;
  case 'x': if ( cono1 > 0.1 ) 
              cono1 -= 0.05; 
            if ( cono2 < -0.1 ) 
              cono2 += 0.05; 
            break;
  }

  glutPostRedisplay();
}

// ==== Función de dibujado =======================================
void dibujar() { 
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
    glLoadIdentity(); 
    gluLookAt(0.0, 1.0, 6.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0);

    glPushMatrix();

    glRotatef(anillo, 0.0, 1.0, 0.0);

    glPushMatrix();
    glRotatef(90, 1.0, 0.0, 0.0);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, matRojo);
    glutSolidTorus(0.25, 1.0, 16, 8);
    glPopMatrix();

    glRotatef(hombro, 0.0, 0.0, 1.0);
    glTranslatef(1.0, 0.0, 0.0);  
    glPushMatrix();
    glScalef (2.0, 0.25, 0.7);   
    glMaterialfv(GL_FRONT, GL_DIFFUSE, matVerde);
    glutSolidCube(1.0);
    glPopMatrix();
    
    glTranslatef(1.0,0.0,0.0);  
    glRotatef(codo, 0.0, 0.0, 1.0);
    glTranslatef(1.0,0.0,0.0);  // Hasta la mitad
    glPushMatrix();
    glScalef (2.0, 0.25, 0.7);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, matAzul);
    glutSolidCube(1.0);

    //Cono 1
    glPushMatrix();
    glTranslatef(0.5,0.0,cono1);
    glRotatef(90, 0.0, 1.0, 0.0);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, matBlanco);
    glutSolidCone( 0.125, 0.125, 8, 0.125 );
    glPopMatrix();

    //Cono 2
    glPushMatrix();
    glTranslatef(0.5,0.0,cono2);
    glRotatef(90, 0.0, 1.0, 0.0);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, matBlanco);
    glutSolidCone( 0.125, 0.125, 8, 0.125 );
    glPopMatrix();

    glPopMatrix();

    glPopMatrix();

    glutSwapBuffers();      
} 

void escalar(int w, int h) 
{ 
  // Viewport para dibujar en toda la ventana 
  glViewport( 0, 0, w, h ); 
  
  // Actualizamos en la matriz de proyección el ratio ancho/alto 
  glMatrixMode( GL_PROJECTION ); 
  glLoadIdentity(); 
  gluPerspective( 60., (double)w/(double)h, 1., 20. ); 
  
  // Volvemos al modo Vista de Modelo 
  glMatrixMode( GL_MODELVIEW ); 
}

int main( int argc, char** argv ) 
{ 
  GLfloat posluz[] = {0.0, 4.0, 8.0, 0.0}; 
  GLfloat luz[] = {1.0, 1.0, 1.0, 1.0};

  glutInit( &argc, argv ); 
  
  glutInitDisplayMode( GLUT_DEPTH | GLUT_RGB | GLUT_DOUBLE ); 
  glutInitWindowSize(640, 480); 
  glutCreateWindow( "Practicas IG - Sesion 3" ); 
  
  glClearColor(0.2, 0.2, 0.2, 1.0);
  glShadeModel(GL_FLAT);
  glLightfv(GL_LIGHT0, GL_POSITION, posluz);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, luz);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_DEPTH_TEST);

  // Registro de funciones de callback
  glutDisplayFunc(dibujar); 
  glutReshapeFunc(escalar); 
  glutKeyboardFunc(teclado);
  
  glutMainLoop();  
  return 0; 
} 
