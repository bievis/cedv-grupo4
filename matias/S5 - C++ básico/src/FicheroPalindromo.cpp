#include <fstream>
#include <ctype.h>
#include <string.h>
#include <iostream>
#include "FicheroPalindromo.h"
#include "Utils.h"

using namespace std;

/*
Funcion que extra los palindromos del fichero que le pasemos y genera un fichero de salida con la informacion
*/
void FicheroPalindromo::obtenerPalindromos (char* nombreArchivo) {
  ifstream archivo(nombreArchivo, ios::in | ios::binary);
  if (archivo.good()){
    char caracter;
    char palabra[SIZE_PALABRA];
    Utils::vaciarPalabra (palabra);
    int iPalabra = 0;
    int nLetras = 0; // Cada caracter ocupa un byte
    int nLineas = 0;
    int nPalindromos = 0;
    while (archivo.good()) {
      caracter = tolower(archivo.get());
      nLetras++;
      nLineas+=Utils::isSaltoLinea(caracter);
      if (isalpha(caracter)) {
        palabra[iPalabra] = caracter;
        iPalabra++;
      } else {
        if (iPalabra > 0) {
          nPalindromos+=Utils::isPalindromo(palabra);
        }
        Utils::vaciarPalabra (palabra);
        iPalabra = 0;
      }
    }
    escribirArchivoSalida (nombreArchivo, nLetras, nLineas, nPalindromos);
  }else{
    cout << "Error al leer el archivo: " << nombreArchivo << endl;
  }
  archivo.close();
}

/*
Escribe el archivo de salida
*/
void FicheroPalindromo::escribirArchivoSalida (char* nombreArchivo, int nLetras, int nLineas, int nPalindromos) {
  char *nombreArchivoSalida = strcat(nombreArchivo, ".salida");
  // Creamos el archivo de salida
  ofstream archivoSalida(nombreArchivoSalida);
  if (archivoSalida.good()) {
    // Escribimos el cotenido del archivo
    archivoSalida << "Nombre: " << nombreArchivo << endl;
    archivoSalida << "Tamanyo: " << nLetras << endl; 
    archivoSalida << "Nº líneas: " << nLineas << endl;
    archivoSalida << "Nº palabras palíndromo: " << nPalindromos << endl;
    
    cout << "Se ha creado el archivo '" << nombreArchivo << "' con la salida de la ejecución."<< endl;
  } else {
    cout << "Error al escribir el archivo: " << nombreArchivo << endl;
  }
  // Cerramos el archivo
  archivoSalida.close();
}
