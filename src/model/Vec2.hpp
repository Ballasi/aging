#ifndef VEC2_HPP
#define VEC2_HPP

#include <cstddef>

#include "BigInt.hpp"

// Describes a 2
class Vec2 {
public:
  // Inner data
  BigInt x, y;

  // Constructors
  Vec2() = default;
  Vec2(const size_t level);
  Vec2(const BigInt &x, const BigInt &y);

  // Equality operators
  inline bool operator==(const Vec2 &) const;
  inline bool operator!=(const Vec2 &) const;

  // Inclusion operators
  inline bool operator< (const Vec2 &) const;
  inline bool operator<=(const Vec2 &) const;
  inline bool operator> (const Vec2 &) const;
  inline bool operator>=(const Vec2 &) const;

  // Sum operator
  inline Vec2 operator+ (const Vec2 &) const;
  inline Vec2 operator- (const Vec2 &) const;
  inline void operator+=(const Vec2 &);
  inline void operator-=(const Vec2 &);

  // Scaling operators
  inline Vec2 operator<< (const size_t) const;
  inline Vec2 operator>> (const size_t) const;
  inline void operator<<=(const size_t);
  inline void operator>>=(const size_t);
};

#endif // VEC2_HPP
