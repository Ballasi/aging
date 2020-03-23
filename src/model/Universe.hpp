#ifndef UNIVERSE_HPP
#define UNIVERSE_HPP

#include "Vec2.hpp"
#include "Rect.hpp"
#include "CellState.hpp"

#include <QString>

#include <vector>

class Universe {
public:
  // TODO: Do constructor question with Tristan

  // Required methods
  virtual void update() = 0;

  virtual const Rect &bounds() const = 0;
  virtual const BigInt &generation() const = 0;


  // Aptitudes
  static bool can_set() {
    return _can_set;
  }
  virtual void set(Vec2 &coord, CellState cell_state) {
    throw "Method set is not defined for this universe type";
  };

  static bool can_get() {
    return _can_get;
  }
  virtual const CellState get(Vec2 &coord) const {
    throw "Method get is not defined for this universe type";
  };

  static bool can_set_step_size() {
    return _can_set_step_size;
  }
  virtual void set_step_size(const size_t step_size) {
    throw "Method set_step_size is not defined for this universe type";
  };

  static bool can_resize() {
    return _can_resize;
  }
  void resize() {
    throw "Method resize is not defined for this universe type";
  }

  // Meta aptitudes
  static bool can_get_set() {
    return _can_get && _can_set;
  }

protected:
  static const bool _can_set = false;
  static const bool _can_get = false;
  static const bool _can_set_step_size = false;
  static const bool _can_resize = false;
};

class UniverseType {
public:
  UniverseType(QString name,
               QString description,
               Universe* (*constructor)(void));

  Universe* (*constructor)(void);

  const QString name;
  const QString description;
};

#endif // UNIVERSE_HPP
