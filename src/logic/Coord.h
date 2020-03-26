#ifndef COORD_HPP
#define COORD_HPP

#include <cstddef>
#include <logic/BigInt.h>

class Coord {
public:
  BigInt x, y;

  Coord() = default;
  explicit Coord(size_t level);
  Coord(BigInt x, BigInt y);

  bool operator==(const Coord &coord);
  Coord operator+(const Coord &coord);
  void operator+=(const Coord &coord);
  Coord operator-(const Coord &coord);
  void operator-=(const Coord &coord);

  Coord operator<<(size_t shift);
  void operator>>=(size_t shift);
  Coord operator>>(size_t shift);
  void operator<<=(size_t shift);
};

#endif // COORD_HPP
