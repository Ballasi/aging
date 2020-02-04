#ifndef MACROCELL_H
#define MACROCELL_H

#include <iostream>

#include <cstdlib>
#include <cstddef>
#include <string>



typedef int AtomicCell;
union Quadrant;

class MiniCell {
public:
  MiniCell();
  MiniCell(AtomicCell nw, AtomicCell ne, AtomicCell sw, AtomicCell se);

  MiniCell *next;
  AtomicCell nw, ne;
  AtomicCell sw, se;
};

class MacroCell {
public:
  MacroCell(Quadrant *nw, Quadrant *ne, Quadrant *sw, Quadrant *se);

  MacroCell *next;
  Quadrant *result;
  Quadrant *nw, *ne;
  Quadrant *sw, *se;
};

union Quadrant {
  void debug_rec(size_t level, std::string indentation);



public :
  MiniCell minicell;
  MacroCell macrocell;
  static Quadrant *generate(size_t level);
  void debug(size_t level);
};


#endif // MACROCELL_H
