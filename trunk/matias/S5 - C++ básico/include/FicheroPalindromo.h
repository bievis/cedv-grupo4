#ifndef FICHERO_PALINDROMO_H
#define FICHERO_PALINDROMO_H

// Constantes
const int SIZE_PALABRA = 1024;

// Clase fichero palindromo
class FicheroPalindromo {
public:
  FicheroPalindromo () { };
  void obtenerPalindromos (char* nombreArchivo);
private:
  void escribirArchivoSalida (char* nombreArchivo, int nLetras = 0, int nLineas = 0, int nPalindromos = 0);
};

#endif
