#include "CellMap.h"
#include "Game.h"

CellMap::CellMap(Game* g, size_t w, size_t h)
  : game(g), width(w + 8 - w % 8), height(h), length_in_bytes((w + 7) * h / 8) {
  // Adding () calls constructor for every Cell in the array
  cells = new Cell[length_in_bytes]();
}

CellMap::CellMap(size_t w, size_t h)
  : width(w + w % 8), height(h), length_in_bytes((w + 7) * h / 8) {
  // Adding () calls constructor for every Cell in the array
  cells = new Cell[length_in_bytes]();
}

CellMap::~CellMap() {
  delete[] cells;
}

void CellMap::clear() {
  for(size_t i = 0; i < length_in_bytes; ++i)
    cells[i] = 0;
}

void CellMap::changeCellState(size_t c, size_t l, int toAlive) {
  Cell* cell_ptr = cells + (l * width / 8) + (c + 7) / 8;

  if (toAlive)
    *(cell_ptr) |= 1 << (c % 8);
  else
    *(cell_ptr) &= ~(1 << (c % 8));
}

int CellMap::isAlive(size_t c, size_t l) {
  return (*(cells + l * width / 8 + (c + 7) / 8) >> (c % 8)) & 0b01;
}

void CellMap::nextGeneration() {
  uint8_t neighbour_count[(width + 1) * height / 2];
  memset(neighbour_count, 0, (width + 1) * height / 2);
  updateNeighbourCount(neighbour_count);

  size_t w = width, h = height;

  uint8_t* count_ptr = neighbour_count;
  for (size_t l = 0; l < h; ++l)
  {
    for (size_t c = 0; c < w; ++c)
    {
      size_t neighbours = *(neighbour_count + l * width / 2 + (c + 1) / 2) >> ((c % 2) * 4) & 0b1111;
      if (isAlive(c, l))
      {
        if ((neighbours != 2) && (neighbours != 3))
          changeCellState(c, l, 0);
      }
      else
      {
        if (neighbours == 3)
          changeCellState(c, l, 1);
      }
    }
  }
}

void CellMap::updateNeighbourCount(uint8_t* neighbour_count) {
  size_t w = width, h = height;

  for (size_t l = 0; l < h; ++l)
  {
    for (size_t c = 0; c < w; ++c)
    {
      if (isAlive(c, l))
      {
        size_t count = 0;
        if (c > 0 && l > 0) // top left
          *(neighbour_count + (l - 1) * w / 2 + c / 2) += 1 << (((c + 1) % 2) * 4);
        if (l > 0) // top
          *(neighbour_count + (l - 1) * w / 2 + (c + 1) / 2) += 1 << ((c % 2) * 4);
        if (c < w - 1 && l > 0) // top right
          *(neighbour_count + (l - 1) * w / 2 + c / 2 + 1) += 1 << (((c + 1) % 2) * 4);
        if (c > 0) // left
          *(neighbour_count + l * w / 2 + c / 2) += 1 << (((c + 1) % 2) * 4);
        if (c < w - 1) // right
          *(neighbour_count + l * w / 2 + c / 2 + 1) += 1 << (((c + 1) % 2) * 4);
        if (c > 0 && l < h - 1) // below left
          *(neighbour_count + (l + 1) * w / 2 + c / 2) += 1 << (((c + 1) % 2) * 4);
        if (l < h - 1) // below
          *(neighbour_count + (l + 1) * w / 2 + (c + 1) / 2) += 1 << ((c % 2) * 4);
        if (c < w - 1 && l < h - 1) // below right
          *(neighbour_count + (l + 1) * w / 2 + c / 2 + 1) += 1 << (((c + 1) % 2) * 4);
      }
    }
  }
}

size_t CellMap::getWidth() { return width; }

size_t CellMap::getHeight() { return height; }
