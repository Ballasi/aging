#include "Game.h"
#include "CellMap.h"

Game::Game() : generation(0) {
  map = new CellMap(this, 50, 50);
}

Game::~Game() { }

void Game::drawCell(size_t x, size_t y, int alive) {
  // TODO
}

void Game::drawGrid() {
  for (size_t c = 0; c < map->getWidth(); ++c)
    for (size_t l = 0; l < map->getHeight(); ++l)
      if (map->isAlive(c, l))
        drawCell(c, l, 1);
}

void Game::nextGeneration() {
  ++generation;
  map->nextGeneration();
}
