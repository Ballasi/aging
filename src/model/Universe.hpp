#ifndef UNIVERSE_HPP
#define UNIVERSE_HPP

#include "Rect.hpp"

class Universe {
public:
  // No constructor for interfaces

  virtual void step() = 0;

  virtual const Rect &bounds() const = 0;
  virtual const BigInt &generation() const = 0;
};

#endif // UNIVERSE_HPP
