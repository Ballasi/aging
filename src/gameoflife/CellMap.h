#ifndef __CELLMAP_H
#define __CELLMAP_H

typedef struct uint8_t Cell;

class Game;

class CellMap
{
public:
	CellMap(Game* g, unsigned int w, unsigned int h);
	~CellMap();
	void giveLifeToCell(unsigned int x, unsigned int y);
	void killCell(unsigned int x, unsigned int y);
	inline int isAlive(int x, int y);
	void nextGeneration();
	void init();

private:
	Game* game;
	Cell* cells;
	Cell* temp_cells;
	unsigned int width;
	unsigned int height;
	unsigned int length_in_bytes;
};

#endif // __CELLMAP_H
