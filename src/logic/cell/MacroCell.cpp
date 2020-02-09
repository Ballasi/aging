#include "MacroCell.hpp"

MacroCell::MacroCell(Quadrant *nw, Quadrant *ne,
                     Quadrant *sw, Quadrant *se)
  : nw(nw), ne(ne), sw(sw), se(se) {}


bool MacroCell::operator==(const MacroCell other) const {
  return nw == other.nw && ne == other.ne && sw == other.sw && se == other.se;
}
