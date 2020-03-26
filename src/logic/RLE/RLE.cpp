#include <logic/RLE/RLE.h>
/*
RLE::RLE(QFile *_file): file(_file) {
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    throw "File could not be opened";

  // TODO Check for errors
  while (!file.atEnd()) {
    QByteArray line = file.readLine();
    if (line[0] == '#') {
      comment.append(line);
    }
    if (line[0] == 'x') {
      QList<QByteArray> list = line.split(',');
      int width = list[0].split('=')[1].simplified().toInt();
      int height = list[1].split('=')[1].simplified().toInt();
      rule = Rule(list[2].split('=')[1].simplified());
      size = Coord(width, height);
    }
  }

  file.close();
}

RLE::Iterator RLE::iterate() {
  return RLE::Iterator(*this);
}

QString RLE::get_comment() const {
  return comment;
}

Coord RLE::get_size() const {
  return size;
}

QFile RLE::get_file_at_data() {
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    throw "File could not be opened";
}
*/
