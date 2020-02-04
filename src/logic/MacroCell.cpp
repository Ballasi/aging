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


void Quadrant::debug_rec(size_t level, std::string indentation) {
  if (level == 1) {
    std::cout << indentation << "MiniCell::create(" << minicell.nw << ", " << minicell.ne
         << ", " << minicell.sw << ", " <<  minicell.se << ")";
  } else {
    std::cout << indentation << "MacroCell::create(\n";
    macrocell.nw->debug_rec(level - 1, indentation + "\t");
    std::cout <<",\n" ;
    macrocell.ne->debug_rec(level - 1, indentation + "\t");
    std::cout <<",\n" ;
    macrocell.sw->debug_rec(level - 1, indentation + "\t");
    std::cout <<",\n" ;
    macrocell.se->debug_rec(level - 1, indentation + "\t");
    std::cout << "\n" << indentation << ")";
  }
}

void Quadrant::debug(size_t level) {
  Quadrant::debug_rec(level, "");
}



