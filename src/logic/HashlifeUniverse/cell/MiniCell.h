#ifndef MINICELL_HPP
#define MINICELL_HPP

#include <functional>

#include <logic/CellState.h>

union Quadrant;

class MiniCell {
public:
  MiniCell() = default;
  MiniCell(CellState nw, CellState ne, CellState sw, CellState se);

  bool operator==(const MiniCell other) const;
  CellState operator[](const size_t index) const;

  friend std::hash<MiniCell>;
  friend Quadrant;

  CellState nw, ne;
  CellState sw, se;
};

namespace std {
template <> struct hash<MiniCell> {
  hash() = default;

  size_t operator()(const MiniCell &minicell) const {
    return hasher(minicell.nw) ^ hasher(minicell.ne) ^ hasher(minicell.sw) ^
           hasher(minicell.se);
  }

  hash<CellState> hasher;
};
} // namespace std

#endif // MINICELL_HPP
