#include "Coordz.h"



Coord::Coord(int x, int y) {
  this->x = x;
  this->y = y;
};

Coord::Coord() {};



Coordz::Coordz(int x, int y, int z) {
  this->x = x;
  this->y = y;
  this->z = z;
}

Coordz::Coordz () {
  this->x = 0;
  this->y = 0;
  this->z = 0;
}

bool Coordz::operator==(const Coordz& a2) const {
  return this->x == a2.x && this->y == a2.y && this->z == a2.z;
}


size_t my_coordz_hash::operator()( const Coordz & key ) const // <-- don't forget const
{
  size_t hashVal = key.z;
  return hashVal;
}