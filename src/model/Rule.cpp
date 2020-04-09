#include "model/CellState.h"
#include <cstddef>
#include <cstring>
#include <model/Rule.h>

Rule::Rule(size_t state_count, size_t max_neighbour_count)
    : state_count(state_count), max_neighbour_count(max_neighbour_count) {
  lookup_table = new CellState[state_count * max_neighbour_count]();

  shift = 0;
  while (max_neighbour_count >>= 1)
    shift++;
}

Rule::~Rule() { delete lookup_table; }

void Rule::clear(CellState state) {
  for (CellState state = 0; state < state_count; ++state) {
    for (size_t neighbour_count = 0; neighbour_count <= max_neighbour_count;
         ++neighbour_count)
      set(state, neighbour_count, state);
  }
}

void Rule::set(CellState state, size_t neighbour_count, CellState result) {
  lookup_table[neighbour_count + (state << shift)] = result;
}

CellState Rule::apply(CellState state, size_t neighbour_count) const {
  return lookup_table[neighbour_count + (state << shift)];
}
