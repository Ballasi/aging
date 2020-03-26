#ifndef RLE_HPP
#define RLE_HPP

#include <logic/CellState.h>
#include <logic/Coord.h>
#include <logic/LifeFile.h>
#include <logic/Rect.h>
#include <logic/Rule.h>

#include <QFile>
#include <QString>

class RLE {
public:
  // Constructors
  explicit RLE(QFile *file);

  // Iterator
  class Iterator {
  public:
    explicit Iterator(QFile *file);

    bool next(const CellState &state);

  private:
    QFile *file;
  };
  Iterator iterate();

  // Getters
  QString get_comment() const;
  Coord get_size() const;
  Rule get_rule() const;

private:
  // file name
  QFile file;

  // Attributes
  QString comment;
  Coord size;
  Rule rule;

  // For iterator
  QFile get_file_at_data();
};

#endif // RLE_HPP
