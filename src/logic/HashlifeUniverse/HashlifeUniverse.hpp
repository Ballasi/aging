#ifndef HASHLIFE_UNIVERSE_HPP
#define HASHLIFE_UNIVERSE_HPP

#include <QFile>
#include <QString>
#include <iterator>
#include <unordered_set>
#include <vector>

#include "../Universe.hpp"

#include "../Coord.hpp"
#include "cell/MiniCell.hpp"
#include "cell/MacroCell.hpp"
#include "cell/Quadrant.hpp"

using namespace std;

class HashlifeUniverse: public Universe {
public:
  HashlifeUniverse(size_t top_level, Coord top_left = Coord());
  HashlifeUniverse(QString filename,Coord top_left = Coord());

  void step();
  const CellState get(Coord coord) const;
  void set(Coord target, CellState state);
  void debug();

  Coord get_top_left();
  size_t get_top_level();

private:
  size_t top_level;
  Coord top_left;
  MacroCell *root;

  vector<unordered_set<MacroCell>> macrocell_sets;
  unordered_set<MiniCell> minicells;
  // TODO: Decide if we keep or not
  vector<Quadrant*> zeros;


  // File loading
  Coord read_rle_size(QFile &file);
  void read_rle_data(QFile &file, Coord boundingbox);

  // TODO find good name
  void assert_handles(size_t asserted_level);

  // Step helpers
  MacroCell* crown();
  Quadrant* result(size_t level, MacroCell* macrocell_tmp);


  // Recursive setter
  Quadrant *set_rec(Coord current, size_t level, Quadrant *cell, Coord target, CellState state);

  // Finding
  CellState *find(Coord) const;
  CellState *find_path(Coord coord, vector<Quadrant*> &path) const;

  // Minicells
  MiniCell *minicell();
  MiniCell *minicell(CellState nw, CellState ne,
                     CellState sw, CellState se);

  // Macrocells
  MacroCell *macrocell(size_t level);
  MacroCell *macrocell(size_t level,
                       Quadrant *nw, Quadrant *ne,
                       Quadrant *sw, Quadrant *se);

  // Quadrant
  Quadrant *quadrant(size_t level);
};

#endif // HASHLIFE_UNIVERSE_HPP
