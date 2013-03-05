#ifndef __GAME_CONFIG__
#define __GAME_CONFIG__

#include <vector>
#include <iostream>

using namespace std;

class Row {

 private:
  unsigned int _elements;
  double _speed;

  void copy ( const Row& source );

 public:
  Row() : _elements(0), _speed(0.0) {};
  virtual ~Row();

  Row ( const Row& source );
  Row operator= ( const Row& source );

  inline unsigned int get_num_elements() const { return _elements; }; 
  inline void set_num_elements ( unsigned int value ) { _elements = value; };

  inline double get_speed() const { return _speed; };
  inline void set_speed ( double value ) { _speed = value; };

  void print() const;
  void clear();
};

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

  void clear();

  inline unsigned int getRows() const { return _rows.size(); };
  bool getRow ( unsigned int elem, Row& value );

  bool addRow ( const Row& newValue );

  void print();
};

class Crater : public Elemento {
};

class Road : public Elemento {
};

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

  void clear();

  inline Crater& getCrater() { return _crater; };
  inline Road& getRoad() { return _road; };

  void print();
};

class GameConfig {

 private:
  void copy ( const GameConfig& source );
 
 protected:
  std::vector<Level *> _vLevels;

 public:
  GameConfig();
  virtual ~GameConfig();

  GameConfig ( const GameConfig& source );
  GameConfig operator= ( const GameConfig& source );

  void clear();

  inline unsigned int getNumLevels() const { return _vLevels.size(); };

  bool getLevel ( unsigned int index, Level& level );
  bool addLevel ( Level& newLevel );
 
  void print();
};

#endif
