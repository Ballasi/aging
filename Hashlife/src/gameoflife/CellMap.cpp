#include "CellMap.h"

CellMap::CellMap(Game* g, unsigned int w, unsigned int h)
	: game(g) width(w), height(h), length_in_bytes(w * h)
{
	cells = new Cell[length_in_bytes];
	temp_cells = new Cell[length_in_bytes];
	memset(cells, 0, length_in_bytes);
}

CellMap::~CellMap()
{
	delete[] cells;
	delete[] temp_cells;
}

void CellMap::setCell(unsigned int x, unsigned int y)
{
	int w = width, h = height;
	Cell* cell_ptr = cells + (y * w) + x;

	*(cell_ptr) |= 0x01;

	if (x > 0 && y > 0)
		*(cell_ptr - w - 1) += 0x02; // top left
	if (y > 0)
		*(cell_ptr - w) += 0x02; // top
	if (x < w - 1 && y > 0)
		*(cell_ptr - w + 1) += 0x02; // top right
	if (x > 0)
		*(cell_ptr - 1) += 0x02; // left
	if (x < w - 1)
		*(cell_ptr + 1) += 0x02; // right
	if (x > 0 && y < h - 1)
		*(cell_ptr + w - 1) += 0x02; // below left
	if (y < h - 1)
		*(cell_ptr + w) += 0x02; // below
	if (x < w - 1 && y < h - 1)
		*(cell_ptr + w + 1) += 0x02; // below right
}

void CellMap::clearCell(unsigned int x, unsigned int y)
{
	int w = width, h = height;
	Cell* cell_ptr = cells + (y * w) + x;

	*(cell_ptr) &= ~0x01;

	if (x > 0 && y > 0)
		*(cell_ptr - w - 1) -= 0x02; // top left
	if (y > 0)
		*(cell_ptr - w) -= 0x02; // top
	if (x < w - 1 && y > 0)
		*(cell_ptr - w + 1) -= 0x02; // top right
	if (x > 0)
		*(cell_ptr - 1) -= 0x02; // left
	if (x < w - 1)
		*(cell_ptr + 1) -= 0x02; // right
	if (x > 0 && y < h - 1)
		*(cell_ptr + w - 1) -= 0x02; // below left
	if (y < h - 1)
		*(cell_ptr + w) -= 0x02; // below
	if (x < w - 1 && y < h - 1)
		*(cell_ptr + w + 1) -= 0x02; // below right
}

inline int CellMap::isAlive(int x, int y)
{
	return *(cells + (y * width) + x) & 0x01;
}

void CellMap::nextGeneration()
{
	unsigned int count;
	unsigned int w = width, h = height;
	Cell* cell_ptr;

	memcpy(temp_cells, cells, length_in_bytes);

	cell_ptr = temp_cells;
	for (unsigned int y = 0; y < h; ++y)
	{
		for (unsigned int x = 0; x < w; ++x)
		{
			if (*cell_ptr != 0)
			{
				neighbours = *cell_ptr >> 1;
				if (*cell_ptr & 0x01)
				{
					// TODO: reset the rules
					if ((neighbours != 2) && (neighbours != 3))
					{
						clearCell(x, y);
						game->drawCell(x, y, 0);
					}
				}
				else
				{
					// TODO: reset the rules
					if (neighbours == 3)
					{
						setCell(x, y);
						game->drawCell(x, y, 1);
					}
				}
			}
			cell_ptr++;
		}
	}
}

void CellMap::init()
{
}
