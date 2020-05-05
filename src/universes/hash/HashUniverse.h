#ifndef HASHLIFE_UNIVERSE_HPP
#define HASHLIFE_UNIVERSE_HPP

#include <QFile>
#include <QString>
#include <unordered_set>
#include <vector>

#include <model/Rect.h>
#include <model/Universe.h>
#include <model/Vec2.h>

#include "cell/MacroCell.h"
#include "cell/MiniCell.h"
#include "cell/Quadrant.h"

using cell::MiniCell;
using cell::MacroCell;
using cell::Quadrant;

class HashUniverse : public Universe {
public:
  // Constructors
  explicit HashUniverse(size_t top_level, Vec2 top_left = Vec2());
  explicit HashUniverse(QString filename, Vec2 top_left = Vec2());

  // Interface
  void update();

  const Rect &bounds() const;
  const BigInt &generation() const;
  const BigInt &step_size() const;

  // Others
  const CellState get(Vec2 coord) const;
  void set(Vec2 target, CellState state);
  void debug();

  void get_cell_in_bounds(Rect bounds, std::vector<Vec2> *coords) const;

  Vec2 get_top_left();
  size_t get_top_level();

  void set_step_size(size_t new_step_size);
  void set_step_size_maximized(bool is_maximized);

  void grid(int *L, int width, Quadrant *r, int level, int x, int y);
  void print_grid(Quadrant *r, size_t level);

private:
  // Inner
  Rect _bounds;
  BigInt _generation;
  BigInt _step_size;

  // Dimentions
  size_t _top_level;
  Vec2 _top_left;

  size_t _recursion_depth;
  bool _step_size_maximized;

  // Data
  MacroCell *_root;

  std::vector<std::unordered_set<MacroCell>> _macrocell_sets;
  std::unordered_set<MiniCell> _minicells;

  // Zero cache
  std::vector<Quadrant *> _zeros;

  // File loading
  void _build_from_rle(QFile *file);
  void _build_from_mc(QFile *file);
  Vec2 _read_rle_size(QFile *file);
  void _read_rle_data(QFile *file, Vec2 boundingbox);

  // TODO(chancyleg): find good name
  void _assert_handles(size_t asserted_level);

  // Step helpers
  void _crown();
  Quadrant *_result(size_t level, MacroCell *macrocell_tmp);

  // Recursive setter
  Quadrant *_set_rec(Vec2 current, size_t level, Quadrant *cell, Vec2 target,
                    CellState state);

  // Finding
  CellState *_find(Vec2) const;
  CellState *_find_path(Vec2 coord, std::vector<Quadrant *> *path) const;

  // Minicells
  MiniCell *_minicell();
  MiniCell *_minicell(CellState nw, CellState ne, CellState sw, CellState se);

  // Macrocells
  MacroCell *_macrocell(size_t level);
  MacroCell *_macrocell(size_t level, Quadrant *nw, Quadrant *ne, Quadrant *sw,
                       Quadrant *se);

  // Quadrant
  Quadrant *_quadrant(size_t level);

  void _get_cell_in_bounds_rec(Rect bounds, std::vector<Vec2> *coords,
                              size_t current_level, Quadrant *current_cell,
                              Vec2 current_coord) const;
};

#endif // HASHLIFE_UNIVERSE_HPP
