#include "Vec2.hpp"

// Constructors
Vec2::Vec2(const size_t level) {}
Vec2::Vec2(const BigInt &x, const BigInt &y){}

// Equality operators
inline  bool Vec2::operator==(const Vec2 &other) const {
  return x == other.x && y == other.y;
}

inline bool Vec2::operator!=(const Vec2 &other) const {
  return x != other.x || y != other.y;
}

// Inclusion operatorsVec2::
inline bool Vec2::operator<(const Vec2 &other) const {
  return x < other.x && y < other.y;
}
inline bool Vec2::operator<=(const Vec2 &other) const {
  return x <= other.x && y <= other.y;
}
inline bool Vec2::operator>(const Vec2 &other) const {
  return x > other.x && y > other.x;
}
inline bool Vec2::operator>=(const Vec2 &other) const {
  return x >= other.x && y >= other.y;
}

// Sum operator
inline Vec2 Vec2::operator+(const Vec2 &other) const {
  return Vec2(x + other.x, y + other.y);
}
inline Vec2 Vec2::operator-(const Vec2 &other) const {
  return Vec2(x - other.x, y - other.y);
}
inline void Vec2::operator+=(const Vec2 &other) {
  x += other.x;
  y += other.y;
}
inline void Vec2::operator-=(const Vec2 &other) {
  x -= other.x;
  y -= other.y;
}

// Scaling operators
inline Vec2 Vec2::operator<<(const size_t level) const {
  return Vec2(x << level, y << level);
}
inline Vec2 Vec2::operator>>(const size_t level) const {
  return Vec2(x >> level, y >> level);
}
inline void Vec2::operator<<=(const size_t level) {
  x <<= level;
  y <<= level;
}
inline void Vec2::operator>>=(const size_t level) {
  x >>= level;
  y >>= level;
}
