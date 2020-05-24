#ifndef MINICELL_HPP
#define MINICELL_HPP

/*!
 * \file MiniCell.h
 * \brief HashUniverse's MiniCell
 */

#include <functional>
#include <model/CellState.h>

namespace cell {
union Quadrant;
/*!
 * \class MiniCell
 * \brief A 2x2 cell grid
 */
class MiniCell {
public:
  /*!
   * \brief Default constructor
   */
  MiniCell() = default;

  /*!
   * \brief Constructor
   *
   * \param nw: state of north-west
   * \param ne: state of north-east
   * \param sw: state of south-west
   * \param se: state of south-east
   */
  MiniCell(CellState nw, CellState ne, CellState sw, CellState se);

  /*!
   * \brief Equality operator
   *
   * MiniCells are equal if they have the
   * exact same sub cell states.
   *
   * \param other: MiniCell to compare with
   */
  bool operator==(const MiniCell other) const;

  /*!
   * \brief Handy indexed getter
   *
   * minicell[0] == minicell.nw
   * minicell[1] == minicell.ne
   * minicell[2] == minicell.sw
   * minicell[3] == minicell.se
   *
   * \param index: index of the cell state
   */
  CellState operator[](const size_t index) const;

  friend std::hash<MiniCell>;
  friend Quadrant;

  CellState nw, ne;
  CellState sw, se;
};
} // namespace cell

namespace std {
using cell::MiniCell;
/*!
 * \struct hasher<MiniCell>
 */
template <> struct hash<MiniCell> {
  /*!
   * \brief Constructor
   */
  hash() = default;

  /*!
   * \brief Call operator
   *
   * \param minicell: minicell to hash
   */
  size_t operator()(const MiniCell &minicell) const {
    return hasher(minicell.nw) ^ hasher(minicell.ne)
         ^ hasher(minicell.sw) ^ hasher(minicell.se);
  }

  hash<CellState> hasher;
};
} // namespace std

#endif // MINICELL_HPP
