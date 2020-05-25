#ifndef HASHLIFE_UNIVERSE_HPP
#define HASHLIFE_UNIVERSE_HPP

#include <QFile>
#include <QString>
#include <unordered_set> #include <vector> 
#include <model/Rect.h>
#include <model/Universe.h>
#include <model/Vec2.h>

#include "cell/MacroCell.h"
#include "cell/MiniCell.h"
#include "cell/Quadrant.h"
#include <utility>
using cell::MiniCell;
using cell::MacroCell;
using cell::Quadrant;

class HashUniverse : public Universe {
public:
  // Constructors
  explicit HashUniverse(size_t top_level, Vec2 top_left = Vec2());
  explicit HashUniverse(QString filename, Vec2 top_left = Vec2());

  // Interface
  void update() override;

  Rect &bounds() override;
  const BigInt &generation() const override;
  const BigInt &step_size() const override;

  // Others
  const CellState get(const Vec2& coord) const override;
  void set(const Vec2& target, CellState state) override;

  std::pair<Rect, size_t> get_pattern_bounding_box();
  void get_cell_in_bounds(Rect bounds,
                          std::vector<Vec2> *coords,
                          size_t min_level) const;

  Vec2 get_top_left();
  size_t get_top_level();

  void set_step_size(size_t new_step_size) override;
  const bool can_set_step_size() const override;

  void set_hyperspeed(bool on) override;
  const bool can_set_hyperspeed() const override;

private:
  // For interface
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

  void _assert_handles(size_t asserted_level);

  // Step helpers
  void _crown();
  Quadrant *_result(size_t level, MacroCell *macrocell_tmp);

  // Recursive setter
  Quadrant *_set_rec(Vec2 current, size_t level, Quadrant *cell, Vec2 target,
                    CellState state);

  size_t _pattern_bounding_box_rec(Rect *box, size_t level, Quadrant *q);

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
                               Vec2 current_coord,
                               size_t min_level) const;

  void _grow_to(size_t target);
};

#endif // HASHLIFE_UNIVERSE_HPP
