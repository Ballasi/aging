#ifndef RLE_HPP
#define RLE_HPP

#include "Coord.hpp"
#include "Rect.hpp"
#include "Rule.hpp"

#include <QString>
#include <QFile>

using namespace std;

class RLE {
public:
  RLE(QString filename);

  class Iterator {
  public:
    bool next(CellState& state);

  private:

  };

  Iterator iterate();

  QString get_comment() const;
  Coord get_size() const;

private:
  // file name
  QString filename;

  QString comment;
  Coord size;
  Rule rule;
};

#endif // RLE_HPP
