#ifndef MACROCELL_H
#define MACROCELL_H

#include <functional>

#include <cstddef>

namespace cell {
union Quadrant;
class MacroCell {
public:
  MacroCell(Quadrant *nw, Quadrant *ne, Quadrant *sw, Quadrant *se);

  bool operator==(const MacroCell other) const;
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
template <> struct hash<MacroCell> {
  size_t operator()(const MacroCell &macrocell) const {
    return hasher(macrocell.nw) ^ hasher(macrocell.ne) ^ hasher(macrocell.sw) ^
           hasher(macrocell.se);
  }
  hash<Quadrant *> hasher;
};
} // namespace std
#endif // MACROCELL_H
