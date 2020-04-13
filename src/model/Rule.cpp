#include "model/CellState.h"
#include <cstddef>
#include <cstring>
#include <model/Rule.h>
#include <stdexcept>

/*
  Returns the number of bits required to write n.
 */
inline size_t required_bits(size_t n) {
  int result = 0;
  while (n >>= 1)
    result++;
  return result;
}

Rule::Rule(QString rule_string)
  : _state_count(2), _neighbour_count(9), _row_length(2),
    _transition_table(new CellState[2 * 9]()) {
  enum ReadingState { B_SEARCH, B_VALUES, S_SEARCH, S_VALUES };

  ReadingState reading_state = B_SEARCH;
  for (QChar c: rule_string) {
    switch (reading_state) {
    case B_SEARCH:
      if (c == 'B')
        reading_state = B_VALUES;
      else
        throw std::invalid_argument("rule_string should start with B");
      break;

    case B_VALUES:
      if ('0' <= c && c <= '8') {
        size_t neighbour = c.toLatin1() - '0';
        set(neighbour, 0, 1);
      } else if (c == '/') {
        reading_state = S_SEARCH;
      } else {
        throw std::invalid_argument("Birth values should be between 0 and 8");
      }
      break;

    case S_SEARCH:
      if (c == 'S')
        reading_state = S_VALUES;
      else
        throw std::invalid_argument("rule_string should have S after \'/\'");
      break;

    case S_VALUES:
      if ('0' <= c && c <= '8') {
        size_t neighbour = c.toLatin1() - '0';
        set(neighbour, 1, 1);
      } else {
        throw std::invalid_argument("Survival values should be between 0 and 8");
      }
      break;
    }
  }
  if (reading_state != S_VALUES)
    throw std::invalid_argument("Incomplete rule string");
}

Rule::Rule(size_t state_count, size_t neighbour_count)
    : _state_count(state_count), _neighbour_count(neighbour_count),
      _row_length(required_bits(neighbour_count)) {
  _transition_table = new CellState[_row_length * neighbour_count]();
}

Rule::~Rule() { delete[] _transition_table; }


// Rule usage

inline CellState Rule::apply(CellState state, size_t neighbour_count) const {
  return _transition_table[neighbour_count + (state << _row_length)];
}


// Rule editing

inline void Rule::set(CellState state, size_t neighbour, CellState result) {
  _transition_table[neighbour + (state << _row_length)] = result;
}

inline void Rule::clear(CellState state) {
  for (CellState state = 0; state < _state_count; ++state) {
    for (size_t neighbour = 0; neighbour <= _neighbour_count; ++neighbour)
      set(state, neighbour, state);
  }
}


// Getters

inline size_t Rule::state_count() { return _state_count; }

inline size_t Rule::neighbour_count() { return _neighbour_count; }

QString Rule::to_rule_string() {
  // Exceptions
  if (_state_count != 2) {
    throw std::domain_error("Rule is not for 2 cellular automaton.");
  }
  if (_neighbour_count > 9) {
    throw std::domain_error("Rule uses neighbourhoods bigger than 9.");
  }

  QString result;
  // Birth
  result.append("B");
  for (size_t neighbour = 0; neighbour < _neighbour_count; ++neighbour) {
    if (apply(0, neighbour)) {
      result.push_back(std::to_string(neighbour).c_str());
    }
  }

  // Survival
  result.append("/S");
  for (size_t neighbour = 0; neighbour < _neighbour_count; ++neighbour) {
    if (apply(1, neighbour)) {
      result.push_back(std::to_string(neighbour).c_str());
    }
  }

  return result;
}

