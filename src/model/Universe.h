#ifndef UNIVERSE_H_
#define UNIVERSE_H_

#include "Vec2.h"
#include "Rect.h"
#include "CellState.h"

#include <QString>

#include <vector>

/*
  Universe interface describes un number of methods
  that can be use by the UI to interact with your universe.
 */
class Universe {
public:
  /* Required methods */

  /*
    Updates the content of the universe.
  */
  virtual void update() = 0;

  /*
    Returns the bounds of smiluation of the universe.
    These are not the smallest one that hold the living cells.
  */
  virtual Rect &bounds() = 0;

  /*
    Returns the number of generations that have been simulated
    since the beginning of the simulation.
  */
  virtual const BigInt &generation() const = 0;

  /*
    Returns the number of generations simulated every
    time the update method is called.
  */
  virtual const BigInt &step_size() const = 0;


  /* Aptitudes */

  /*
    Aptitudes are methods that have to be overriden to be used
    if no default method is provided.
    They all come with a method prefixed by `can_` describing
    if the given method can be used.
    It set to zero if no default method is provided
   */

  /*
    Sets the cell at the coord `coord` to the value `cell_state`.
   */
  virtual void set(const Vec2 &coord, CellState cell_state);
  virtual const bool can_set() const;

  /*
    Returns the value of the cell of coord `coord`
   */
  virtual const CellState get(const Vec2 &coord) const;
  virtual const bool can_get() const;

  /*
    Sets every cell in the universe to the default CellState value.
   */
  virtual void clear();
  virtual const bool can_clear() const;

  /*
    Sets the size of simulation steps.
   */
  virtual void set_step_size(const size_t step_size);
  virtual const bool can_set_step_size() const;

  /*
   Sets hyper speed mode.
  */
  virtual void set_hyperspeed(const bool state);
  virtual const bool can_set_hyperspeed() const;


  /*
    Changes the universe shape by giving it new bounds.
    Every new cell is set to the default CellState.
    Cells out of the new bounds are destroyed.
   */
  virtual void reshape(Rect bounds);
  virtual const bool can_reshape() const;
};

enum class UniverseType {Life, Hashlife};

#endif // UNIVERSE_H_
