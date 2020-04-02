/*#ifndef __GAME_H
#define __GAME_H

#include "CellMap.h"

#include <QString>
#include <cstddef>

class Game {
public:
  Game(std::size_t width, std::size_t height);
  ~Game();
  void nextGeneration();
  CellMap *getMap();
  std::size_t getGeneration();
  void loadRLE(QString filename);

private:
  CellMap *map;
  std::size_t generation;
};

#endif // __GAME_H
*/
