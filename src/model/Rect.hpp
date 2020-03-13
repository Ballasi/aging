#ifndef RECT_HPP
#define RECT_HPP

#include <cstddef>
#include <string>

#include "BigInt.hpp"
#include "Vec2.hpp"

class Rect {
public:
  // Inner data
  Vec2 size;
  Vec2 top_left;

  // Constructors
  Rect() = default;
  constexpr Rect(Vec2 &top_left, Vec2 &size);
  constexpr Rect(Vec2 &top_left, size_t level);

  // Equality operators
  inline constexpr bool operator==(const Vec2 &) const;
  inline constexpr bool operator!=(const Vec2 &) const;

  // Maybe change whith named operator "collides"
  // Touch operator operator
  inline constexpr bool operator&(const Vec2 &) const;
  inline constexpr bool operator&(const Rect &) const;

  // Do NOT touch operator
  inline constexpr bool operator|(const Vec2 &) const;
  inline constexpr bool operator|(const Rect &) const;

  // Move operator
  inline constexpr Rect operator+ (const Vec2 &) const;
  inline constexpr Rect operator- (const Vec2 &) const;
  inline constexpr void operator+=(const Vec2 &);
  inline constexpr void operator-=(const Vec2 &);

  // Scaling operators
  inline constexpr Vec2 operator<< (const size_t) const;
  inline constexpr Vec2 operator>> (const size_t) const;
  inline constexpr void operator<<=(const size_t);
  inline constexpr void operator>>=(const size_t);

  // Getters
  inline BigInt &x() const;
  inline BigInt &y() const;
  inline BigInt &width() const;
  inline BigInt &height() const;

  // Pseudo getters
  inline const Vec2& bottom_right() const;

private:
  // // Cache (optional)
  mutable bool is_bottom_left_cached;
  mutable Vec2 bottom_left_cache;
};

#endif // RECT_HPP
