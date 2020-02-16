#include "Iterator.hpp"
#include "HashlifeUniverse.hpp"


HashlifeUniverse::Iterator::Iterator(HashlifeUniverse *universe, Rect bounds)
  : universe(universe), bounds(bounds), finished() {

  // Checking that the universe is not empty
  if ((Quadrant *)universe->root == universe->zeros[universe->top_level]) {
    finished = true;
    return;
  }

  // Initilization node statup values
  size_t index = 0;
  Coord coord = universe->top_left;
  Quadrant *quadrant = (Quadrant *)universe->root;

  // Initilizing iteration stack
  for (size_t i = universe->top_level; i > 0; --i) {
    stack.push_back({index, coord, quadrant});

    // Getting to the top left of the Universe
    quadrant = quadrant->macrocell.nw;
  }
}

bool HashlifeUniverse::Iterator::next(Coord& coord, CellState& state) {
  // Check if iteration is finished
  if (finished)
    return false;

  // Getting current node
  Node &node = stack.back();

  // Setting coordonat
  coord = node.coord;
  if (node.index > 1)
    coord.y ++;
  if (node.index % 2)
    coord.x ++;

  // Setting state
  state = node.quadrant->minicell[node.index];

  // Finding next inner state
  node.index ++;

  if (node.index > 3) {
    size_t layer = 1;
    while (node.index > 3 ||             // node has no nothing to explore
           &node.quadrant[node.index] == // is not empty
               universe->zeros[layer]) {

      // Move up
      stack.pop_back();
      if (stack.empty())
        return finished = true;
      node = stack.back();
      layer ++;
    }

    // Moving node position
    BigInt size = 1 << layer;
    if (!(node.index % 2))
      node.coord += size;
    else if (node.index == 1) {
      node.coord.x -= size;
      node.coord.y += size;
    }

    // Moving back down
    stack.push_back({0, node.coord, node.quadrant});
    while (stack.size() < universe->top_level) {
      size_t index = 0;
      while (&node.quadrant[index++] == universe->zeros[layer]
             || bounds.does_not_collide({}));
      stack.push_back({index, node.coord, node.quadrant});
    }
  }
  return true;
}
