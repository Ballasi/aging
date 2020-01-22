#ifndef __GAME_H
#define __GAME_H

#include "CellMap.h"

#include <cstddef>

class Game
{
public:
  Game();
  ~Game();
  void drawCell(size_t x, size_t y, int alive);
  void drawGrid();
  void nextGeneration();

private:
  CellMap* map;
  unsigned long generation;
};

#endif // __GAME_H
