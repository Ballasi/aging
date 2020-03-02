#ifndef RLE_HPP
#define RLE_HPP

#include "../CellState.hpp"
#include "../Coord.hpp"
#include "../LifeFile.hpp"
#include "../Rect.hpp"
#include "../Rule.hpp"

#include <QString>
#include <QFile>

using namespace std;

class RLE {
public:
  // Constructors
  RLE(QFile *file);

  // Iterator
  class Iterator {
  public:
    Iterator(QFile *file);

    bool next(CellState &state);

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
