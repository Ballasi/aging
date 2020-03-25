#ifndef LIFE_UNIVERSE_H_
#define LIFE_UNIVERSE_H_

#include <bits/stdint-uintn.h>
#include <cstddef>
#include <cstdint>
#include <cstring>

#include "model/CellState.hpp"
#include "model/Universe.hpp"

#include "model/Rect.hpp"

/*
  Cells are memorized in a 8 bit type format.
  Each bits of this byte corresponds to the living state of the cell.
 */

class LifeUniverse: public Universe
{
public:
  LifeUniverse(Vec2 &size);
  ~LifeUniverse();

  void update() override;

  const Rect &bounds() const override;
  const BigInt &generation() const override;
  const BigInt &step_size() const override;

  // Aptitudes
  const bool can_set() const override { return true; }
  void set(Vec2 &target, CellState state) override;

  const bool can_get() const override { return true; }
  const CellState get(Vec2 &coord) const override;

private:
  const bool get(size_t x, size_t y) const;
  void set(size_t x, size_t y, bool state);

  Rect bound_rect;
  BigInt generation_count;

  uint8_t *cell_blocks;

  size_t width;
  size_t height;

  size_t length_in_bytes;

  // Internal upage
  void updateNeighbourCount(uint8_t *neighbour_count);
};

#endif // LIFE_UNIVERSE_H_
