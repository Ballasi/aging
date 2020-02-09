#include "Universe.hpp"
#include "cell/MiniCell.hpp"
#include <vector>

Universe::Universe(size_t level): Universe(Coord(), level) {}

Universe::Universe(Coord top_left, size_t top_level)
  : top_left(top_left), top_level(top_level) {
  // Starting with one extra level
  macrocell_sets.resize(top_level);
  zeros.push_back((Quadrant*) minicell());
  for (size_t i = 1; i < top_level; ++i) {
    zeros.push_back((Quadrant*) macrocell(i));
  }
  root = zeros.back();
}

void Universe::debug() {
  root->debug(top_level);
}

size_t Universe::step() {
  // TODO
  return 0;
}


const CellState Universe::get(Coord coord) const {
  return *find(coord);
}

void Universe::set(Coord target, CellState state) {
  root = set_rec(top_left, top_level, root, target, state);
}


Quadrant *Universe::set_rec(Coord current, size_t level, Quadrant *cell, Coord target, CellState state) {
  Coord size(level - 1);
  Coord center = current + size;

  if (level <= 1) {
    if (target.y < center.y) {
      if (target.x < center.x)
        return (Quadrant*) minicell(      state      , cell->minicell.ne,
                                    cell->minicell.sw, cell->minicell.se);
      else
        return (Quadrant*) minicell(cell->minicell.nw,       state      ,
                                    cell->minicell.sw, cell->minicell.se);
    } else {
      if (target.x < center.x)
        return (Quadrant*) minicell(cell->minicell.nw, cell->minicell.ne,
                                          state      , cell->minicell.se);
      else
        return (Quadrant*) minicell(cell->minicell.nw, cell->minicell.ne,
                                    cell->minicell.sw,       state      );
    }
  }
  else {
    level -= 1;
    if (target.y < center.y) {
      if (target.x < center.x)
        return (Quadrant *) macrocell(level,
                                      set_rec(current, level, cell->macrocell.nw , target, state),
                                      cell->macrocell.ne, cell->macrocell.sw, cell->macrocell.se);
      else {
        current.x += size.x;
        return (Quadrant *) macrocell(level, cell->macrocell.nw,
                                      set_rec(current, level, cell->macrocell.ne, target, state),
                                      cell->macrocell.sw, cell->macrocell.se);
      }
    } else {
      current.y += size.y;
      if (target.x < center.x)
        return (Quadrant *) macrocell(level, cell->macrocell.nw, cell->macrocell.ne,
                                      set_rec(current, level, cell->macrocell.sw , target, state),
                                      cell->macrocell.se);
      else {
        current.x += size.x;
        return (Quadrant *) macrocell(level, cell->macrocell.nw, cell->macrocell.ne, cell->macrocell.sw,
                                      set_rec(current, level, cell->macrocell.se, target, state));
      }
    }
  }
}

CellState *Universe::find(Coord coord) const {
  Quadrant *cell = root;
  Coord size(top_level - 1);
  Coord current(top_left);
  Coord center = current + size;
  for (size_t level = top_level; level > 1; --level) {
    if (coord.x < center.x) {
      if (coord.y < center.y) {
        cell = cell->macrocell.nw;
      } else {
        current.y += size.y;
        cell = cell->macrocell.ne;
      }
    } else {
      current.x += size.x;
      if (coord.y < center.y) {
        cell = cell->macrocell.sw;
      } else {
        current.y += size.y;
        cell = cell->macrocell.se;
      }
    }
    size >>= 1;
    center = current + size;
  }


  if (coord.y < center.y) {
    if (coord.x < center.x) {
      return &cell->minicell.nw;
    }
    return &cell->minicell.ne;
  } else {
    if (coord.x < center.x) {
      return &cell->minicell.sw;
    }
    return &cell->minicell.se;
  }
}

CellState *Universe::find_path(Coord coord, vector<Quadrant*> &path) const {
  Quadrant *cell = root;
  Coord size(top_level - 1);
  Coord current(top_left);
  Coord center = current + size;
  for (size_t level = top_level; level > 1; --level) {
    path.push_back(cell);
    if (coord.x < center.x) {
      if (coord.y < center.y) {
        cell = cell->macrocell.nw;
      } else {
        current.y += size.y;
        cell = cell->macrocell.ne;
      }
    } else {
      current.x += size.x;
      if (coord.y < center.y) {
        cell = cell->macrocell.sw;
      } else {
        current.y += size.y;
        cell = cell->macrocell.se;
      }
    }
    size >>= 1;
    center = current + size;
  }

  if (coord.y < center.y) {
    if (coord.x < center.x) {
      return &cell->minicell.nw;
    }
    return &cell->minicell.ne;
  } else {
    if (coord.x < center.x) {
      return &cell->minicell.sw;
    }
    return &cell->minicell.se;
  }
}

MiniCell *Universe::minicell() {
  return (MiniCell *)&*minicells.emplace().first;
}

MiniCell *Universe::minicell(CellState nw, CellState ne,
                             CellState sw, CellState se) {
  return (MiniCell*) &*minicells.emplace(nw, ne, sw, se).first;
}

MacroCell *Universe::macrocell(size_t level) {
  return (MacroCell*) &*macrocell_sets[level]
      .emplace(zeros[level - 1], zeros[level - 1],
               zeros[level - 1], zeros[level - 1])
      .first;
}

MacroCell *Universe::macrocell(size_t level,
                               Quadrant *nw, Quadrant *ne,
                               Quadrant *sw, Quadrant *se) {
  return (MacroCell*) &*macrocell_sets[level].emplace(nw, ne, sw, se).first;
}

Quadrant *Universe::quadrant(size_t level) {
  if (level <= 1) {
    return (Quadrant*) minicell();
  } else {
    return (Quadrant*) macrocell(level,
                                 quadrant(level - 1), quadrant(level - 1),
                                 quadrant(level - 1), quadrant(level - 1));
  }
}

size_t Universe::get_top_level() {
  return top_level;
}

Coord Universe::get_top_left(){
  return top_left;
}
