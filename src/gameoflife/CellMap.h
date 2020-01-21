#ifndef __CELLMAP_H
#define __CELLMAP_H

typedef uint8_t Cell;

/*
 * Cells are a byte long.
 * They are defined as followed:
 *    0001 1112
 * where 0s are not used,
 *       1s are the neighbour count,
 *       2  is the state of the cell (1 means alive).
 * The difference between this version and the old one is that the neighbour
 * count is only calculated when making up a new generation.
 * This allows us to not use the temp_cells array.
 */

class Game;

class CellMap
{
public:
	CellMap(Game* g, unsigned int w, unsigned int h);
	~CellMap();
	void changeCellState(unsigned int c, unsigned int l, int toAlive);
	inline int isAlive(unsigned int c, unsigned int l);
	void nextGeneration();
	void loadRLE();

private:
	Game* game;
	Cell* cells;
	unsigned int width;
	unsigned int height;
	unsigned int length_in_bytes;
	void updateNeighbourCount();
};

#endif // __CELLMAP_H
