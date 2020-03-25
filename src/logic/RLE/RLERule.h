#ifndef RLE_RULE_HPP
#define RLE_RULE_HPP

#include <logic/CellState.h>

#include <QString>

class RLERule {
public:
  RLERule() = default;
  explicit RLERule(QString rule_string);

  QString to_qstring() const;

  CellState apply(CellState state, size_t neighbour_count) const;

private:
  CellState transition[2][9];
};

#endif // RLE_RULE_HPP
