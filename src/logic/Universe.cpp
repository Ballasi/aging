#include <logic/LifeFile.h>
#include <logic/Universe.h>

Universe::Universe(Rect bounds) : bounds(bounds), generation_count(0) {}

// This is just an alias of the above
Universe::Universe(Coord size) : Universe({Coord(), size}) {}

Universe::Universe(QString filename, Coord top_left) : generation_count(0) {}

Universe::Universe(size_t top_level, Coord top_left) : generation_count(0) , step_size(top_level - 2)  {}

Universe::~Universe() {}

Rect Universe::get_bounds() const { return bounds; }

BigInt Universe::get_generation() const { return generation_count; }

size_t Universe::get_step_size() {return step_size;}