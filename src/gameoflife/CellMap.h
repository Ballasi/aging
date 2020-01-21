#ifndef __CELLMAP_H
#define __CELLMAP_H

#include <cstdint>
#include <cstring>

#define LIVING_BIT          0b01
#define NEIGHBOR_COUNT_BITS 0b10

typedef uint8_t Cell;

class Game;

/*
 * Cells are a byte long.
 * They are defined as followed:
 *    ...a aaab
 * where .s are not used,
 *       as are the neighbour count,
 *       b  is the state of the cell (1 means alive).
 * The difference between this version and the old one is that the neighbour
 * count is only calculated when making up a new generation.
 * This allows us to not use the temp_cells array.
 */

class CellMap
{
public:
	CellMap(Game* g, size_t w, size_t h);
	~CellMap();
	void changeCellState(size_t c, size_t l, int toAlive);
	inline int isAlive(size_t c, size_t l);
	void nextGeneration();
	void loadRLE();
	size_t getWidth();
	size_t getHeight();

private:
	Game* game;
	Cell* cells;
	size_t width;
	size_t height;
	size_t length_in_bytes;
	void updateNeighbourCount();
};

#endif // __CELLMAP_H
