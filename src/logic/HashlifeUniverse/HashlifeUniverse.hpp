#ifndef HASHLIFE_UNIVERSE_HPP
#define HASHLIFE_UNIVERSE_HPP

#include <QFile>
#include <QString>
// #include <iterator>
#include <unordered_set>
#include <vector>

#include <logic/Universe.hpp>

#include <logic/Coord.hpp>
#include <logic/Rect.hpp>
#include <logic/HashlifeUniverse/cell/MiniCell.hpp>
#include <logic/HashlifeUniverse/cell/MacroCell.hpp>
#include <logic/HashlifeUniverse/cell/Quadrant.hpp>

using namespace std;

class HashlifeUniverse: public Universe {
public:
  HashlifeUniverse(size_t top_level, Coord top_left = Coord());
  HashlifeUniverse(QString filename, Coord top_left = Coord());

  void step();
  const CellState get(Coord coord) const;
  void set(Coord target, CellState state);
  void debug();
  

  void get_cell_in_bounds(Rect bounds, vector<Coord> &coords) const;
/*
  class Iterator;
  Iterator iter(Rect bounds);
*/


  Coord get_top_left();
  size_t get_top_level();


  void grid(int *L, int width, Quadrant* r, int level, int x, int y);
  void print_grid(Quadrant* r, size_t level);
private:
  size_t top_level;
  Coord top_left;
  MacroCell *root;

  vector<unordered_set<MacroCell>> macrocell_sets;
  unordered_set<MiniCell> minicells;
  vector<Quadrant*> zeros;


  // File loading
  Coord read_rle_size(QFile &file);
  void read_rle_data(QFile &file, Coord boundingbox);

  // TODO find good name
  void assert_handles(size_t asserted_level);

  // Step helpers
  void crown();
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


  void get_cell_in_bounds_rec(Rect bounds, vector<Coord> &coords, size_t current_level, Quadrant* current_cell, Coord current_coord) const;
};

#endif // HASHLIFE_UNIVERSE_HPP
