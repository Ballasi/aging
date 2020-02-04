#include "MacroCell.hpp"

Quadrant* MiniCell::create(AtomicCell nw, AtomicCell ne, AtomicCell sw, AtomicCell se) {
  MiniCell *minicell = (MiniCell*) malloc(sizeof(MiniCell));
  minicell->next = nullptr;
  minicell->nw = nw;
  minicell->ne = ne;
  minicell->sw = sw;
  minicell->se = se;
  return (Quadrant*) minicell;
}

Quadrant* MacroCell::create(Quadrant *nw, Quadrant *ne, Quadrant *sw,
                            Quadrant *se) {
  MacroCell *macrocell = (MacroCell *)malloc(sizeof(MacroCell));
  macrocell->next = nullptr;
  macrocell->result = nullptr;
  macrocell->nw = (Quadrant *)nw;
  macrocell->ne = (Quadrant *)ne;
  macrocell->sw = (Quadrant *)sw;
  macrocell->se = (Quadrant *)se;
  return (Quadrant *)macrocell;
}

Quadrant* Quadrant::generate(size_t level) {
  if (level <= 1) {
    return (Quadrant*) MiniCell::create(0, 0, 0, 0);
  } else {
    return MacroCell::create(generate(level - 1), generate(level - 1),
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
