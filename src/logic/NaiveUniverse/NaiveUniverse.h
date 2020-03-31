#ifndef NAIVEUNIVERSE_H_
#define NAIVEUNIVERSE_H_

#include <cstddef>
#include <cstdint>
#include <cstring>

#include <logic/Coord.hpp>
#include <logic/Universe.hpp>

typedef uint8_t Cell;

/*
  Cells are memorized in a 8 bit type format.
  Each bits of this byte corresponds to the living state of the cell.
 */

class NaiveUniverse : public Universe {
public:
  explicit NaiveUniverse(Coord size);
  ~NaiveUniverse();

  void clear();
  void set(Coord target, CellState state);
  const CellState get(Coord coord) const;
  void step();

private:
  Cell *cells;
  size_t width;
  size_t height;
  size_t length_in_bytes;
  void updateNeighbourCount(uint8_t *neighbour_count);
};

#endif // NAIVEUNIVERSE_H_
