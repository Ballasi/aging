#ifndef __CELLMAP_H
#define __CELLMAP_H

#include <cstddef>
#include <cstdint>
#include <cstring>

typedef uint8_t Cell;

class Game;

/*
  Cells are memorized in a 8 bit type format.
  Each bits of this byte corresponds to the living state of the cell.
 */

class CellMap
{
public:
  CellMap(Game* g, size_t w, size_t h);
  CellMap(size_t w, size_t h);
  ~CellMap();
  void changeCellState(size_t c, size_t l, int toAlive);
  int isAlive(size_t c, size_t l);
  void nextGeneration();
  size_t getWidth();
  size_t getHeight();
  void clear();

private:
  Game* game;
  Cell* cells;
  size_t width;
  size_t height;
  size_t length_in_bytes;
  void updateNeighbourCount(uint8_t* neighbour_count);
};

#endif // __CELLMAP_H
