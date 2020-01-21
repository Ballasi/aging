#include "Game.h"
#include "CellMap.h"

Game::Game() : generation(0)
{
	map = new CellMap(this, 50, 50);
}

Game::~Game() { }

void Game::drawCell(unsigned int x, unsigned int y, int alive)
{
	// TODO
}

void Game::nextGeneration()
{
	++generation;
	map->nextGeneration();
}
