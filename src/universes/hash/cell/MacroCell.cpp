#include <universes/hash/cell/MacroCell.h>

namespace cell {
MacroCell::MacroCell(Quadrant *nw, Quadrant *ne, Quadrant *sw, Quadrant *se)
    : nw(nw), ne(ne), sw(sw), se(se), result(nullptr) {}

bool MacroCell::operator==(const MacroCell other) const {
  return nw == other.nw && ne == other.ne && sw == other.sw && se == other.se;
}

Quadrant *MacroCell::operator[](const size_t index) const {
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
} // namespace cell
