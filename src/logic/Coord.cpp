#include "Coord.hpp"

Coord::Coord(size_t level)
  : x(1 << level), y(1 << level) {}

Coord::Coord(BigInt x, BigInt y)
  : x(x), y(y) {}

bool Coord::operator==(const Coord &coord) {
  return x == coord.x && y == coord.y;
}

Coord Coord::operator+(const Coord &coord) {
  return Coord(x + coord.x,y + coord .y);
}

void Coord::operator+=(const Coord &coord) {
  x += coord.x;
  y += coord.y;
}

Coord Coord::operator-(const Coord &coord) {
  return Coord(x - coord.x, y - coord.y);
}

void Coord::operator-=(const Coord &coord) {
  x -= coord.x;
  y -= coord.y;
}

Coord Coord::operator<<(size_t shift) {
  return Coord(x << shift, y << shift);
}

void Coord::operator<<=(size_t shift) {
  x <<= shift;
  y <<= shift;
}

Coord Coord::operator>>(size_t shift) {
  return Coord(x >> shift, y >> shift);
}

void Coord::operator>>=(size_t shift) {
  x >>= shift;
  y >>= shift;
}
