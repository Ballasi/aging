#ifndef MCELL_FILE_HPP_
#define MCELL_FILE_HPP_

#include <model/CellState.h>
#include <model/Rule.h>
#include <model/Vec2.h>

#include <QFile>
#include <QString>


class MCellFile {
public:
  explicit MCellFile(QFile *file);
  ~MCellFile();

  class LineIter;

  LineIter iter_line();

private:
  QFile* _file;

  // Header
  QString _comment;
  Rule _rule;
};

class MCellFile::LineIter {
public:
  explicit LineIter(QFile* file);
  enum LineType {MACRO_3, MACRO_N, DONE};

  class Level3_MCell {
    public:
      Level3_MCell();
      CellState cells[8][8];
  };
  class LevelN_MCell {
    public:
      LevelN_MCell();
      size_t level;
      size_t nw_index, ne_index;
      size_t sw_index, se_index;
  };
  union Line {
    Line();
    Level3_MCell level3;
    LevelN_MCell levelN;
  };

  LineType next(Line *line, LineType *type);

private:
  QFile* _file;
  QByteArray _current_line;
};

#endif // MCELL_FILE_HPP_
