#include "RLE.hpp"

RLE::RLE(QString filename) : filename(filename) {
  QFile file(filename);

  if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    throw "File could not be opened";

  while (!file.atEnd()) {
    QByteArray line = file.readLine();
    if (line[0] == '#') {
      continue;
    }
    if (line[0] == 'x') {
      QList<QByteArray> list = line.split(',');
      int width = list[0].split('=')[1].simplified().toInt();
      int height = list[1].split('=')[1].simplified().toInt();
      rule = Rule(list[2].split('=')[1].simplified());
      size = Coord(width, height);
    }
  }
}

QString RLE::get_comment() const {
  return comment;
}

Coord RLE::get_size() const {
  return size;
}
