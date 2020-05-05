#ifndef RLE_H_
#define RLE_H_

#include <model/Vec2.h>
#include <model/CellState.h>
#include <model/Rule.h>
#include <QFile>
#include <QString>
#include <QStringAlgorithms>

class Rle {
public:
  explicit Rle(QFile* file);

  class Iter;

  Iter iter();

  // Getters
  const QString& name();
  const QString& author();
  const QString& description();

  const Vec2& top_left();
  const Rule& rule();

private:
  QFile *_file;

  // Meta data
  QString _name = "";
  QString _author = "";
  QString _description = "";

  Vec2 _top_left;
  Rule _rule;
};

class Rle::Iter {
public:
  explicit Iter(QFile* file);
  ~Iter();

  bool next(size_t *count, CellState *state, bool *new_line);

private:
  bool _line_start = true;
  QFile *_file;
};

#endif // RLE_H_
