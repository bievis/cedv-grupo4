#ifndef __ELEMENTO__
#define __ELEMENTO__

#include <Row.hpp>
#include <vector>
#include <iostream>

using namespace std;

class Elemento {

 protected:
  std::vector<Row *> _rows;

 private:
  void copy ( const Elemento& source );

 public:
  Elemento();
  virtual ~Elemento();

  Elemento ( const Elemento& source );
  Elemento operator= ( const Elemento& source );

  inline unsigned int getRows() const { return _rows.size(); };

  bool getRow ( unsigned int elem, Row& value );

  bool addRow ( const Row& newValue );

  void clear();
  void print();
};

#endif
