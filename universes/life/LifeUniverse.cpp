#include "LifeUniverse.h"
#include <bits/stdint-uintn.h>
#include <sys/types.h>

LifeUniverse::LifeUniverse(Vec2 &size) {
  // Adding () calls constructor for every Cell in the array
  // TODO precise lenght_in_bytes using
  // (if possible) width & height
  width = size.x().get_si() + 8 - size.x().get_si() % 8;
  height = size.y().get_si();
  length_in_bytes = (size.x().get_si() + 7) * size.y().get_si() / 8;
  Vec2 pos;
  bound_rect = Rect(pos, size);
  cell_blocks = new uint8_t[length_in_bytes]();
}

LifeUniverse::~LifeUniverse() { delete[] cell_blocks; }

void LifeUniverse::update() {
  uint8_t neighbour_count[(width + 1) * height / 2];
  memset(neighbour_count, 0, (width + 1) * height / 2);
  updateNeighbourCount(neighbour_count);

  size_t w = width, h = height;

  for (size_t l = 0; l < h; ++l) {
    for (size_t c = 0; c < w; ++c) {
      size_t neighbours =
          *(neighbour_count + l * width / 2 + (c + 1) / 2) >> ((c % 2) * 4) &
          0b1111;
      if (get(c, l)) {
        if ((neighbours != 2) && (neighbours != 3))
          set(c, l, 0);
      } else {
        if (neighbours == 3)
          set(c, l, 0);
      }
    }
  }

  ++generation_count;
}

const Rect &LifeUniverse::bounds() const {
  return bound_rect;
}
const BigInt &LifeUniverse::generation() const {
  return generation_count;
}

// Aptitudes
void LifeUniverse::set(Vec2 &target, CellState state) {
  set(target.x().get_ui(), target.y().get_ui(), state.id);
}
const CellState LifeUniverse::get(Vec2 &target) const {
  return get(target.x().get_ui(), target.y().get_ui());
}


// Inner workings
void LifeUniverse::set(size_t x, size_t y, bool state) {
  uint8_t *cell_ptr = cell_blocks + (y * width / 8) + (x + 7) / 8;

  if (state)
    *(cell_ptr) |= 1 << (x % 8);
  else
    *(cell_ptr) &= ~(1 << (x % 8));
}
const bool LifeUniverse::get(size_t x, size_t y) const {
  return (*(cell_blocks + y * width / 8 + (x + 7) / 8) >> (x % 8)) & 0b01;
}
void LifeUniverse::updateNeighbourCount(uint8_t *neighbour_count) {
  size_t w = width, h = height;

  for (size_t l = 0; l < h; ++l) {
    for (size_t c = 0; c < w; ++c) {
      if (get(c, l)) {
        size_t count = 0;

        if (c > 0 && l > 0) {
          uint8_t *topleft_cell = neighbour_count + (l - 1) * w / 2 + c / 2;
          *topleft_cell += 1 << (((c + 1) % 2) * 4);
        }

        if (l > 0) {
          uint8_t *top_cell = neighbour_count + (l - 1) * w / 2 + (c + 1) / 2;
          *top_cell += 1 << ((c % 2) * 4);
        }

        if (c < w - 1 && l > 0) {
          uint8_t *topright_cell = neighbour_count + (l - 1) * w / 2 + c / 2 + 1;
          *topright_cell += 1 << (((c + 1) % 2) * 4);
        }

        if (c > 0) {
          uint8_t *left_cell = neighbour_count + l * w / 2 + c / 2;
          *left_cell += 1 << (((c + 1) % 2) * 4);
        }

        if (c < w - 1) {
          uint8_t *right_cell = neighbour_count + l * w / 2 + c / 2 + 1;
          *right_cell += 1 << (((c + 1) % 2) * 4);
        }

        if (c > 0 && l < h - 1) {
          uint8_t *bottomleft_cell = neighbour_count + (l + 1) * w / 2 + c / 2;
          *bottomleft_cell += 1 << (((c + 1) % 2) * 4);
        }

        if (l < h - 1) {
          uint8_t *bottom_cell = neighbour_count + (l + 1) * w / 2 + (c + 1) / 2;
          *bottom_cell += 1 << ((c % 2) * 4);
        }

        if (c < w - 1 && l < h - 1) {
          uint8_t *bottomright_cell =
              neighbour_count + (l + 1) * w / 2 + c / 2 + 1;
          *bottomright_cell += 1 << (((c + 1) % 2) * 4);
        }
      }
    }
  }
}


