#ifndef VEC2_H_
#define VEC2_H_

#include <cstddef>

#include "BigInt.h"

// Describes a 2
class Vec2 {
public:
  // Inner data
  BigInt x, y;

  // Constructors
  Vec2() = default;
  explicit Vec2(const size_t level);
  Vec2(const BigInt &x, const BigInt &y);

  // Equality operators
  bool operator==(const Vec2 &) const;
  bool operator!=(const Vec2 &) const;

  // Inclusion operators
  bool operator< (const Vec2 &) const;
  bool operator<=(const Vec2 &) const;
  bool operator> (const Vec2 &) const;
  bool operator>=(const Vec2 &) const;

  // Sum operator
  Vec2 operator+ (const Vec2 &) const;
  Vec2 operator- (const Vec2 &) const;
  void operator+=(const Vec2 &);
  void operator-=(const Vec2 &);

  // Scaling operators
  Vec2 operator<< (const size_t) const;
  Vec2 operator>> (const size_t) const;
  void operator<<=(const size_t);
  void operator>>=(const size_t);
};

#endif // VEC2_H_
