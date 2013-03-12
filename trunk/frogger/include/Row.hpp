#ifndef __ROW_H__
#define __ROW_H__

#include <iostream>

using namespace std;

enum Way {
  LEFT,
  RIGHT
};

class Row {

 private:
  string _name;
  double _speed;
  enum Way _way;
  double _distance;

  void copy ( const Row& source );

 public:
  Row() : _name(""), _speed(0.0), _way(RIGHT), _distance(0.0) {};
  virtual ~Row();

  Row ( const Row& source );
  Row operator= ( const Row& source );

  inline string get_name() const { return _name; };
  inline void set_name ( const string& value ) { _name = value; };

  inline double get_speed() const { return _speed; };
  inline void set_speed ( double value ) { _speed = value; };

  inline Way get_way() const { return _way; };
  inline void set_way(enum Way value) { _way = value; };

  inline double get_distance() const { return _distance; };
  inline void set_distance ( double value ) { _distance = value; };

  void print() const;
  void clear();
};

#endif
