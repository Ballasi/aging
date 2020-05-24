#include <model/Universe.h>

const bool Universe::can_set() const { return false; }

void Universe::set(const Vec2 &coord, CellState cell_state) {
  throw "This `set` method is not implemented for this universe";
}


const bool Universe::can_get() const { return false; }

const CellState Universe::get(const Vec2 &coord) const {
  throw "This `get` method is not implemented for this universe";
}


const bool Universe::can_clear() const { return false; }

void Universe::clear() {
  throw "This `clear` method is not implemented for this universe";
}


const bool Universe::can_set_step_size() const { return false; }

void Universe::set_step_size(const size_t step_size) {
  throw "This `set_step_size` method is not implemented for this universe";
}

const bool Universe::can_set_hyperspeed() const { return false; }

void Universe::set_hyperspeed(const bool state) {
  throw "This `set_hyperspeed` method is not implemented for this universe";
}

const bool Universe::can_reshape() const { return false; }

void Universe::reshape(const Rect& bounds) {
  throw "This `reshape` method is not implemented for this universe";
}
