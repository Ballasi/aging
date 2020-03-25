#include <logic/HashlifeUniverse/cell/MiniCell.h>

MiniCell::MiniCell(CellState nw, CellState ne, CellState sw, CellState se)
    : nw(nw), ne(ne), se(se), sw(sw) {}

bool MiniCell::operator==(const MiniCell other) const {
  return nw == other.nw && ne == other.ne && sw == other.sw && se == other.se;
}

CellState MiniCell::operator[](const size_t index) const {
  switch (index) {
  case 0:
    return nw;
  case 1:
    return ne;
  case 2:
    return sw;
  default:
    return se;
  }
}
