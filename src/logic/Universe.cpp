#include "Universe.hpp"
#include "LifeFile.hpp"

Universe::Universe(Rect bounds) : bounds(bounds) {}

// This is just an alias of the above
Universe::Universe(Coord size) : Universe({Coord(), size}) {}

Universe::Universe(LifeFile life_file, Coord top_left) {}

Rect Universe::get_bounds() const { return bounds; }

BigInt Universe::get_generation() const { return generation_count; }
