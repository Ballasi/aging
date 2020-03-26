#ifndef RULE_HPP
#define RULE_HPP

#include <cstddef>
#include <logic/CellState.h>

class Rule {
  virtual CellState apply(CellState state, size_t neighbout_count) const;
};

#endif // RULE_HPP
