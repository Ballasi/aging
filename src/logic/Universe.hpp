#ifndef UNIVERSE_HPP
#define UNIVERSE_HPP

#include "CellState.hpp"
#include "Coord.hpp"
#include "LifeFile.hpp"
#include "Rect.hpp"

// Universal Universe interface
class Universe {
public:
  Universe(Rect bounds);
  Universe(LifeFile life_file, Coord top_left = 0);
  Universe(Coord size = Coord(8, 8));

  // Makes a step in the simulation
  virtual void step() = 0;

  // Universe interaction
  virtual void set(Coord, CellState) = 0;
  virtual const CellState get(Coord coord) const = 0;

  class Iterator {
    virtual bool next(Coord &coord, CellState &state) = 0;
  };

  virtual Iterator iterate(Rect bounds) const = 0;

  // Getters
  Rect get_bounds() const;
  BigInt get_generation() const;

  BigInt generation_count;
  Rect bounds;
};

#endif // UNIVERSE_HPP
