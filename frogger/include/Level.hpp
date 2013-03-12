#ifndef __LEVEL__
#define __LEVEL__

#include <Crater.hpp>
#include <Road.hpp>
#include <iostream>

using namespace std;

class Level {

 private:
  Crater _crater;
  Road _road;

  void copy ( Level& source );

 public:

  Level();
  virtual ~Level();

  Level ( Level& source );
  Level& operator= ( Level& source );

  inline Crater& getCrater() { return _crater; };
  inline Road& getRoad() { return _road; };

  void clear();
  void print();
};

#endif
