#ifndef HASHLIFEUNIVERSE_ITERATOR
#define HASHLIFEUNIVERSE_ITERATOR

#include <vector>

#include "HashlifeUniverse.hpp"
#include "../Rect.hpp"

using namespace std;

class HashlifeUniverse::Iterator {
public:
  // Constructor
  Iterator(HashlifeUniverse *universe, Rect bounds);

  bool next(Coord& coord, CellState& state);

private:

  // Parameters
  HashlifeUniverse* universe;
  Rect bounds;

  // Node
  struct Node {
    size_t index;
    Coord coord;
    Quadrant *quadrant;
  };

  // Inner state
  vector<Node> stack;
  bool finished;
};

#endif // HASHLIFEUNIVERSE_ITERATOR
