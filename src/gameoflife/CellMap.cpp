#include "CellMap.h"
//#include "Game.h"

/*
CellMap::CellMap(Game* g, size_t w, size_t h)
  : game(g), width(w), height(h), length_in_bytes(w * h) {
  // Adding () calls constructor for every Cell in the array
  cells = new Cell[length_in_bytes]();
}
*/

CellMap::CellMap(size_t w, size_t h)
  :width(w), height(h), length_in_bytes(w * h) {
  // Adding () calls constructor for every Cell in the array
  cells = new Cell[length_in_bytes]();
}

CellMap::~CellMap() {
  delete[] cells;
}

void CellMap::changeCellState(size_t c, size_t l, int toAlive) {
  Cell* cell_ptr = cells + (l * width) + c;

  if (toAlive)
    *(cell_ptr) |= LIVING_BIT;
  else
    *(cell_ptr) &= ~LIVING_BIT;
}

int CellMap::isAlive(size_t c, size_t l) {
  return *(cells + (l * width) + c) & LIVING_BIT;
}

void CellMap::nextGeneration() {
  updateNeighbourCount();

  size_t w = width, h = height;
  Cell* cell_ptr;

  cell_ptr = cells;
  for (size_t l = 0; l < h; ++l)
  {
    for (size_t c = 0; c < w; ++c)
    {
      if (*cell_ptr != 0)
      {
        size_t neighbours = *cell_ptr >> 1;
        if (*cell_ptr & LIVING_BIT)
        {
          // TODO: adapt the game's rules
          if ((neighbours != 2) && (neighbours != 3))
          {
            changeCellState(c, l, 0);
            //game->drawCell(c, l, 0);
          }
        }
        else
        {
          // TODO: adapt the game's rules
          if (neighbours == 3)
          {
            changeCellState(c, l, 1);
            //game->drawCell(c, l, 1);
          }
        }
      }
      ++cell_ptr;
    }
  }
}

void CellMap::updateNeighbourCount() {
  Cell* cell_ptr;
  size_t w = width, h = height;

  // we clear previous neighbour calculations
  cell_ptr = cells;
  for (size_t i = 0; i < length_in_bytes; ++i)
  {
    *(cell_ptr) &= LIVING_BIT;
    ++cell_ptr;
  }

  // and we calculate the neighbour count
  cell_ptr = cells;
  for (size_t l = 0; l < h; ++l)
  {
    for (size_t c = 0; c < w; ++c)
    {
      if (*cell_ptr & LIVING_BIT)
      {
        if (c > 0 && l > 0)
          *(cell_ptr - w - 1) += NEIGHBOR_COUNT_BITS; // top left
        if (l > 0)
          *(cell_ptr - w) += NEIGHBOR_COUNT_BITS; // top
        if (c < w - 1 && l > 0)
          *(cell_ptr - w + 1) += NEIGHBOR_COUNT_BITS; // top right
        if (c > 0)
          *(cell_ptr - 1) += NEIGHBOR_COUNT_BITS; // left
        if (c < w - 1)
          *(cell_ptr + 1) += NEIGHBOR_COUNT_BITS; // right
        if (c > 0 && l < h - 1)
          *(cell_ptr + w - 1) += NEIGHBOR_COUNT_BITS; // below left
        if (l < h - 1)
          *(cell_ptr + w) += NEIGHBOR_COUNT_BITS; // below
        if (c < w - 1 && l < h - 1)
          *(cell_ptr + w + 1) += NEIGHBOR_COUNT_BITS; // below right
      }
      ++cell_ptr;
    }
  }
}

void CellMap::loadRLE() {
}

size_t CellMap::getWidth() {
  return width;
}

size_t CellMap::getHeight() {
  return height;
}
