#ifndef QUADRANT_HPP
#define QUADRANT_HPP

#include <iostream>
#include <random>
#include <string>

#include "MiniCell.hpp"
#include "MacroCell.hpp"

union Quadrant {
public:
  MiniCell minicell;
  MacroCell macrocell;
  void debug(size_t level);
  static Quadrant *generate(size_t level);
  static Quadrant *generate_random(size_t level, AtomicCell max_value);

private:
  void debug_rec(size_t level, std::string indentation);
  static Quadrant *
  raw_generate_random(size_t level, std::default_random_engine random_engine,
                      std::uniform_int_distribution<AtomicCell> range);
};

#endif // QUADRANT_HPP
