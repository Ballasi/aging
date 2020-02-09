#include "logic/Universe.hpp"

int main(int argc, char **argv) {
  Universe universe(3);

  universe.set(Coord(4, 2), 1);

  universe.debug();
}
