#ifndef UNIVERSE_H_
#define UNIVERSE_H_
#include "CellState.h"
#include "Rect.h"
#include "Vec2.h"

/*!
 * \file Universe.h
 * \brief The universal interface for the manipulation of universes
 * \author aging-team
 */

#include <QString>
#include <vector>

/*!
 * \class Universe
 * \brief Universal universe interface
 *
 * The universe interface describes un number of methods
 * that can be use by the UI to interact with any universe.
 *
 * Some methods a **have** to be overriden however most are optional.
 * Those which are optional are called aptitudes and are predefined to throw an
 * error. It is possible to test if a aptitude is implemented by calling the
 * `can_` prefixed function that comes with it.
 *
 * _Example:_
 * \code
 * if (some_universe->can_set_step_size()) {
 *   some_universe->set_step_size(some_step_size)
 * }
 * else {
 *   // Do something else...
 * }
 * \endcode
 */
class Universe {
public:
  /* Required methods */

  /*!
   * \brief Updates the content of the universe.
   *
   * Updates universe advancing the simulation a number of steps determined
   * by the universe. This number of steps may be modified using
   * Universe::set_step_size.
   */
  virtual void update() = 0;

  /*!
   * \brief Universe dimention getter
   *
   * \return The bounds of smiluated universe
   * \warning The bounds is not the smallest Rect that holds the living cells
   */
  virtual Rect &bounds() = 0;

  /*!
   * \brief Universe generation getter
   *
   * Here generations refers to generations that have been simulated since the
   * beginning of the execution.
   *
   * \return the number of simulated generations
   */
  virtual const BigInt &generation() const = 0;

  /*!
   * \brief Step size getter
   * \returns count of steps simulated per update
   */
  virtual const BigInt &step_size() const = 0;

  /* Aptitudes */

  /*!
   * \brief Cell state setter.
   *
   * \param coord: coord at which to set the cell state
   * \param cell_state: cell state that to which to set the destination
   */
  virtual void set(const Vec2 &coord, CellState cell_state);
  virtual const bool can_set() const;

  /*!
   * \brief Cell state getter.
   *
   * \param coord: Coordonnates to look at.
   * \return The state of the cell.
   */
  virtual const CellState get(const Vec2 &coord) const;
  virtual const bool can_get() const;

  /*
   * \brief Clears the universe.
   *
   * Sets every cell in the universe to the default CellState value.
   */
  virtual void clear();
  virtual const bool can_clear() const;

  /*!
   * \brief Step size setter
   *
   * The step size is the number of simulation steps made
   * each call to Universe::update
   *
   * \param step_size: 
   */
  virtual void set_step_size(const size_t step_size);
  virtual const bool can_set_step_size() const;

  /*
   * \brief change universe size
   *
   * Changes the universe shape by giving it new bounds.
   * Every new cell is set to the default CellState.
   * Cells out of the new bounds are destroyed.
   *
   * \param bounds: New universe bounds
   */
  virtual void reshape(const Rect& bounds);
  virtual const bool can_reshape() const;
};

/*!
 * \brief Outdated method to determine universe type
 */
enum class[[deprecated]] UniverseType { Life, Hashlife };

#endif // UNIVERSE_H_
