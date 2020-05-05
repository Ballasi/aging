#ifndef RULE_HPP
#define RULE_HPP

#include <model/CellState.h>
#include <cstddef>
#include <QString>
#include <vector>

/*
  Describes a classical rule for a simple cellular automaton,
  where only initial state and the number of neighbour is used.

  WARNING:
  At the momment the rule only supports up to:
  - 2 states
  - 8 neighbours
*/
class Rule {
public:
  /*
    Default constructor creates the game of life
  */
  Rule();

  /*
    Rule copying
  */
  explicit Rule(const Rule& rule) = default;

  /*
    Constructs a rule from a rule string in the B/S format.
    Other formats are not yet supported.

    Exception:
    If `rule_string` is invalid an std::invalid_argument is thrown.
  */
  explicit Rule(QString rule_string);

  /*
    Creates a "blank" rule where every situation leads to
    the default cell state.
  */
  Rule(size_t state_count, size_t neighbour_count);

  /*
    Returns the new state of a cell of state `state` with `neighbour` neighbours
    obtained when the rule is applied.
  */
  CellState apply(CellState state, size_t neighbour) const;

  /*
    Sets the result of the rule when applied to a cell
    of state `state` and with `neighbour` neighbours.
  */
  void set(CellState state, size_t neighbour, CellState result);

  /*
    Clears the rule so that every situation leads to the default state.
   */
  void clear(CellState state = 0);

  /*
    Returns the number of states handled by the rule.
  */
  size_t state_count();

  /*
    Returns the number of neighbour count handled by the rule.
  */
  size_t neighbour_count();

  /*
    Returns a the rule string using the B/S rule format.

    Exception:
    If this rule is not about a 2 state automaton or
    uses a neighbourhood of more than 9 neighbours,
    a std::domain_error is raised.
  */
  QString to_rule_string();

private:
  /*
    Maximum state value handled by the rule.
  */
  size_t _state_count;

  /*
    Maximum number of neighbour handled by the rule.
  */
  size_t _neighbour_count;

  /*
    Holdes the transition from every state of with every neighbour count.
    Each row describes state,
    each collumn a neighbour count (aka a serounding).
  */
  std::vector<CellState> _transition_table;

 /*
    Shift is used to take advantage of alignment when accessing the _transition_table.
    It is the number of bits required to write neighbour_count.
  */
  size_t _shift;
};

#endif // RULE_HPP
