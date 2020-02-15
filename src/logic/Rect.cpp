#include "Rect.hpp"

Rect::Rect(Coord top_left, Coord bottom_right)
  : top_left(top_left), bottom_right(bottom_right) {}

bool Rect::is_in(Coord coord) const {
  return (top_left.x <= coord.x &&
          top_left.y <= coord.y &&
          coord.x < bottom_right.x &&
          coord.y < bottom_right.y);
}

bool Rect::collides(Rect rect) const {
  return (rect.top_left.x < bottom_right.x  &&
          rect.bottom_right.x >= top_left.x &&
          rect.top_left.y < bottom_right.y  &&
          rect.bottom_right.y >= top_left.y );
}

bool Rect::does_not_collide(Rect rect) const {
  return (rect.top_left.x >= bottom_right.x || rect.bottom_right.x < top_left.x ||
          rect.top_left.y >= bottom_right.y || rect.bottom_right.y < top_left.y);
}
