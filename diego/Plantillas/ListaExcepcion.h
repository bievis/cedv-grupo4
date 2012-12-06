#ifndef __LISTA_EXCEPCION_H__
#define __LISTA_EXCEPCION_H__

using namespace std;

class ListaExcepcion : public exception {
  public:
   ListaExcepcion ( int motivo ) : exception(), _motivo(motivo) {}
   const char* what() const throw(); 
  private:
   int _motivo;
};

const char* ListaExcepcion::what() const throw() {
   switch(_motivo) {
     case 1: 
        return "El elemento no existe";
   }
   return "Error inesperado";
} // (1)

#endif
