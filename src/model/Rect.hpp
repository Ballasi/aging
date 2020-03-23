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
  Rect(Vec2 &top_left, Vec2 &size);
  Rect(Vec2 &top_left, size_t level);

  // Equality operators
  inline bool operator==(const Vec2 &) const;
  inline bool operator!=(const Vec2 &) const;

  // Collides operator
  inline bool operator&(const Vec2 &) const;
  inline bool operator&(const Rect &) const;

  // Does not collide operator
  inline bool operator|(const Vec2 &) const;
  inline bool operator|(const Rect &) const;

  // Move operator
  inline Rect operator+ (const Vec2 &) const;
  inline Rect operator- (const Vec2 &) const;
  inline void operator+=(const Vec2 &);
  inline void operator-=(const Vec2 &);

  // Scaling operators
  inline Vec2 operator<< (const size_t) const;
  inline Vec2 operator>> (const size_t) const;
  inline void operator<<=(const size_t);
  inline void operator>>=(const size_t);

  // Getters
  inline BigInt &x() const;
  inline BigInt &y() const;
  inline BigInt &width() const;
  inline BigInt &height() const;

  // Pseudo getters
  inline const Vec2& bottom_right() const;
};

#endif // RECT_HPP
