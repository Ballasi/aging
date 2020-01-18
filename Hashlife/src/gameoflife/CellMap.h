#ifndef __CELLMAP_H
#define __CELLMAP_H

typedef struct uint8_t Cell;

class Game;

class CellMap
{
public:
	CellMap(Game* g, unsigned int w, unsigned int h);
	~CellMap();
	void setCell(unsigned int x, unsigned int y);
	void clearCell(unsigned int x, unsigned int y);
	inline int isAlive(int x, int y);
	void nextGeneration();
	void init();
private:
	Game* game;
	unsigned Cell* cells;
	unsigned Cell* temp_cells;
	unsigned int width;
	unsigned int height;
	unsigned int length_in_bytes;
};

#endif // __CELLMAP_H
