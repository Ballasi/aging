#ifndef LIFE_FILE_HPP
#define LIFE_FILE_HPP

#include <QFile>
#include <QString>

#include "CellState.hpp"
#include "Coord.hpp"
#include "Rect.hpp"
#include "Rule.hpp"

class LifeFile {
public:
  LifeFile(QFile *file);

  class Iterator {
  public:
    Iterator(QFile *file);

    bool next(CellState& state);
  };

  Iterator iterate(QFile *file) const;

  // Getters
  QString get_comment() const;
  Coord get_size() const;
  Rule get_rule() const;
};

#endif // LIFE_FILE_HPP
