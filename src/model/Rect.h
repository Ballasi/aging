#ifndef RECT_H_
#define RECT_H_

#include <cstddef>
#include <string>

#include "BigInt.h"
#include "Vec2.h"

class Rect {
public:
  // Inner data
  Vec2 top_left;
  Vec2 size;

  // Constructors
  Rect() = default;
  Rect(const Vec2 &top_left, const Vec2 &size);
  Rect(const Vec2 &top_left, size_t level);

  // Equality operators
  bool operator==(const Rect &) const;
  bool operator!=(const Rect &) const;

  // Collides operator
  bool operator&(const Vec2 &)const;
  bool operator&(const Rect &)const;

  // Does not collide operator
  bool operator|(const Vec2 &) const;
  bool operator|(const Rect &) const;

  // Move operator
  Rect operator+(const Vec2 &) const;
  Rect operator-(const Vec2 &) const;
  void operator+=(const Vec2 &);
  void operator-=(const Vec2 &);

  // Scaling operators
  Rect operator<<(const size_t) const;
  Rect operator>>(const size_t) const;
  void operator<<=(const size_t);
  void operator>>=(const size_t);

  // Getters
  const BigInt &x() const;
  const BigInt &y() const;
  const BigInt &width() const;
  const BigInt &height() const;

  // Pseudo getters
  const Vec2 bottom_right() const;

  // Pseudo setters
  BigInt &x();
  BigInt &y();
  BigInt &width();
  BigInt &height();
};

#endif // RECT_H_
