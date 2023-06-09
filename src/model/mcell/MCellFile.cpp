#include <model/mcell/MCellFile.h>
#include <iostream>

MCellFile::MCellFile(QFile *file) : _file(file) {
    if (!_file->isOpen()) {
        if (!_file->open(QIODevice::ReadOnly | QIODevice::Text))
            throw "Error opening MCell File";
    }
    QByteArray line;
    while (!_file->atEnd()) {
        line = _file->readLine();
        if (line[0] == '#' || line[0] == '[') {
            if (line.length() > 5) {
                if (line[0] == '#' && line[1] == 'R') {
                    QByteArrayList tokens;
                    tokens = line.split(' ');
                    if (tokens.length() >= 2) {
                        //_rule = Rule(tokens[1].trimmed());
                    }
                }
            }
            _comment += line;
            continue;
        } else {
            _file->close();
            break;
        }
    }
}

MCellFile::LineIter::LineIter(QFile *file) : _file(file) {}
MCellFile::LineIter::Level3_MCell::Level3_MCell() {}
MCellFile::LineIter::LevelN_MCell::LevelN_MCell() {}

MCellFile::LineIter MCellFile::iter_line() {
    if (!_file->isOpen()) {
        if (!_file->open(QIODevice::ReadOnly | QIODevice::Text))
            throw "Error opening MCell File";
    }
    return LineIter(_file);
}

MCellFile::LineIter::LineType MCellFile::LineIter::next(Line *line,
                                                        LineType *type) {
    if (_file->atEnd()) {
      _file->close();
      *type = DONE;
      return DONE;
    }

    _current_line = _file->readLine();
    if (_current_line.trimmed().isEmpty() ||
        _current_line[0] == '#'           ||
        _current_line[0] == '[') {
        return next(line, type);
    } else if ( _current_line[0] == '$' ||
                _current_line[0] == '.' ||
                _current_line[0] == '*') {
        for (size_t i = 0; i < 8; ++i) {
            for (size_t j = 0; j < 8; ++j) {
                line->level3.cells[i][j] = 0;
            }
        }
        uint8_t x = 0;
        uint8_t y = 0;
        int line_i = 0;
        while (_current_line[line_i] == '$'
            || _current_line[line_i] == '.'
            || _current_line[line_i] == '*') {
            char c = _current_line[line_i];
            if (c == '.') {
                ++x;
            } else if (c == '*') {
                line->level3.cells[x][y] = 1;
                ++x;
            } else if (c == '$') {
                ++y;
                x = 0;
            }
            ++line_i;
        }
        *type = MACRO_3;
        return MACRO_3;
    } else if (_current_line[0] >= '0' &&
               _current_line[0] <= '9') {
        QByteArrayList line_data = _current_line.split(' ');
        if (line_data.length() < 5)
            throw "Invalid macrocell line";
        bool oks[5] = {true};
        line->levelN.level = line_data[0].trimmed().toULong(oks);
        line->levelN.nw_index = line_data[1].trimmed().toULong(oks + 1);
        line->levelN.ne_index = line_data[2].trimmed().toULong(oks + 2);
        line->levelN.sw_index = line_data[3].trimmed().toULong(oks + 3);
        line->levelN.se_index = line_data[4].trimmed().toULong(oks + 4);
        for (bool ok : oks) {
            if (!ok)
                throw "Error in toULong()";
        }
        *type = MACRO_N;
        return MACRO_N;
    }

    throw "Unrecognized line";
    _file->close();
    *type = DONE;
    return DONE;
}

MCellFile::LineIter::Line::Line() {}

MCellFile::~MCellFile() {}
