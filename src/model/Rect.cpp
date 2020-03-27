#include <model/Rect.h>

Rect::Rect(const Vec2 &top_left, const Vec2 &size)
    : top_left(top_left), size(size) {}
Rect::Rect(const Vec2 &top_left, size_t level)
    : top_left(top_left), size(level) {}

// Equality operators
bool Rect::operator==(const Rect &other) const {
  return top_left == other.top_left && size == other.size;
}
bool Rect::operator!=(const Rect &other) const {
  return top_left != other.top_left || size != other.size;
}

// Collides operator
bool Rect::operator&(const Vec2 &vec) const {
  return (top_left.x <= vec.x && top_left.y <= vec.y &&
          vec.x < bottom_right().x && vec.y < bottom_right().y);
}

bool Rect::operator&(const Rect &other) const {
  return (other.top_left.x < bottom_right().x &&
          other.bottom_right().x >= top_left.x &&
          other.top_left.y < bottom_right().y &&
          other.bottom_right().y >= top_left.y);
}

// Does not collide operator
bool Rect::operator|(const Vec2 &vec) const {
  return (top_left.x > vec.x || top_left.y > vec.y ||

          vec.x >= bottom_right().x || vec.y >= bottom_right().y);
}

bool Rect::operator|(const Rect &rect) const {
  return (rect.top_left.x >= bottom_right().x ||
          rect.bottom_right().x < top_left.x ||
          rect.top_left.y >= bottom_right().y ||
          rect.bottom_right().y < top_left.y);
}

// Move operator
Rect Rect::operator+(const Vec2 &vec) const {
  return Rect(top_left + vec, size);
}
Rect Rect::operator-(const Vec2 &vec) const {
  return Rect(top_left - vec, size);
}
void Rect::operator+=(const Vec2 &vec) { top_left += vec; }
void Rect::operator-=(const Vec2 &vec) { top_left -= vec; }

// Scaling operators
Rect Rect::operator<<(const size_t shift) const {
  return Rect(top_left, size << shift);
}
Rect Rect::operator>>(const size_t shift) const {
  return Rect(top_left, size >> shift);
}
void Rect::operator<<=(const size_t shift) { size <<= shift; }
void Rect::operator>>=(const size_t shift) { size >>= shift; }

// Getters
const BigInt &Rect::x() const { return top_left.x; }
const BigInt &Rect::y() const { return top_left.y; }
const BigInt &Rect::width() const { return size.x; }
const BigInt &Rect::height() const { return size.y; }

// Pseudo getters
const Vec2 Rect::bottom_right() const { return Vec2(top_left + size); }

// Pseudo setters
BigInt &Rect::x() { return top_left.x; }
BigInt &Rect::y() { return top_left.y; }
BigInt &Rect::width() { return size.x; }
BigInt &Rect::height() { return size.y; }
