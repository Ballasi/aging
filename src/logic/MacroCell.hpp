#ifndef MACROCELL_H
#define MACROCELL_H

#include <iostream>

#include <cstdlib>
#include <cstddef>


typedef int AtomicCell;
union Quadrant;

class MiniCell {
public:
  MiniCell *next;
  AtomicCell nw, ne;
  AtomicCell sw, se;

  static Quadrant* create(AtomicCell nw, AtomicCell ne, AtomicCell sw, AtomicCell se);
};

class MacroCell {
public:
  MacroCell *next;
  Quadrant *result;
  Quadrant *nw, *ne;
  Quadrant *sw, *se;

  static Quadrant* create(Quadrant *nw, Quadrant *ne, Quadrant *sw, Quadrant *se);
};

union Quadrant {
  MiniCell minicell;
  MacroCell macrocell;

  static Quadrant *generate(size_t level);
  void debug(size_t level);
};


#endif // MACROCELL_H
