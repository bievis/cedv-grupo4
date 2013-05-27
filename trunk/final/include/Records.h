#ifndef _RECORDS_H_
#define _RECORDS_H_

#include <list>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <string>
#include <string.h>
#include <time.h>

using namespace std;

class Records {

  static Records* msSingleton;

  std::list<string> _records;

  void Copy ( Records &source );

  Records();
  virtual ~Records();

  Records ( Records& source );

  int compare ( int seconds, string value2 );

  void getFechaHora ( string &fecha, string &hora );

 public:

  static Records& getSingleton();
  static Records* getSingletonPtr();

  Records operator= ( Records& source );

  inline unsigned int getSize() { return _records.size(); };
  string getValue ( unsigned int index );

  void add ( int seconds );

  void write();
  void read();

  void compacta ( unsigned int max_size );

  void print();
};

#endif
