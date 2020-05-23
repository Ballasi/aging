#ifndef LIFE_UNIVERSE_H_
#define LIFE_UNIVERSE_H_

#include <cstddef>
#include <cstdint>
#include <cstring>

#include "model/CellState.h"
#include "model/Universe.h"
#include "model/Vec2.h"
#include "model/Rect.h"

/*!
 * \file LifeUniverse.h
 * \brief The universe using the Life algorithm
 * \author aging-team
 */

/*!
 * \class LifeUniverse
 * \brief The universe using the Life algorithm
 *
 * The classical game of life simulating universe.
 * With limited size, this CPU based simulation assures
 * constant performance even on the mose chaotic scenarios.
 */
class LifeUniverse: public Universe {
public:
  // Constructors

	/*!
   * \brief Constructor
   *
   * Base constructor of the universe given a certain size
   *
   * \param size: the desired size of the universe
   */
  explicit LifeUniverse(const Vec2 &size);

	/*!
   * \brief Constructor
   *
   * Constructor of the universe given a certain file (.rle)
   *
   * \param file_path: the path to the file
   * \warning As of right not, it throws "Unimplemented". Do not use.
   */
  explicit LifeUniverse(QString file_path);
  ~LifeUniverse();

  // Universe interface
  void update() override;
  Rect &bounds() override;
  const BigInt &generation() const override;
  const BigInt &step_size() const override;

  // Aptitudes
  /*!
   * \brief Ability to set a cell state
   * \return The boolean true
   */
  const bool can_set() const override { return true; }
  void set(const Vec2 &target, CellState state) override;

  /*!
   * \brief Ability to get a cell state
   * \return The boolean true
   */
  const bool can_get() const override { return true; }
  const CellState get(const Vec2 &coord) const override;

private:
  // getters
  const bool _get(size_t x,  size_t y) const;
  void _set(size_t x, size_t y, bool state);

  // Universe interface variables
  Rect _bound_rect;
  BigInt _generation_count;
  const BigInt _step_size = 1;

  // Inner workings
  size_t _width;
  size_t _height;

  uint8_t *_cell_blocks;

  size_t _length_in_bytes;


  // Internal upage
  void _update_neighbour_count(uint8_t *neighbour_count);
};

#endif // LIFE_UNIVERSE_H_
