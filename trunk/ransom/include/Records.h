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

/// \brief This class manages the records in the game
class Records {

private:

  /// \brief reference to Records object
  static Records* msSingleton;
  /// \brief records table
  std::list<string> _records;

  /// \brief method to copy source object in ourself
  /// \param source source object to copy
  void Copy ( Records &source );
  /// \brief default constructor
  Records();
  /// \brief default destructor
  virtual ~Records();
  /// \brief copy constructor
  /// \param source source object to copy
  Records ( Records& source );
  /// \brief method to compare param values with the elements in the records table
  /// \param hostages hostages number rescued
  /// \param seconds seconds spent during the mission
  /// \param value2 value in the records table to compare
  /// \return 0 if is equal, 1 if (new hostages + seconds) is greater than value in the records table (value2)
  int compare ( int hostages, int seconds, string value2 );
  /// \brief method to get system time and it returned with the params values
  /// \param fecha output param with the system date time (dd/mm/yyyy)
  /// \param hora output param with the system time (hh:mm)
  void getFechaHora ( string &fecha, string &hora );

public:

  /// \brief method to get the Records object
  /// \return static Records object
  static Records& getSingleton();
  /// \brief method to get the Records reference
  /// \return reference to Records object
  static Records* getSingletonPtr();
  /// \brief assignment operator for the Records class
  /// \param source the source Records object by reference to will be copied
  /// \return new Records object created
  Records operator= ( Records& source );
  /// \brief method to get size of the records table
  /// \return elements number in the records table
  inline unsigned int getSize() { return _records.size(); };
  /// \brief method to get the value in the element (index) in the records table
  /// \param index position in the records table {0..N-1}
  /// \return string with the value in the index position
  string getValue ( unsigned int index );
  /// \brief method to add an element to the records table
  /// \param hostages hostages number rescued
  /// \param seconds seconds spent during the mission
  void add ( int hostages, int seconds );
  /// \brief method to write the records table to file
  /// The file was named highscores.dat
  void write();
  /// \brief method to read the records table from file
  /// The file was named highscores.dat
  void read();
  /// \brief method to fix records number in the file, namely, it set elements number in the records table obviating the remaining
  /// \param max_size with this value the records elements reamining will be erased from the records table
  void compacta ( unsigned int max_size );
  /// \brief method to print the records table to console
  void print();
};

#endif
