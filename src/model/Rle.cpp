#include "model/CellState.h"
#include <model/Rle.h>
#include <stdexcept>

Rle::Rle(QFile *file) : _file(file) {
  if (!_file->open(QIODevice::ReadOnly | QIODevice::Text)) {
    throw std::runtime_error("Rle file could not be opened for reading");
  }

  QByteArray line;
  while (_file->canReadLine()) {
    line = _file->readLine();

    if (line.length() >= 3 && line.startsWith("#")) {
      switch (line.at(1)) {
      case 'N':
        // Name
        _name = line.mid(2).trimmed();
        break;
      case 'O':
        // Author
        _author = line.mid(2).trimmed();
        break;
      case 'c':
      case 'C':
        // Comment / Description
        _description.append(line.mid(2).trimmed() + '\n');
        break;
      case 'R':
      case 'P':
        {
        // Top left corner
        auto arguments = line.mid(2).split(',');
        if (arguments.length() == 2) {
          _top_left.x = arguments[0].trimmed().toStdString();
          _top_left.y = arguments[1].trimmed().toStdString();
        } else {
          throw std::runtime_error("Invalid coordonntes in P/R line.");
        }
          break;
        }
      case 'r':
        // Rule
        _rule = Rule(line.mid(2).trimmed());
        break;
      }
    }
  }

  _file->close();
}

Rle::Iter Rle::iter() {
  return Iter(_file);
}

const QString &Rle::name() {
  return _name;
}
const QString &Rle::author() {
  return _author;
}
const QString &Rle::description() {
  return _description;
}

const Vec2 &Rle::top_left() {
  return _top_left;
}
const Rule &Rle::rule() {
  return _rule;
}

Rle::Iter::Iter(QFile* file) : _file(file) {
  if (!_file->open(QIODevice::ReadOnly | QIODevice::Text)) {
    throw std::runtime_error("Rle file could not be opened for reading");
  }
}

Rle::Iter::~Iter() {
  _file->close();
}

bool Rle::Iter::next(size_t* count, CellState* state, bool* new_line) {
  char c;
  *count = 0;
  *new_line = false;
  while (!_file->getChar(&c)) {
    if (_line_start && c == '#') {
      _file->readLine();
    } else if ('0' <= c && c <= '9') {
      *count = *count * 10 + (c - '0');
    } else if (c != '\n') {
      if (c == 'b' || c == '.') {
        *state = 0;
      } else if (c == 'o' || c == 'A') {
        *state = 1;
      } else if (c == '$') {
        *new_line = true;
      }
      return true;
    }
  }
  return false;
}
