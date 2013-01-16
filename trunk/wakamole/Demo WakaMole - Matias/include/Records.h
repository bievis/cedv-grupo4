#ifndef _PUNTOS_H_
#define _PUNTOS_H_

#include <vector>
#include <fstream>
#include <iostream>

using namespace std;

class Records {

  vector<unsigned int> _vRecords;
  
  void Copy ( Records &source );

 public:

  Records();
  ~Records();

  Records ( Records& source );
  Records operator= ( Records& source );

  inline unsigned int getSize() { return _vRecords.size(); };
  unsigned int getValue ( unsigned int index );

  void add ( unsigned int value );

  void write();
  void read();

};

#endif
