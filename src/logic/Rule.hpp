#ifndef RULE_HPP
#define RULE_HPP

#include "CellState.hpp"
#include <cstddef>

class Rule {
  virtual CellState apply(CellState state, size_t neighbout_count) const;
};

#endif // RULE_HPP
