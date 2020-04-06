#ifndef RULE_HPP
#define RULE_HPP

#include "CellState.h"
#include <cstddef>

class Rule {
  virtual CellState apply(CellState state, size_t neighbout_count) const;
};

#endif // RULE_HPP
