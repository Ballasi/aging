#include <model/Vec2.h>

// Constructors
Vec2::Vec2(const size_t level)
  : x(BigInt(1 << level)), y(BigInt(1 << level)) {}
Vec2::Vec2(const BigInt &x, const BigInt &y)
  : x(x), y(y) {}

// Equality operators
bool Vec2::operator==(const Vec2 &other) const {
  return x == other.x && y == other.y;
}

bool Vec2::operator!=(const Vec2 &other) const {
  return x != other.x || y != other.y;
}

// Inclusion operators
bool Vec2::operator<(const Vec2 &other) const {
  return x < other.x && y < other.y;
}
bool Vec2::operator<=(const Vec2 &other) const {
  return x <= other.x && y <= other.y;
}
bool Vec2::operator>(const Vec2 &other) const {
  return x > other.x && y > other.x;
}
bool Vec2::operator>=(const Vec2 &other) const {
  return x >= other.x && y >= other.y;
}

// Sum operator
Vec2 Vec2::operator+(const Vec2 &other) const {
  return Vec2(x + other.x, y + other.y);
}
Vec2 Vec2::operator-(const Vec2 &other) const {
  return Vec2(x - other.x, y - other.y);
}
void Vec2::operator+=(const Vec2 &other) {
  x += other.x;
  y += other.y;
}
void Vec2::operator-=(const Vec2 &other) {
  x -= other.x;
  y -= other.y;
}

// Scaling operators
Vec2 Vec2::operator<<(const size_t level) const {
  return Vec2(x << level, y << level);
}
Vec2 Vec2::operator>>(const size_t level) const {
  return Vec2(x >> level, y >> level);
}
void Vec2::operator<<=(const size_t level) {
  x <<= level;
  y <<= level;
}
void Vec2::operator>>=(const size_t level) {
  x >>= level;
  y >>= level;
}
