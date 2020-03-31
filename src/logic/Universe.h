#ifndef UNIVERSE_HPP
#define UNIVERSE_HPP

#include <logic/CellState.h>
#include <logic/Coord.h>
#include <logic/LifeFile.h>
#include <logic/RLE/RLE.h>
#include <logic/Rect.h>
#include <vector>

// Universal Universe interface
class Universe {
public:
  explicit Universe(Rect bounds);
  explicit Universe(QString filename, Coord top_left);
  explicit Universe(Coord size = Coord(8, 8));
  Universe(size_t top_level, Coord top_left);
  virtual ~Universe();

  // Makes a step in the simulation
  virtual void step() = 0;

  // Universe interaction
  virtual void set(Coord, CellState) = 0;
  virtual const CellState get(Coord coord) const = 0;

  // virtual void get_cell_in_bounds(Rect bounds, vector<Coord> coords,
  // vector<CellState> cell_states) const = 0;

  /*
    class Iterator {
      virtual bool next(Coord &coord, CellState &state) = 0;
    };

    virtual Iterator iterate(Rect bounds) const = 0;
  */

  // Getters
  Rect get_bounds() const;
  BigInt get_generation() const;
  size_t get_step_size();

  BigInt generation_count;
  Rect bounds;
  size_t step_size;
};

#endif // UNIVERSE_HPP
