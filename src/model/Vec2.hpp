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
  constexpr Vec2(const size_t level);
  constexpr Vec2(const BigInt &x, const BigInt &y);

  // Equality operators
  inline constexpr bool operator==(const Vec2 &) const;
  inline constexpr bool operator!=(const Vec2 &) const;

  // Inclusion operators
  inline constexpr bool operator< (const Vec2 &) const;
  inline constexpr bool operator<=(const Vec2 &) const;
  inline constexpr bool operator> (const Vec2 &) const;
  inline constexpr bool operator>=(const Vec2 &) const;

  // Sum operator
  inline constexpr Vec2 operator+ (const Vec2 &) const;
  inline constexpr Vec2 operator- (const Vec2 &) const;
  inline constexpr void operator+=(const Vec2 &);
  inline constexpr void operator-=(const Vec2 &);

  // Scaling operators
  inline constexpr Vec2 operator<< (const size_t) const;
  inline constexpr Vec2 operator>> (const size_t) const;
  inline constexpr void operator<<=(const size_t);
  inline constexpr void operator>>=(const size_t);
};

#endif // VEC2_HPP
