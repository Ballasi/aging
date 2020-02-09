#ifndef MACROCELL_H
#define MACROCELL_H

#include <functional>

#include <cstddef>

union Quadrant;

class MacroCell {
public:
  MacroCell(Quadrant *nw, Quadrant *ne,
            Quadrant *sw, Quadrant *se);

  bool operator==(const MacroCell other) const;
  friend std::hash<MacroCell>;
  friend Quadrant;

  Quadrant *result;
  Quadrant *nw, *ne;
  Quadrant *sw, *se;
};


// Defining default hashers
namespace std {
  template <>
  struct hash<MacroCell> {
    size_t operator()(const MacroCell &macrocell) const {
      hash<Quadrant*> hasher;
      return hasher(macrocell.nw) ^ hasher(macrocell.ne) ^
        hasher(macrocell.sw) ^ hasher(macrocell.se);
    }
  };
}

#endif // MACROCELL_H
