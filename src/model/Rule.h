#ifndef RULE_HPP
#define RULE_HPP

#include "CellState.h"
#include <cstddef>

class Rule {
public:
  Rule(size_t state_count, size_t max_neighbour_count);
  ~Rule();

  void clear(CellState state = 0);
  void set(CellState state, size_t neighbour_count, CellState result);
  CellState apply(CellState state, size_t neighbour_count) const;

private:
  size_t state_count;
  size_t max_neighbour_count;

  size_t shift;

  CellState *lookup_table;
};

#endif // RULE_HPP
