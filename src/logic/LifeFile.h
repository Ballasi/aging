#ifndef LIFE_FILE_HPP
#define LIFE_FILE_HPP

#include <QFile>
#include <QString>

#include <logic/CellState.h>
#include <logic/Coord.h>
#include <logic/Rect.h>
#include <logic/Rule.h>

class LifeFile {
public:
  explicit LifeFile(QFile *file);

  class Iterator {
  public:
    explicit Iterator(QFile *file);

    bool next(const CellState &state);
  };

  Iterator iterate(QFile *file) const;

  // Getters
  QString get_comment() const;
  Coord get_size() const;
  Rule get_rule() const;
};

#endif // LIFE_FILE_HPP
