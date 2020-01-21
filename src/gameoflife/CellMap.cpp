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

void CellMap::changeCellState(unsigned int c, unsigned int l, int toAlive)
{
	unsigned int w = width, h = height;
	Cell* cell_ptr = cells + (l * w) + c;

	if (toAlive)
		*(cell_ptr) |= 0b01;
	else
		*(cell_ptr) &= ~0b01;
}

inline int CellMap::isAlive(unsigned int c, unsigned int l)
{
	return *(cells + (l * width) + c) & 0b01;
}

void CellMap::nextGeneration()
{
	updateNeighbourCount();

	unsigned int w = width, h = height;
	Cell* cell_ptr;

	cell_ptr = cells;
	for (unsigned int l = 0; l < h; ++l)
	{
		for (unsigned int c = 0; c < w; ++c)
		{
			if (*cell_ptr != 0)
			{
				neighbours = *cell_ptr >> 1;
				if (*cell_ptr & 0b01)
				{
					// TODO: adapt the game's rules
					if ((neighbours != 2) && (neighbours != 3))
					{
						changeCellState(c, l, 0);
						game->drawCell(c, l, 0);
					}
				}
				else
				{
					// TODO: adapt the game's rules
					if (neighbours == 3)
					{
						changeCellState(c, l, 1);
						game->drawCell(c, l, 1);
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
	for (unsigned int l = 0; l < h; ++l)
	{
		for (unsigned int c = 0; c < w; ++w)
		{
			if (c > 0 && l > 0)
				*(cell_ptr - w - 1) += 0b10; // top left
			if (l > 0)
				*(cell_ptr - w) += 0b10; // top
			if (c < w - 1 && l > 0)
				*(cell_ptr - w + 1) += 0b10; // top right
			if (c > 0)
				*(cell_ptr - 1) += 0b10; // left
			if (c < w - 1)
				*(cell_ptr + 1) += 0b10; // right
			if (c > 0 && l < h - 1)
				*(cell_ptr + w - 1) += 0b10; // below left
			if (l < h - 1)
				*(cell_ptr + w) += 0b10; // below
			if (c < w - 1 && l < h - 1)
				*(cell_ptr + w + 1) += 0b10; // below right
			++cell_ptr;
		}
	}
}

void CellMap::loadRLE()
{
}
