#include "MiniCell.hpp"

MiniCell::MiniCell(AtomicCell nw, AtomicCell ne, AtomicCell sw, AtomicCell se)
    :  nw(nw), ne(ne), se(se), sw(sw) {}

bool MiniCell::operator==(MiniCell other) {
  return nw == other.nw && ne == other.ne && sw == other.sw && se == other.se;
}
