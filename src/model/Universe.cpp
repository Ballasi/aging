#include <model/Universe.h>

const bool Universe::can_set() const { return false; }

void Universe::set(const Vec2 &coord, CellState cell_state) {
  throw "This method is not implemented for this universe";
}


const bool Universe::can_get() const { return false; }

const CellState Universe::get(const Vec2 &coord) const {
  throw "This method is not implemented for this universe";
}


const bool Universe::can_clear() const { return false; }

void Universe::clear() {
  throw "This method is not implemented for this universe";
}


const bool Universe::can_set_step_size() const { return false; }

void Universe::set_step_size(const size_t step_size) {
  throw "This method is not implemented for this universe";
}

const bool Universe::can_reshape() const { return false; }

void Universe::reshape(Rect bounds) {
  throw "This method is not implemented for this universe";
}
