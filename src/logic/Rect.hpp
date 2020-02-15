#ifndef RECT_HPP
#define RECT_HPP

#include "Coord.hpp"

class Rect {
public:
  Rect() = default;
  Rect(Coord top_left, Coord bottom_right);

  bool is_in(Coord coord) const;
  bool collides(Rect rect) const;
  bool does_not_collide(Rect rect) const;

  Coord top_left;
  Coord bottom_right;
};

#endif // RECT_HPP
