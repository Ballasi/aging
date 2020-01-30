#include "Game.h"
#include "CellMap.h"

Game::Game(std:: size_t width, std::size_t height) : generation(0) {
  map = new CellMap(this, width, height);
}

Game::~Game() { }

std::size_t Game::getGeneration() {
  return generation;
}

CellMap* Game::getMap() {
  return map;
}

void Game::nextGeneration() {
  ++generation;
  map->nextGeneration();
}

void Game::loadRLE(QString filename){
}

