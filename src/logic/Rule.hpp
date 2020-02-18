#ifndef RULE_HPP
#define RULE_HPP

#include "CellState.hpp"

#include <QString>

class Rule {
public:
  Rule() = default;
  Rule(QString rule_string);

  QString to_qstring() const;

  CellState apply(CellState state, size_t neighbour_count) const;


private:
  CellState transition[2][9];
};

#endif // RULE_HPP
