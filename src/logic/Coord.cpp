#include "Coord.hpp"

Coord::Coord(size_t level)
  : x(BigInt(1) << mp_size_t(level)), y(BigInt(1) << mp_size_t(level)) {}

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
  return Coord(x << mp_size_t(shift), y << mp_size_t(shift));
}

void Coord::operator<<=(size_t shift) {
  x <<= mp_size_t(shift);
  y <<= mp_size_t(shift);
}

Coord Coord::operator>>(size_t shift) {
  return Coord(x >> mp_size_t(shift), y >> mp_size_t(shift));
}

void Coord::operator>>=(size_t shift) {
  x >>= mp_size_t(shift);
  y >>= mp_size_t(shift);
}
