#ifndef MACROCELL_H
#define MACROCELL_H

#include <iostream>

#include <cstdlib>
#include <cstddef>
#include <string>
#include <random>


typedef int AtomicCell;
union Quadrant;

class MiniCell {
public:
  MiniCell() = default;
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
public :
  MiniCell minicell;
  MacroCell macrocell;
  void debug(size_t level);
  static Quadrant *generate(size_t level);
  static Quadrant *generate_random(size_t level, AtomicCell max_value);

private:
  void debug_rec(size_t level, std::string indentation);
  static Quadrant *raw_generate_random(size_t level,
                                       std::default_random_engine random_engine,
                                       std::uniform_int_distribution<AtomicCell> range);
};


#endif // MACROCELL_H
