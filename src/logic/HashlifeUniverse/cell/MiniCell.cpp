#include "MiniCell.hpp"

MiniCell::MiniCell(CellState nw, CellState ne, CellState sw, CellState se)
    :  nw(nw), ne(ne), se(se), sw(sw) {}

bool MiniCell::operator==(const MiniCell other) const {
  return nw == other.nw && ne == other.ne && sw == other.sw && se == other.se;
}
