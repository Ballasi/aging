#ifndef MINICELL_HPP
#define MINICELL_HPP

#include <functional>

typedef int AtomicCell;
union Quadrant;

class MiniCell {
public:
  MiniCell() = default;
  MiniCell(AtomicCell nw, AtomicCell ne, AtomicCell sw, AtomicCell se);

  bool operator==(MiniCell other);
  friend std::hash<MiniCell>;
  friend Quadrant;

private:
  AtomicCell nw, ne;
  AtomicCell sw, se;
};

namespace std {
  template <>
  struct hash<MiniCell> {
    hash() = default;

    size_t operator()(const MiniCell &minicell) const {
      return hasher(minicell.nw) ^ hasher(minicell.ne) ^ hasher(minicell.sw) ^
            hasher(minicell.se);
    }

    hash<AtomicCell> hasher;
  };
} // namespace std

#endif // MINICELL_HPP
