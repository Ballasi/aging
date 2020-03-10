#include "NaiveUniverse.hpp"
#include <gmp.h>

NaiveUniverse::NaiveUniverse(Coord size)
    : width(size.x.get_si() + 8 - size.x.get_si() % 8), height(size.y.get_si()), length_in_bytes((size.x.get_si() + 7) * size.y.get_si() / 8) {
  // Adding () calls constructor for every Cell in the array
  cells = new Cell[length_in_bytes]();
}

NaiveUniverse::~NaiveUniverse() { delete[] cells; }

void NaiveUniverse::clear() {
  for (size_t i = 0; i < length_in_bytes; ++i)
    cells[i] = 0;
}

void NaiveUniverse::set(Coord target, CellState state) {
  Cell *cell_ptr = cells + (target.y.get_si() * width / 8) + (target.x.get_si() + 7) / 8;

  if (state)
    *(cell_ptr) |= 1 << (target.x.get_si() % 8);
  else
    *(cell_ptr) &= ~(1 << (target.x.get_si() % 8));
}

const CellState NaiveUniverse::get(Coord coord) const {
  return (*(cells + coord.y.get_si() * width / 8 + (coord.x.get_si() + 7) / 8) >> (coord.x.get_si() % 8)) & 0b01;
}

void NaiveUniverse::step() {
  uint8_t neighbour_count[(width + 1) * height / 2];
  memset(neighbour_count, 0, (width + 1) * height / 2);
  updateNeighbourCount(neighbour_count);

  size_t w = width, h = height;

  for (size_t l = 0; l < h; ++l) {
    for (size_t c = 0; c < w; ++c) {
      size_t neighbours =
          *(neighbour_count + l * width / 2 + (c + 1) / 2) >> ((c % 2) * 4) &
          0b1111;
      if (get(Coord(c, l))) {
        if ((neighbours != 2) && (neighbours != 3))
          set(Coord(c, l), 0);
      } else {
        if (neighbours == 3)
          set(Coord(c, l), 1);
      }
    }
  }

  ++generation_count;
}

void NaiveUniverse::updateNeighbourCount(uint8_t *neighbour_count) {
  size_t w = width, h = height;

  for (size_t l = 0; l < h; ++l) {
    for (size_t c = 0; c < w; ++c) {
      if (get(Coord(c, l))) {
        size_t count = 0;

        if (c > 0 && l > 0) {
          Cell *topleft_cell = neighbour_count + (l - 1) * w / 2 + c / 2;
          *topleft_cell += 1 << (((c + 1) % 2) * 4);
        }

        if (l > 0) {
          Cell *top_cell = neighbour_count + (l - 1) * w / 2 + (c + 1) / 2;
          *top_cell += 1 << ((c % 2) * 4);
        }

        if (c < w - 1 && l > 0) {
          Cell *topright_cell = neighbour_count + (l - 1) * w / 2 + c / 2 + 1;
          *topright_cell += 1 << (((c + 1) % 2) * 4);
        }

        if (c > 0) {
          Cell *left_cell = neighbour_count + l * w / 2 + c / 2;
          *left_cell += 1 << (((c + 1) % 2) * 4);
        }

        if (c < w - 1) {
          Cell *right_cell = neighbour_count + l * w / 2 + c / 2 + 1;
          *right_cell += 1 << (((c + 1) % 2) * 4);
        }

        if (c > 0 && l < h - 1) {
          Cell *bottomleft_cell = neighbour_count + (l + 1) * w / 2 + c / 2;
          *bottomleft_cell += 1 << (((c + 1) % 2) * 4);
        }

        if (l < h - 1) {
          Cell *bottom_cell = neighbour_count + (l + 1) * w / 2 + (c + 1) / 2;
          *bottom_cell += 1 << ((c % 2) * 4);
        }

        if (c < w - 1 && l < h - 1) {
          Cell *bottomright_cell =
              neighbour_count + (l + 1) * w / 2 + c / 2 + 1;
          *bottomright_cell += 1 << (((c + 1) % 2) * 4);
        }
      }
    }
  }
}
