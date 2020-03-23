#ifndef __NAIVEUNIVERSE_H
#define __NAIVEUNIVERSE_H

#include <cstddef>
#include <cstdint>
#include <cstring>

#include "../Universe.hpp"
#include "../Coord.hpp"

typedef uint8_t Cell;

/*
  Cells are memorized in a 8 bit type format.
  Each bits of this byte corresponds to the living state of the cell.
 */

class NaiveUniverse: public Universe
{
public:
  NaiveUniverse(Coord size);
  ~NaiveUniverse();

  void clear();
  void set(Coord target, CellState state);
  const CellState get(Coord coord) const;
  void step();

private:
  Cell* cells;
  size_t width;
  size_t height;
  size_t length_in_bytes;
  void updateNeighbourCount(uint8_t* neighbour_count);
};

#endif // __NAIVEUNIVERSE_H
