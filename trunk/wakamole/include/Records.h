#ifndef _PUNTOS_H_
#define _PUNTOS_H_

#include <vector>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <string>

using namespace std;

class Records {

  std::vector<string> _vRecords;
  
  void Copy ( Records &source );

 public:

  Records();
  ~Records();

  Records ( Records& source );
  Records operator= ( Records& source );

  inline unsigned int getSize() { return _vRecords.size(); };
  string getValue ( unsigned int index );

  void add ( string value );

  void write();
  void read();

  void compacta ( unsigned int max_size );
};

#endif
