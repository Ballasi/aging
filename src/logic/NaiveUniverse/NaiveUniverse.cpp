#include <logic/NaiveUniverse/NaiveUniverse.h>
#include <iostream>
#include <gmp.h>
#include <cmath>

NaiveUniverse::NaiveUniverse(Coord size)
    : width(size.x.get_si() + 8 - size.x.get_si() % 8), height(size.y.get_si()),
      length_in_bytes(width * height) {
  // Adding () calls constructor for every Cell in the array
  cells = new Cell[length_in_bytes]();
  neighbour_count = new uint8_t[(width + 1) * height / 2];
}

NaiveUniverse::NaiveUniverse(QString filename) {
  QFile file(filename);
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    return;
  Coord size;
  Coord box = read_rle_size(&file, &size);

  size_t min_len = (size.x.get_si() > 128) ? size.x.get_si() : 128;
  size = Coord(min_len, min_len);

  width = size.x.get_si() + 8 - size.x.get_si() % 8;
  height = size.y.get_si();
  length_in_bytes = width * height;
  cells = new Cell[length_in_bytes]();
  neighbour_count = new uint8_t[(width + 1) * height / 2];

  read_rle_data(&file, box, size);
}

NaiveUniverse::~NaiveUniverse() {
  delete[] cells;
  delete[] neighbour_count;
}

void NaiveUniverse::clear() {
  for (size_t i = 0; i < length_in_bytes; ++i)
    cells[i] = 0;
}

void NaiveUniverse::set(Coord target, CellState state) {
  if (target.x.get_si() >= 0 && target.x.get_si() < width &&
     target.y.get_si() >= 0 & target.y.get_si() < height) {
    Cell *cell_ptr =
        cells + (target.y.get_si() * width / 8) + (target.x.get_si() + 7) / 8;

    if (state)
      *(cell_ptr) |= 1 << (target.x.get_si() % 8);
    else
      *(cell_ptr) &= ~(1 << (target.x.get_si() % 8));
  }
}

const CellState NaiveUniverse::get(Coord coord) const {
  return (*(cells + coord.y.get_si() * width / 8 +
            (coord.x.get_si() + 7) / 8) >>
          (coord.x.get_si() % 8)) &
         0b01;
}

void NaiveUniverse::step() {
  memset(neighbour_count, 0, (width + 1) * height / 2);
  updateNeighbourCount(neighbour_count);

  size_t w = width, h = height;

  for (size_t l = 0; l < h; ++l) {
    for (size_t c = 0; c < w; ++c) {
      size_t neighbours =
          *(neighbour_count + l * width / 2 + (c + 1) / 2) >> ((c % 2) * 4) &
          0b1111;
      if (get(Coord(c, l))) {
        if ((neighbours != 2) && (neighbours != 3))
          set(Coord(c, l), 0);
      } else {
        if (neighbours == 3)
          set(Coord(c, l), 1);
      }
    }
  }

  ++generation_count;
}

void NaiveUniverse::updateNeighbourCount(uint8_t *neighbour_count) {
  size_t w = width, h = height;

  for (size_t l = 0; l < h; ++l) {
    for (size_t c = 0; c < w; ++c) {
      if (get(Coord(c, l))) {
        size_t count = 0;

        if (c > 0 && l > 0) {
          Cell *topleft_cell = neighbour_count + (l - 1) * w / 2 + c / 2;
          *topleft_cell += 1 << (((c + 1) % 2) * 4);
        }

        if (l > 0) {
          Cell *top_cell = neighbour_count + (l - 1) * w / 2 + (c + 1) / 2;
          *top_cell += 1 << ((c % 2) * 4);
        }

        if (c < w - 1 && l > 0) {
          Cell *topright_cell = neighbour_count + (l - 1) * w / 2 + c / 2 + 1;
          *topright_cell += 1 << (((c + 1) % 2) * 4);
        }

        if (c > 0) {
          Cell *left_cell = neighbour_count + l * w / 2 + c / 2;
          *left_cell += 1 << (((c + 1) % 2) * 4);
        }

        if (c < w - 1) {
          Cell *right_cell = neighbour_count + l * w / 2 + c / 2 + 1;
          *right_cell += 1 << (((c + 1) % 2) * 4);
        }

        if (c > 0 && l < h - 1) {
          Cell *bottomleft_cell = neighbour_count + (l + 1) * w / 2 + c / 2;
          *bottomleft_cell += 1 << (((c + 1) % 2) * 4);
        }

        if (l < h - 1) {
          Cell *bottom_cell = neighbour_count + (l + 1) * w / 2 + (c + 1) / 2;
          *bottom_cell += 1 << ((c % 2) * 4);
        }

        if (c < w - 1 && l < h - 1) {
          Cell *bottomright_cell =
              neighbour_count + (l + 1) * w / 2 + c / 2 + 1;
          *bottomright_cell += 1 << (((c + 1) % 2) * 4);
        }
      }
    }
  }
}

Coord NaiveUniverse::read_rle_size(QFile *file, Coord *size) {
  while (!file->atEnd()) {
    QByteArray line = file->readLine();
    if (line[0] == '#') {
      continue;
    }
    if (line[0] == 'x') {
      QList<QByteArray> list = line.split(',');
      int width = ((list[0].split('='))[1]
      .simplified()).toInt();
      int height = ((list[1].split('='))[1]
      .simplified()).toInt();
      size_t len = 1 <<
      ((width > height) ? (size_t) log2(width) + 2
                        : (size_t) log2(height) + 2);
      *size = Coord(len, len);
      return Coord(width, height);
    }
  }
  return Coord(6);
}

void NaiveUniverse::read_rle_data(QFile *file, Coord boundingbox, Coord size) {
  QByteArray data("");
  while (!file->atEnd()) {
    QByteArray line = file->readLine();
    data.append(line);
  }

  data = data.simplified();
  int boundingbox_x = boundingbox.x.get_si();
  int boundingbox_y = boundingbox.y.get_si();

  int init_x = size.x.get_si() / 2 - boundingbox_x / 2;
  int init_y = size.y.get_si() / 2 - boundingbox_y / 2;
  int curr_x = init_x;
  int curr_y = init_y;

  QByteArray qs("");
  for (int i = 0; i < data.length(); ++i) {
    if (data[i] == '\0')
      continue;
    int q;
    if (data[i] == '$') {
      q = qs.isEmpty() ? 1 : qs.toInt();
      curr_y += q;
      curr_x = init_x;
      qs.clear();
    }
    if (data[i] >= '0' && data[i] <= '9') {
      qs.append(data[i]);
    }
    if (data[i] == 'o' || data[i] == 'b') {
      q = qs.isEmpty() ? 1 : qs.toInt();
      for (int n = 0; n < q; n++) {
        set(Coord(curr_x, curr_y), data[i] == 'o');
        curr_x++;
      }
      qs.clear();
    }
    if (data[i] == '!')
      break;
  }
}

const Coord NaiveUniverse::get_size() const {
  return Coord(BigInt(width), BigInt(height));
}