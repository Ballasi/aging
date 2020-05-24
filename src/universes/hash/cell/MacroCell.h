#ifndef MACROCELL_H
#define MACROCELL_H

/*!
 * \file MacroCell.h
 * \brief A group of 4 pieces of a universe
 */

#include <functional>
#include <cstddef>

namespace cell {
union Quadrant;
/*!
 * \class MacroCell
 */
class MacroCell {
public:
  /*!
   * \brief Constructor
   *
   * \param nw: north-west quadrant
   * \param ne: north-east quadrant
   * \param sw: south-west quadrant
   * \param se: south-east quadrant
   */
  MacroCell(Quadrant *nw, Quadrant *ne, Quadrant *sw, Quadrant *se);

  /*!
   * \brief Equality operator
   *
   * \param other: MacroCell to compare with
   */
  bool operator==(const MacroCell other) const;

  /*!
   * \brief Handy indexed getter
   *
   * macrocell[0] == macrocell.nw
   * macrocell[1] == macrocell.ne
   * macrocell[2] == macrocell.sw
   * macrocell[3] == macrocell.se
   *
   * \param index: index of the quadrant
   */
  Quadrant *operator[](const size_t index) const;

  friend std::hash<MacroCell>;
  friend Quadrant;

  // Metadata
  bool result_advances_in_time;

  Quadrant *result;
  Quadrant *nw, *ne;
  Quadrant *sw, *se;
};
} // namespace cell

// Defining default hashers
namespace std {
using cell::MacroCell;
using cell::Quadrant;
/*!
 * \struct hasher<MacroCell>
 */
template <> struct hash<MacroCell> {

  /*!
   * \brief Call operator
   *
   * \param macrocell: macrocell to hash
   */
  size_t operator()(const MacroCell &macrocell) const {
    return hasher(macrocell.nw) ^ hasher(macrocell.ne) ^ hasher(macrocell.sw) ^
           hasher(macrocell.se);
  }
  hash<Quadrant *> hasher;
};
} // namespace std
#endif // MACROCELL_H
