#include <cstddef>
#include <logic/CellState.h>
#include <logic/RLE/RLERule.h>

enum ReadingState { B_SEARCH, B_VALUES, S_SEARCH, S_VALUES };

QString RLERule::to_qstring() const {
  QString str;

  str.append("B");
  for (size_t index = 0; index < 9; ++index) {
    if (transition[0][index]) {
      str.push_back(std::to_string(index).c_str());
    }
  }

  str.append("/S");
  for (size_t index = 0; index < 9; ++index) {
    if (transition[1][index]) {
      str.push_back(std::to_string(index).c_str());
    }
  }

  return str;
}

RLERule::RLERule(QString rule_string) {
  ReadingState state = B_SEARCH;
  for (size_t cursor = 0; cursor < rule_string.length(); ++cursor) {
    QChar c = rule_string.at(cursor);
    switch (state) {
    case B_SEARCH:
      if (c == 'B')
        state = B_VALUES;
      else
        throw "RLERule strings should start with B";
      break;

    case B_VALUES:
      if ('0' < c && c < '8') {
        size_t index = c.toLatin1() - '0';
        transition[0][index] = 1;
      } else if (c == '/') {
        state = S_SEARCH;
      } else {
        throw "Birth values should be between 0 and 8";
      }
      break;

    case S_SEARCH:
      if (c == 'S')
        state = S_VALUES;
      else
        throw "RLERule strings should have S after \'/\'";
      break;

    case S_VALUES:
      if ('0' < c && c < '8') {
        size_t index = c.toLatin1() - '0';
        transition[1][index] = 1;
      } else {
        throw "Survival values should be between 0 and 8";
      }
      break;
    }
  }
  if (state != S_VALUES)
    throw "Incomplete rule string";
}

CellState RLERule::apply(CellState state, size_t neighbour_count) const {
  return transition[state][neighbour_count];
}
