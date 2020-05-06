#ifndef NAIVEUNIVERSE_H_
#define NAIVEUNIVERSE_H_

#include <cstddef>
#include <cstdint>
#include <cstring>

#include <logic/Coord.h>
#include <logic/Universe.h>

typedef uint8_t Cell;

/*
  Cells are memorized in a 8 bit type format.
  Each bits of this byte corresponds to the living state of the cell.
 */

class NaiveUniverse : public Universe {
public:
  explicit NaiveUniverse(Coord size);
  explicit NaiveUniverse(QString filename);
  ~NaiveUniverse();

  void clear();
  void set(Coord target, CellState state);
  const CellState get(Coord coord) const;
  void step();

  size_t width;
  size_t height;

  const Coord get_size() const;
private:
  Cell *cells;
  size_t length_in_bytes;
  uint8_t *neighbour_count;
  void updateNeighbourCount(uint8_t *neighbour_count);

  Coord read_rle_size(QFile *file, Coord *size);
  void read_rle_data(QFile *file, Coord boundingbox, Coord size);
};

#endif // NAIVEUNIVERSE_H_
