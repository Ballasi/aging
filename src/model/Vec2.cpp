#include "Vec2.hpp"

// Constructors
Vec2::Vec2(const size_t level)
  : _x(BigInt(1 << level)), _y(BigInt(1 << level)) {}
Vec2::Vec2(const BigInt &x, const BigInt &y)
  : _x(x), _y(y) {}

// Getters / Setters
inline BigInt &Vec2::x() { return _x; }
inline BigInt &Vec2::y() { return _y; }

// Equality operators
inline bool Vec2::operator==(const Vec2 &other) const {
  return _x == other._x && _y == other._y;
}

inline bool Vec2::operator!=(const Vec2 &other) const {
  return _x != other._x || _y != other._y;
}

// Inclusion operators
inline bool Vec2::operator<(const Vec2 &other) const {
  return _x < other._x && _y < other._y;
}
inline bool Vec2::operator<=(const Vec2 &other) const {
  return _x <= other._x && _y <= other._y;
}
inline bool Vec2::operator>(const Vec2 &other) const {
  return _x > other._x && _y > other._x;
}
inline bool Vec2::operator>=(const Vec2 &other) const {
  return _x >= other._x && _y >= other._y;
}

// Sum operator
inline Vec2 Vec2::operator+(const Vec2 &other) const {
  return Vec2(_x + other._x, _y + other._y);
}
inline Vec2 Vec2::operator-(const Vec2 &other) const {
  return Vec2(_x - other._x, _y - other._y);
}
inline void Vec2::operator+=(const Vec2 &other) {
  _x += other._x;
  _y += other._y;
}
inline void Vec2::operator-=(const Vec2 &other) {
  _x -= other._x;
  _y -= other._y;
}

// Scaling operators
inline Vec2 Vec2::operator<<(const size_t level) const {
  return Vec2(_x << level, _y << level);
}
inline Vec2 Vec2::operator>>(const size_t level) const {
  return Vec2(_x >> level, _y >> level);
}
inline void Vec2::operator<<=(const size_t level) {
  _x <<= level;
  _y <<= level;
}
inline void Vec2::operator>>=(const size_t level) {
  _x >>= level;
  _y >>= level;
}
