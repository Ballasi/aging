#ifndef UNIVERSE_HPP
#define UNIVERSE_HPP

#include "Coord.hpp"
#include "RLE.hpp"
#include "CellState.hpp"

#include <QString>

// Universal Universe interface
class Universe {
public:
  Universe(size_t top_level, Coord top_left) {};
  Universe(QString filename, Coord top_left = Coord()) {}

  // To use in the futur
  // Universe(RLE rle, Coord top_left = 0) {}

  // Makes a step in the simulation
  virtual void step() = 0;


  virtual const CellState get(Coord coord) const = 0;
  virtual void set(Coord, CellState) = 0;

  virtual Coord get_top_left() = 0;
  virtual size_t get_top_level() = 0;

  // // Returns the generatoion count
  // virtual BigInt get_generation() = 0;
};

#endif // UNIVERSE_HPP
