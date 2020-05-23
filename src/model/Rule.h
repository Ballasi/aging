#ifndef RULE_HPP
#define RULE_HPP

#include <QString>
#include <cstddef>
#include <model/CellState.h>
#include <vector>

/*!
 * \file Rule.h
 * \brief Describes a classical rule for a simple cellular automaton
 * \author aging-team
 */
class Rule {
  /*!
   * \class Rule
   * \brief Describes a classical rule for a simple cellular automaton
   *
   * The rule descriptor uses only an initial state and the number of
   * neighbours. The rule only supports up to 2 states and 8 neighbours.
   */
public:
  /*!
   * \brief Constructor
   *
   * Default constructor of Rule. It automatically matches the rules of
   * Conway's Game of Life.
   */
  Rule();

  /*!
   * \brief Rule copying
   *
   * A constructor that allows to duplicate a Rule.
   *
   * \param rule : a reference to the rule you want to duplicate
   */
  explicit Rule(const Rule &rule) = default;

  /*!
   * \brief Rule constructor from a rule string
   *
   * Constructs a rule from a rule string in the B/S format.
   * Other formats are not yet supported.
   *
   * Exception: If `rule_string` is invalid an std::invalid_argument is thrown.
   *
   * \param rule_string : a string that describes a rule in a B/S format
   */
  explicit Rule(QString rule_string);

  /*!
   * \brief Blank rule constructor
   *
   * Creates a "blank" rule where every situation leads to
   * the default cell state.
   *
   * \param state_count : the number of states the rule can handle
   * \param neighbour_count : the number of neighbours the rule can handle
   */
  Rule(size_t state_count, size_t neighbour_count);

  /*!
   * \brief Rule applier on a certain state
   *
   * Returns the new state of a cell of state `state` with `neighbour`
   * neighbours obtained when the rule is applied.
   *
   * \param state: the initial cell state
   * \param neighbour: the number of neighbour the state has
   */
  CellState apply(CellState state, size_t neighbour) const;

  /*!
   * \brief Rule setter for a certain state
   *
   * Sets the result of the rule when applied to a cell of state `state` and
   * with `neighbour` neighbours.
   *
   * \param state: the initial cell state
   * \param neighbour: the number of neighbour the state has
   * \param result: the state you are supposed to get when applying the rule
   * with the given setup
   */
  void set(CellState state, size_t neighbour, CellState result);

  /*!
   * \brief Rule clearer
   *
   * Clears the rule so that every situation leads to the default state.
   */
  void clear(CellState state = 0);

  /*!
   * \brief Returns the number of states handled by the rule.
   */
  size_t state_count();

  /*!
   * \brief Returns the number of neighbour count handled by the rule.
   */
  size_t neighbour_count();

  /*!
   * \brief Returns a the rule string using the B/S rule format.
   *
   * Exception:
   * If this rule is not about a 2 state automaton or
   * uses a neighbourhood of more than 9 neighbours,
   * a std::domain_error is raised.
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
     Shift is used to take advantage of alignment when accessing the
     _transition_table. It is the number of bits required to write
     neighbour_count.
   */
  size_t _shift;
};

#endif // RULE_HPP
