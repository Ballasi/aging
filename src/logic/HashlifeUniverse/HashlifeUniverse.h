#ifndef HASHLIFE_UNIVERSE_HPP
#define HASHLIFE_UNIVERSE_HPP

#include <QFile>
#include <QString>
// #include <iterator>
#include <unordered_set>
#include <vector>

#include <logic/Universe.h>

#include <logic/Coord.h>
#include <logic/HashlifeUniverse/cell/MacroCell.h>
#include <logic/HashlifeUniverse/cell/MiniCell.h>
#include <logic/HashlifeUniverse/cell/Quadrant.h>
#include <logic/Rect.h>

class HashlifeUniverse : public Universe {
public:
  explicit HashlifeUniverse(size_t top_level, Coord top_left = Coord());
  explicit HashlifeUniverse(QString filename, Coord top_left = Coord());

  void step();
  const CellState get(Coord coord) const;
  void set(Coord target, CellState state);
  void debug();

  void get_cell_in_bounds(Rect bounds,
          std::vector<Coord> *coords, size_t min_level) const;
  /*
    class Iterator;
    Iterator iter(Rect bounds);
  */

  Coord get_top_left();
  size_t get_top_level();

  void set_step_size(size_t new_step_size);
  void set_step_size_maximized(bool is_maximized);
  void set_hyperspeed(bool hyperspeed_activated);

  void grid(int *L, int width, Quadrant *r, int level, int x, int y);
  void print_grid(Quadrant *r, size_t level);

  Rect get_pattern_bounding_box();

private:
  size_t top_level;
  Coord top_left;
  MacroCell *root;
  bool step_size_maximized;
  bool hyperspeed;

  std::vector<std::unordered_set<MacroCell>> macrocell_sets;
  std::unordered_set<MiniCell> minicells;
  std::vector<Quadrant *> zeros;

  // File loading
  void build_from_rle(QFile *file);
  void build_from_mc(QFile *file);
  Coord read_rle_size(QFile *file);
  void read_rle_data(QFile *file, Coord boundingbox);

  // TODO(chancyleg): find good name
  void assert_handles(size_t asserted_level);

  // Step helpers
  void crown();
  Quadrant *result(size_t level, MacroCell *macrocell_tmp);

  // Recursive setter
  Quadrant *set_rec(Coord current, size_t level, Quadrant *cell, Coord target,
                    CellState state);

  // Finding
  CellState *find(Coord) const;
  CellState *find_path(Coord coord, std::vector<Quadrant *> *path) const;

  // Minicells
  MiniCell *minicell();
  MiniCell *minicell(CellState nw, CellState ne, CellState sw, CellState se);

  // Macrocells
  MacroCell *macrocell(size_t level);
  MacroCell *macrocell(size_t level, Quadrant *nw, Quadrant *ne, Quadrant *sw,
                       Quadrant *se);

  // Quadrant
  Quadrant *quadrant(size_t level);

  void get_cell_in_bounds_rec(Rect bounds, std::vector<Coord> *coords,
                              size_t current_level, Quadrant *current_cell,
                              Coord current_coord, size_t min_level) const;

  void pattern_bounding_box_rec(Rect *box, size_t level, Quadrant *q);
};

#endif // HASHLIFE_UNIVERSE_HPP
