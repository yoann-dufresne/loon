#include <unordered_set>


#ifndef COORDZ_H
#define COORDZ_H

class Coord {
public:
  int x;
  int y;

  Coord(int x, int y);
  Coord();
};

class Coordz : public Coord {
public:
  int z;

  Coordz(int x, int y, int z);
  Coordz ();

  bool operator==(const Coordz& a2) const;
};

class my_coordz_hash{
public:
  size_t operator()( const Coordz & key ) const;
};

#endif
