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
  virtual const BigInt &step_size() const = 0;

  // Aptitudes
  virtual const bool can_set() const;
  virtual void set(Vec2 &coord, CellState cell_state);

  virtual const bool can_get() const;
  virtual const CellState get(Vec2 &coord);

  virtual const bool can_clear() const;
  virtual void clear();

  virtual const bool can_set_step_size() const;
  virtual void set_step_size(const size_t step_size);

  virtual const bool can_resize() const;
  virtual void resize();

  // Meta aptitudes
  bool can_get_set();
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
