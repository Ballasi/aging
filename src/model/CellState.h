#ifndef CELL_STATE_H_
#define CELL_STATE_H_

#include <cstddef>

class CellState {
public:
  CellState() = default;
  CellState(size_t id): id(id) {}

  size_t id;
};

#endif // CELL_STATE_H_
