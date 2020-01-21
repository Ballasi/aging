#include "CellMap.h"

CellMap::CellMap(Game* g, unsigned int w, unsigned int h)
	: game(g), width(w), height(h), length_in_bytes(w * h)
{
	cells = new Cell[length_in_bytes];
	memset(cells, 0, length_in_bytes);
}

CellMap::~CellMap()
{
	delete[] cells;
}

void CellMap::changeCellState(unsigned int x, unsigned int y, int toAlive)
{
	unsigned int w = width, h = height;
	Cell* cell_ptr = cells + (y * w) + x;

	if (toAlive)
		*(cell_ptr) |= 0b01;
	else
		*(cell_ptr) &= ~0b01;
}

inline int CellMap::isAlive(unsigned int x, unsigned int y)
{
	return *(cells + (y * width) + x) & 0b01;
}

void CellMap::nextGeneration()
{
	updateNeighbourCount();

	unsigned int w = width, h = height;
	Cell* cell_ptr;

	cell_ptr = cells;
	for (unsigned int y = 0; y < h; ++y)
	{
		for (unsigned int x = 0; x < w; ++x)
		{
			if (*cell_ptr != 0)
			{
				neighbours = *cell_ptr >> 1;
				if (*cell_ptr & 0b01)
				{
					// TODO: adapt the game's rules
					if ((neighbours != 2) && (neighbours != 3))
					{
						changeCellState(x, y, 0);
						game->drawCell(x, y, 0);
					}
				}
				else
				{
					// TODO: adapt the game's rules
					if (neighbours == 3)
					{
						changeCellState(x, y, 1);
						game->drawCell(x, y, 1);
					}
				}
			}
		}
	}
}

void CellMap::updateNeighbourCount()
{
	Cell* cell_ptr;

	// we clear previous neighbour calculations
	cell_ptr = cells;
	for (unsigned int i = 0; i < length_in_bytes; ++i)
	{
		*(cell_ptr) &= 0b01;
		++cell_ptr;
	}

	// and we calculate the neighbour count
	cell_ptr = cells;
	for (unsigned int y = 0; y < h; ++y)
	{
		for (unsigned int x = 0; x < w; ++w)
		{
			if (x > 0 && y > 0)
				*(cell_ptr - w - 1) += 0b10; // top left
			if (y > 0)
				*(cell_ptr - w) += 0b10; // top
			if (x < w - 1 && y > 0)
				*(cell_ptr - w + 1) += 0b10; // top right
			if (x > 0)
				*(cell_ptr - 1) += 0b10; // left
			if (x < w - 1)
				*(cell_ptr + 1) += 0b10; // right
			if (x > 0 && y < h - 1)
				*(cell_ptr + w - 1) += 0b10; // below left
			if (y < h - 1)
				*(cell_ptr + w) += 0b10; // below
			if (x < w - 1 && y < h - 1)
				*(cell_ptr + w + 1) += 0b10; // below right
			++cell_ptr;
		}
	}
}

void CellMap::loadRLE()
{
}
