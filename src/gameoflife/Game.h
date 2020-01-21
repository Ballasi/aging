#ifndef __GAME_H
#define __GAME_H

#include "CellMap.h"

class Game
{
public:
	Game();
	~Game();
	void drawCell(unsigned int x, unsigned int y, int alive);
	void drawGrid();
	void nextGeneration();

private:
	CellMap* map;
	unsigned long generation;
};

#endif // __GAME_H
