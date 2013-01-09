#include <iostream>
#include <stdlib.h>
#include "Sistema.h"
#include "Astro.h"
#include "Planeta.h"
#include "Satelite.h"

using namespace std;

/*
Funcion main
*/
int main (int argc, char **argv) {
  float RotEarthDay=0.0, RotEarth=0.0, RotMoon=0.0;
  long hours = 1;  
  
  Sistema sistemaSolar("Sistema Solar");  
  
  RotEarthDay = (hours % 24) * (360/24.0);
  RotEarth = (hours / 24.0) * (360 / 365.0) * 10; // x10 rapido!
  RotMoon = (hours / 24.0) * (360 / 27.0) * 10; // x10 rapido!

  Astro sol("Sol", 0.0, 255, 186, 0, 1.0, 16);
  Planeta tierra ("Tierra", 3.0, 0, 0, 255, 0.5, 8, RotEarth, RotEarthDay);
  Satelite luna ("Luna", 1.0f, 255, 255, 255, 0.2, 5, RotMoon, 0.0, tierra.getId());
  tierra.getSatelites().push_back(luna);

  sistemaSolar.getAstros().insert(pair<string, Astro>(sol.getId(), sol));
  sistemaSolar.getAstros()[tierra.getId()] = tierra;

  cout << sistemaSolar.toString() << endl;
}
