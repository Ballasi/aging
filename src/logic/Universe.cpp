#include "Universe.hpp"
#include "cell/MiniCell.hpp"

Universe::Universe(size_t level): Universe(Coord(), level) {}

Universe::Universe(Coord top_left, size_t top_level)
  : top_left(top_left), top_level(top_level) {
  // Starting with one extra level
  macrocell_sets.resize(top_level);
  zeros.push_back((Quadrant*) minicell());
  for (size_t i = 0; i < top_level; ++i) {
    zeros.push_back((Quadrant*) macrocell(i));
  }
  root = quadrant(top_level);
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

void Universe::set(Coord coord, CellState state) {
  Quadrant *cell = root;
  Coord size(top_level);
  Coord current(top_left);
  Coord center;
  for (size_t level = top_level; level > 1; --level) {
    center = current + size;
    if (coord.x < center.x) {
      coord.x += size.x;
      if (coord.y < top_left.y) {
        coord.y += size.y;
        cell = cell->macrocell.nw;
      }
      else {
        cell = cell->macrocell.ne;
      }
      }
    else {
      if (coord.y < top_left.y) {
        coord.y += size.y;
        cell = cell->macrocell.sw;
      }
      else {
        cell = cell->macrocell.se;
      }
    }
    size <<= 1;
  }

  center = current + size;
  if (coord.x < center.x) {
    if (coord.y < center.y) {
      cell->minicell.nw = state;
    }
    cell->minicell.ne = state;
  }
  else {
    if (coord.y < center.y) {
      cell->minicell.sw = state;
    }
    cell->minicell.se = state;
  }
}

CellState *Universe::find(Coord coord) const {
  Quadrant *cell = root;
  Coord size(top_level);
  Coord current(top_left);
  Coord center;
  for (size_t level = top_level; level > 1; --level) {
    center = current + size;
    if (coord.x < center.x) {
      coord.x += size.x;
      if (coord.y < top_left.y) {
        coord.y += size.y;
        cell = cell->macrocell.nw;
      }
      else {
        cell = cell->macrocell.ne;
      }
      }
    else {
      if (coord.y < top_left.y) {
        coord.y += size.y;
        cell = cell->macrocell.sw;
      }
      else {
        cell = cell->macrocell.se;
      }
    }
    size <<= 1;
  }

  center = current + size;
  if (coord.x < center.x) {
    if (coord.y < center.y) {
      return &cell->minicell.nw;
    }
    return &cell->minicell.ne;
  }
  else {
    if (coord.y < center.y) {
      return &cell->minicell.sw;
    }
    return &cell->minicell.se;
  }
}

CellState *Universe::find_path(Coord coord, vector<Quadrant*> &path) const {
  Quadrant *cell = root;
  Coord size(top_level);
  Coord current(top_left);
  Coord center;
  for (size_t level = top_level; level > 1; --level) {
    path.push_back(cell);
    center = current + size;
    if (coord.x < center.x) {
      coord.x += size.x;
      if (coord.y < top_left.y) {
        coord.y += size.y;
        cell = cell->macrocell.nw;
      } else {
        cell = cell->macrocell.ne;
      }
    } else {
      if (coord.y < top_left.y) {
        coord.y += size.y;
        cell = cell->macrocell.sw;
      } else {
        cell = cell->macrocell.se;
      }
    }
    size <<= 1;
  }

  center = current + size;
  if (coord.x < center.x) {
    if (coord.y < center.y) {
      return &cell->minicell.nw;
    }
    return &cell->minicell.ne;
  } else {
    if (coord.y < center.y) {
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
  return (MacroCell*) &*macrocell_sets[level - 1].emplace(nw, ne, sw, se).first;
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
