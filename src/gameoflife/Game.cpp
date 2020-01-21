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

void Game::drawGrid()
{
	for (unsigned int c = 0; c < map->getWidth(); ++c)
		for (unsigned int l = 0; l < map->getHeight(); ++l)
			if (map->isAlive(i, j))
				drawCell(i, j, 1);
}

void Game::nextGeneration()
{
	++generation;
	map->nextGeneration();
}
