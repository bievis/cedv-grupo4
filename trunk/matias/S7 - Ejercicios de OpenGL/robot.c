#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h> 
#include <stdio.h>

// ==== Definicion de constantes y variables globales =============
static int hombro = 15;
static int codo = 30;
static int base = 0;
static int punta = 0;
static int signo = 1;
GLfloat matVerde[] = {0.0, 1.0, 0.0, 1.0};
GLfloat matAzul[] = {0.0, 0.0, 1.0, 1.0};
GLfloat matRojo[] = {1.0, 0.0, 0.0, 0.0};
GLfloat matBlanco[] = {1.0, 1.0, 1.0, 1.0};

// ==== Función de callback del teclado ===========================
void teclado(unsigned char key, int x, int y) {
  switch (key) { 
  case '1': base = (base + 1) % 360; break; // He quitado el ++ y -- porque no realizaba bien la operación
  case '2': base = (base - 1) % 360; break;
  case 'q': hombro = (hombro + 1) % 360; break;
  case 'w': hombro = (hombro - 1) % 360; break;
  case 'a': codo = (codo++) % 360; break;
  case 's': codo = (codo--) % 360; break;
  case 'z': punta = (punta + 1) % 25;
            if (punta < 0) punta *= -1;
            break;
  case 'x': punta = (punta - 1) % 25;
            if (punta < 0) punta *= -1;
            break;
  }
  glutPostRedisplay();
}

// ==== Función de dibujado =======================================
void dibujar() { 
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
    glLoadIdentity(); 
    gluLookAt(0.0, 1.0, 6.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0);
    
    glRotatef(90, 1.0, 0.0, 0.0);
    glRotatef(base, 0.0, 0.0, 1.0);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, matRojo);
    glutSolidTorus(0.2,1.0,8.0,8.0);
    glTranslatef(-1.0, 0.0, 0.0);

    glRotatef(hombro, 0.0, 1.0, 0.0);
    glTranslatef(1.0, 0.0, 0.0);  
    glPushMatrix();
    glScalef (2.0, 0.7, 0.1); // Para transformar el cubo en rectangular
    glMaterialfv(GL_FRONT, GL_DIFFUSE, matVerde);
    glutSolidCube(1.0);
    glPopMatrix();
    
    glTranslatef(1.0,0.0,0.0);  // Avanzamos al extremo 
    glRotatef(codo, 0.0, 1.0, 0.0);
    glTranslatef(1.0,0.0,0.0);  // Hasta la mitad
    glPushMatrix();
    glScalef (2.0, 0.7, 0.1);  // Para transformar el cubo en rectangular
    glMaterialfv(GL_FRONT, GL_DIFFUSE, matAzul);
    glutSolidCube(1.0);
    glPopMatrix();

    float mover = punta * 0.01;
    glTranslatef(1.0,0.0,0.0);  // Hasta la extremo
    glRotatef(90, 0.0, 1.0, 0.0);
    glPushMatrix();
    glTranslatef(0.0,0.05 + mover,0.0);  // Movemos la punta der
    glMaterialfv(GL_FRONT, GL_DIFFUSE, matBlanco);
    glutSolidCone(0.05,0.2,8.0,8.0);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0.0,-0.05 - mover,0.0);  // Movemos la punta izq
    glMaterialfv(GL_FRONT, GL_DIFFUSE, matBlanco);
    glutSolidCone(0.05,0.2,8.0,8.0);
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
  glutCreateWindow( "Ejercicio 2 - OpenGL" ); 
  
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
