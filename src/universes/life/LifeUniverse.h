#ifndef LIFE_UNIVERSE_H_
#define LIFE_UNIVERSE_H_

#include <bits/stdint-uintn.h>
#include <cstddef>
#include <cstdint>
#include <cstring>

#include "model/CellState.h"
#include "model/Universe.h"
#include "model/Vec2.h"
#include "model/Rect.h"

/*
  Cells are memorized in a 8 bit type format.
  Each bits of this byte corresponds to the living state of the cell.
 */

class LifeUniverse: public Universe {
public:
  explicit LifeUniverse(const Vec2 &size);
  ~LifeUniverse();

  void update() override;

  const Rect &bounds() const override;
  const BigInt &generation() const override;
  const BigInt &step_size() const override;

  // Aptitudes
  const bool can_set() const override { return true; }
  void set(const Vec2 &target, CellState state) override;

  const bool can_get() const override { return true; }
  const CellState get(const Vec2 &coord) const override;

private:
  const bool _get(size_t x,  size_t y) const;
  void _set(size_t x, size_t y, bool state);

  Rect _bound_rect;
  const BigInt _step_size = 1;
  BigInt _generation_count;

  uint8_t *_cell_blocks;

  size_t _width;
  size_t _height;

  size_t _length_in_bytes;


  // Internal upage
  void _update_neighbour_count(uint8_t *neighbour_count);
};

#endif // LIFE_UNIVERSE_H_
