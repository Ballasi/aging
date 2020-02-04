#include "MacroCell.hpp"

MiniCell::MiniCell()
  : next(nullptr), nw(), ne(), se(), sw() {}

MiniCell::MiniCell(AtomicCell nw, AtomicCell ne, AtomicCell sw, AtomicCell se)
  : next(nullptr), nw(nw), ne(ne), se(se), sw(sw) {}

MacroCell::MacroCell(Quadrant *nw, Quadrant *ne, Quadrant *sw, Quadrant *se)
  : next(nullptr), nw(nw), ne(ne), sw(sw), se(se) {}

Quadrant* Quadrant::generate(size_t level) {
  if (level <= 1) {
    return (Quadrant*) new MiniCell;
  } else {
    return (Quadrant*)new MacroCell(generate(level - 1), generate(level - 1),
                             generate(level - 1), generate(level - 1));
  }
}

void Quadrant::debug(size_t level) {
  if (level == 1) {
    std::cout << "MiniCell::create(" << minicell.nw << ", " << minicell.ne
         << ", " << minicell.sw << ", " <<  minicell.se << ")";
  } else {
    std::cout << "MacroCell::create(";
    macrocell.nw->debug(level - 1);
    std::cout << ", ";
    macrocell.ne->debug(level - 1);
    std::cout << ", ";
    macrocell.sw->debug(level - 1);
    std::cout << ", ";
    macrocell.se->debug(level - 1);
  }
}
