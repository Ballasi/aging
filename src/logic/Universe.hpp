#ifndef UNIVERSE_HPP
#define UNIVERSE_HPP

#include <unordered_set>
#include <vector>

#include "cell.hpp"
#include "Coord.hpp"

using namespace std;

class Universe {
public:
  Universe(size_t top_level);
  Universe(Coord top_left, size_t top_level);
  ~Universe();

  size_t step();
  const CellState get(Coord coord) const;
  void set(Coord coord, CellState state);

private:
  size_t top_level;
  Coord top_left;
  Quadrant *root;

  vector<unordered_set<MacroCell>> macrocell_sets;
  unordered_set<MiniCell> minicells;
  vector<Quadrant*> zeros;

  // Finding
  CellState *find(Coord) const;
  CellState *find_path(Coord coord, vector<Quadrant*> &path) const;

  // Minicells
  MiniCell *minicell();
  MiniCell *minicell(const CellState nw, const CellState ne,
                     const CellState sw, const CellState se);

  // Macrocells
  MacroCell *macrocell(size_t level);
  MacroCell *macrocell(size_t level,
                       const Quadrant *nw, const Quadrant *ne,
                       const Quadrant *sw, const Quadrant *se);

  // Quadrant
  Quadrant *quadrant(size_t level);
};

#endif // UNIVERSE_HPP
