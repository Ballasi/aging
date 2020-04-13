#include <universes/hashlife/cell/Quadrant.h>
#include <string>

Quadrant *Quadrant::generate(size_t level) {
  if (level <= 1) {
    return reinterpret_cast<Quadrant *>(new MiniCell);
  } else {
    return reinterpret_cast<Quadrant *>(
        new MacroCell(generate(level - 1), generate(level - 1),
                      generate(level - 1), generate(level - 1)));
  }
}

Quadrant *Quadrant::generate_random(size_t level, CellState max_value) {
  std::random_device undeterministic_generator;
  std::default_random_engine deterministic_generator(
      undeterministic_generator());
  std::uniform_int_distribution<CellState> range(0, max_value);
  return raw_generate_random(level, deterministic_generator, range);
}

Quadrant *
Quadrant::raw_generate_random(size_t level,
                              std::default_random_engine random_engine,
                              std::uniform_int_distribution<CellState> range) {
  if (level <= 1) {
    return reinterpret_cast<Quadrant *>(
        new MiniCell(range(random_engine), range(random_engine),
                     range(random_engine), range(random_engine)));
  } else {
    return reinterpret_cast<Quadrant *>(
        new MacroCell(raw_generate_random(level - 1, random_engine, range),
                      raw_generate_random(level - 1, random_engine, range),
                      raw_generate_random(level - 1, random_engine, range),
                      raw_generate_random(level - 1, random_engine, range)));
  }
}

void Quadrant::debug_rec(size_t level, std::string indentation) {
  if (level <= 1) {
    std::cout << indentation << "(Quadrant *) new MiniCell(" << minicell.nw
              << ", " << minicell.ne << ", " << minicell.sw << ", "
              << minicell.se << ")";
  } else {
    std::cout << indentation << "(Quadrant *) new MacroCell(\n";
    macrocell.nw->debug_rec(level - 1, indentation + "  ");
    std::cout << ",\n";
    macrocell.ne->debug_rec(level - 1, indentation + "  ");
    std::cout << ",\n";
    macrocell.sw->debug_rec(level - 1, indentation + "  ");
    std::cout << ",\n";
    macrocell.se->debug_rec(level - 1, indentation + "  ");
    std::cout << "\n" << indentation << ")";
  }
}

void Quadrant::debug(size_t level) { Quadrant::debug_rec(level, ""); }

Quadrant *Quadrant::operator[](const size_t index) const {
  switch (index) {
  case 0:
    return macrocell.nw;
  case 1:
    return macrocell.ne;
  case 2:
    return macrocell.sw;
  default:
    return macrocell.se;
  }
}
