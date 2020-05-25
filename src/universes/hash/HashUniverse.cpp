#include "model/BigInt.h"
#include "model/Vec2.h"
#include "model/mcell/MCellFile.h"
#include <universes/hash/HashUniverse.h>
#include <unordered_set>
#include <vector>

// Constructors
HashUniverse::HashUniverse(size_t top_level, Vec2 top_left)
    : _top_left(top_left), _top_level(top_level), _step_size_maximized(false) {
  _macrocell_sets.resize(_top_level + 1);
  _zeros.push_back(nullptr);
  _zeros.push_back(reinterpret_cast<Quadrant *>(_minicell()));
  for (size_t i = 2; i <= _top_level; ++i) {
    _zeros.push_back(reinterpret_cast<Quadrant *>(_macrocell(i)));
  }

  _root = reinterpret_cast<MacroCell *>(_zeros.back());
  _recursion_depth = 1;
  _step_size = 1;
}

HashUniverse::HashUniverse(QString filename, Vec2 _top_left)
    : _top_left(_top_left), _step_size_maximized(false) {
  QFile file(filename);
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    return;
  QString format = filename.split(".")[1].simplified();
  if (format == "rle") {
    _build_from_rle(&file);
  } else if (format == "mc") {
    _build_from_mc(&file);
  }
  _recursion_depth = 1;
  _step_size = 1;
}

// Interface
void HashUniverse::update() {

  _grow_to(_top_level + 2);

  // Setting the step size
  // Step size either grows with each step or stays user-defined.
  // Calculate result of universe
  _root = reinterpret_cast<MacroCell *>(_result(_top_level--, _root));


  Quadrant *z = _zeros[_top_level - 2];

  // Checking for crown
  if (_root->nw->macrocell.se == z && _root->nw->macrocell.nw == z &&
      _root->nw->macrocell.ne == z && _root->ne->macrocell.nw == z &&
      _root->ne->macrocell.ne == z && _root->ne->macrocell.se == z &&
      _root->se->macrocell.ne == z && _root->se->macrocell.se == z &&
      _root->se->macrocell.sw == z && _root->sw->macrocell.se == z &&
      _root->sw->macrocell.sw == z && _root->sw->macrocell.nw == z) {
    _top_level--;
    _root =
        _macrocell(_top_level, _root->nw->macrocell.se, _root->ne->macrocell.sw,
                   _root->sw->macrocell.ne, _root->se->macrocell.nw);
  } else {
    _top_left -= Vec2(_top_level - 2);
  }


  _generation += _step_size;

  // Calculate new step_size
  if (_step_size_maximized) {
	_recursion_depth = _top_level;
    _step_size = BigInt(1) << (_recursion_depth - 2);
  }
}

Rect &HashUniverse::bounds() {
  _bounds.top_left = _top_left;
  _bounds.size.x = BigInt(1) << mp_size_t(_top_level);
  _bounds.size.y = BigInt(1) << mp_size_t(_top_level);
  return _bounds;
}

const BigInt &HashUniverse::generation() const { return _generation; }

const BigInt &HashUniverse::step_size() const { return _step_size; }

const bool HashUniverse::can_set_step_size() const {
	return true;
}

std::pair<Rect, size_t> HashUniverse::get_pattern_bounding_box() {
  Rect r = bounds();
  size_t l = _pattern_bounding_box_rec(&r, _top_level,
                                      reinterpret_cast<Quadrant *>(_root));

  std::pair<Rect, size_t> pair(r, l);
  return pair;
}

size_t HashUniverse::_pattern_bounding_box_rec(Rect *box,
                              size_t level, Quadrant *q) {
  if (level <= 4 || q == _zeros[level]) {
    return level;
  } else {
    Quadrant *z1 = _zeros[level - 1];
    Quadrant *z2 = _zeros[level - 2];
    bool nw = q->macrocell.nw != z1;
    bool ne = q->macrocell.ne != z1;
    bool sw = q->macrocell.sw != z1;
    bool se = q->macrocell.se != z1;
    if (nw && !ne && !sw && !se) {
      box->size.x -= BigInt(1) << mp_size_t(level - 1);
      box->size.y -= BigInt(1) << mp_size_t(level - 1);
      return _pattern_bounding_box_rec(box, level - 1, q->macrocell.nw);
    } else if (!nw && ne && !sw && !se) {
      box->top_left.x += BigInt(1) << mp_size_t(level - 1);
      box->size.x -= BigInt(1) << mp_size_t(level - 1);
      box->size.y -= BigInt(1) << mp_size_t(level - 1);
      return _pattern_bounding_box_rec(box, level - 1, q->macrocell.ne);
    } else if (!nw && !ne && sw && !se) {
      box->top_left.y += BigInt(1) << mp_size_t(level - 1);
      box->size.y -= BigInt(1) << mp_size_t(level - 1);
      box->size.x -= BigInt(1) << mp_size_t(level - 1);
      return _pattern_bounding_box_rec(box, level - 1, q->macrocell.sw);
    } else if (!nw && !ne && !sw && se) {
      box->top_left.x += BigInt(1) << mp_size_t(level - 1);
      box->top_left.y += BigInt(1) << mp_size_t(level - 1);
      box->size.x -= BigInt(1) << mp_size_t(level - 1);
      box->size.y -= BigInt(1) << mp_size_t(level - 1);
      return _pattern_bounding_box_rec(box, level - 1, q->macrocell.se);
    }
    /*
    else if (q->macrocell.nw->macrocell.se == z2 && q->macrocell.nw->macrocell.nw == z2 &&
        q->macrocell.nw->macrocell.ne == z2 && q->macrocell.ne->macrocell.nw == z2 &&
        q->macrocell.ne->macrocell.ne == z2 && q->macrocell.ne->macrocell.se == z2 &&
        q->macrocell.se->macrocell.ne == z2 && q->macrocell.se->macrocell.se == z2 &&
        q->macrocell.se->macrocell.sw == z2 && q->macrocell.sw->macrocell.se == z2 &&
        q->macrocell.sw->macrocell.sw == z2 && q->macrocell.sw->macrocell.nw == z2 &&
        q->macrocell.nw->macrocell.se != z2 && q->macrocell.ne->macrocell.sw != z2 &&
        q->macrocell.sw->macrocell.ne != z2 && q->macrocell.se->macrocell.nw != z2) {
          std::cout << "CENTER" << std::endl;
          MacroCell q2(q->macrocell.nw->macrocell.se, q->macrocell.ne->macrocell.sw,
                      q->macrocell.sw->macrocell.ne, q->macrocell.se->macrocell.nw);
          box->top_left.x += BigInt(1) << mp_size_t(level - 2);
          box->top_left.y += BigInt(1) << mp_size_t(level - 2);
          box->bottom_right.x -= BigInt(1) << mp_size_t(level - 2);
          box->bottom_right.y -= BigInt(1) << mp_size_t(level - 2);
          pattern_bounding_box_rec(box, level - 1, reinterpret_cast<Quadrant *>(&q2));
        }
      */
  }
  return level;
}
// Loading methods
void HashUniverse::_build_from_rle(QFile *file) {
  Vec2 boundingbox = _read_rle_size(file);
  // Starting with one extra level
  _macrocell_sets.resize(_top_level + 1);
  _zeros.push_back(nullptr);
  _zeros.push_back(reinterpret_cast<Quadrant *>(_minicell()));
  for (size_t i = 2; i <= _top_level; ++i) {
    _zeros.push_back(reinterpret_cast<Quadrant *>(_macrocell(i)));
  }
  _root = reinterpret_cast<MacroCell *>(_zeros.back());

  _read_rle_data(file, boundingbox);
  file->close();

  _recursion_depth = _top_level - 2;
}

void HashUniverse::_build_from_mc(QFile *file) {
  //  vector will store the pointers to the quadrants
  std::vector<Quadrant *> mcells;
  // mcells[0] should never be accessed
  mcells.push_back(nullptr);
  size_t max_top_level = 3;
  _macrocell_sets.resize(max_top_level + 1);
  _zeros.push_back(nullptr);
  _zeros.push_back(reinterpret_cast<Quadrant *>(_minicell()));
  _zeros.push_back(reinterpret_cast<Quadrant *>(_macrocell(2)));
  _zeros.push_back(reinterpret_cast<Quadrant *>(_macrocell(3)));

  MCellFile m_file(file);

  MCellFile::LineIter iter = m_file.iter_line();
  MCellFile::LineIter::Line line;
  MCellFile::LineIter::LineType line_type;

  while (iter.next(&line, &line_type) != MCellFile::LineIter::LineType::DONE) {
    if (line_type == MCellFile::LineIter::LineType::MACRO_3) {
      auto mcell_info = line.level3;
      size_t level = 3;
      if (level > max_top_level) {
        _macrocell_sets.resize(level + 1);
        for (size_t i = max_top_level + 1; i <= level; ++i) {
          _zeros.push_back(reinterpret_cast<Quadrant *>(_macrocell(i)));
        }
        max_top_level = level;
      }
      Quadrant *minis[4][4];
      for (size_t j = 0; j < 4; ++j) {
        for (size_t i = 0; i < 4; ++i) {
          minis[i][j] = reinterpret_cast<Quadrant *>(
              _minicell(mcell_info.cells[2 * i][2 * j],
                        mcell_info.cells[2 * i + 1][2 * j],
                        mcell_info.cells[2 * i][2 * j + 1],
                        mcell_info.cells[2 * i + 1][2 * j + 1]));
        }
      }
      mcells.push_back(reinterpret_cast<Quadrant *>(_macrocell(
          3,
          reinterpret_cast<Quadrant *>(_macrocell(2, minis[0][0], minis[1][0],
                                                  minis[0][1], minis[1][1])),
          reinterpret_cast<Quadrant *>(_macrocell(2, minis[2][0], minis[3][0],
                                                  minis[2][1], minis[3][1])),
          reinterpret_cast<Quadrant *>(_macrocell(2, minis[0][2], minis[1][2],
                                                  minis[0][3], minis[1][3])),
          reinterpret_cast<Quadrant *>(_macrocell(2, minis[2][2], minis[3][2],
                                                  minis[2][3], minis[3][3])))));
    } else if (line_type == MCellFile::LineIter::LineType::MACRO_N) {
      auto mcell_info = line.levelN;
      if (mcell_info.level > max_top_level) {
        _macrocell_sets.resize(mcell_info.level + 1);
        for (size_t i = max_top_level + 1; i <= mcell_info.level; ++i) {
          _zeros.push_back(reinterpret_cast<Quadrant *>(_macrocell(i)));
        }
        max_top_level = mcell_info.level;
      }
      Quadrant *nw, *ne, *sw, *se;
      nw = (mcell_info.nw_index == 0) ? _zeros[mcell_info.level - 1] :
                                        mcells[mcell_info.nw_index];
      ne = (mcell_info.ne_index == 0) ? _zeros[mcell_info.level - 1] :
                                        mcells[mcell_info.ne_index];
      sw = (mcell_info.sw_index == 0) ? _zeros[mcell_info.level - 1] :
                                        mcells[mcell_info.sw_index];
      se = (mcell_info.se_index == 0) ? _zeros[mcell_info.level - 1] :
                                        mcells[mcell_info.se_index];
      mcells.push_back(
          reinterpret_cast<Quadrant *>(
            _macrocell(mcell_info.level, nw, ne, sw, se)));
    } else {
      throw "Unexpected line type";
    }
  }

  file->close();
  _top_level = max_top_level;
  _recursion_depth = _top_level - 2;
  _root = reinterpret_cast<MacroCell *>(mcells.back());
  //_step_size = BigInt(1) << mp_size_t(_recursion_depth - 2);
}

Vec2 HashUniverse::_read_rle_size(QFile *file) {
  while (!file->atEnd()) {
    QByteArray line = file->readLine();
    if (line[0] == '#') {
      continue;
    }
    if (line[0] == 'x') {
      QList<QByteArray> list = line.split(',');
      int width = ((list[0].split('='))[1].simplified()).toInt();
      int height = ((list[1].split('='))[1].simplified()).toInt();
      _top_level =
          (width > height) ? (size_t)log2(width) + 1 : (size_t)log2(height) + 1;
      return Vec2(width, height);
    }
  }
  return Vec2(6);
}

void HashUniverse::_read_rle_data(QFile *file, Vec2 boundingbox) {
  QByteArray data("");
  while (!file->atEnd()) {
    QByteArray line = file->readLine();
    data.append(line);
  }

  data = data.simplified();
  int boundingbox_x = boundingbox.x.get_si();
  int boundingbox_y = boundingbox.y.get_si();

  int init_x = (1 << (_top_level - 1)) - boundingbox_x / 2;
  int init_y = (1 << (_top_level - 1)) + boundingbox_y / 2;
  int curr_x = init_x;
  int curr_y = init_y;

  QByteArray qs("");
  for (int i = 0; i < data.length(); ++i) {
    if (data[i] == '\0')
      continue;
    int q;
    if (data[i] == '$') {
      q = qs.isEmpty() ? 1 : qs.toInt();
      curr_y -= q;
      curr_x = init_x;
      qs.clear();
    }
    if (data[i] >= '0' && data[i] <= '9') {
      qs.append(data[i]);
    }
    if (data[i] == 'o' || data[i] == 'b') {
      q = qs.isEmpty() ? 1 : qs.toInt();
      for (int n = 0; n < q; n++) {
        set(Vec2(curr_x, curr_y), data[i] == 'o');
        curr_x++;
      }
      qs.clear();
    }
    if (data[i] == '!')
      break;
  }
}

void HashUniverse::_assert_handles(size_t asserted_level) {
  if (asserted_level >= _macrocell_sets.size()) {
    size_t previous_asserted_level = _macrocell_sets.size() - 1;

    _macrocell_sets.resize(asserted_level + 1);
    for (size_t level = previous_asserted_level + 1; level <= asserted_level;
         ++level)
      _zeros.push_back(reinterpret_cast<Quadrant *>(_macrocell(level)));
  }
}

const CellState HashUniverse::get(const Vec2& target) const {
  if (target.y >= _top_left.y &&
      target.y < _top_left.y + (BigInt(1) << mp_size_t(_top_level)) &&
      target.x >= _top_left.x &&
      target.x < _top_left.x + (BigInt(1) << mp_size_t(_top_level)))
    return *_find(target);
  else
    return 0;
}

void HashUniverse::set(const Vec2& target, CellState state) {
  _root = reinterpret_cast<MacroCell *>(
      _set_rec(_top_left, _top_level, reinterpret_cast<Quadrant *>(_root),
               target, state));
}

Quadrant *HashUniverse::_set_rec(Vec2 current, size_t level, Quadrant *cell,
                                     Vec2 target, CellState state) {
  Vec2 size(level - 1);
  Vec2 center = current + size;

  if (level == 1) {
    if (target.y < center.y) {
      if (target.x < center.x)
        return reinterpret_cast<Quadrant *>(_minicell(
            state, cell->minicell.ne, cell->minicell.sw, cell->minicell.se));
      else
        return reinterpret_cast<Quadrant *>(_minicell(
            cell->minicell.nw, state, cell->minicell.sw, cell->minicell.se));
    } else {
      if (target.x < center.x)
        return reinterpret_cast<Quadrant *>(_minicell(
            cell->minicell.nw, cell->minicell.ne, state, cell->minicell.se));
      else
        return reinterpret_cast<Quadrant *>(_minicell(
            cell->minicell.nw, cell->minicell.ne, cell->minicell.sw, state));
    }
  } else {
    level -= 1;
    if (target.y < center.y) {
      if (target.x < center.x) {
        return reinterpret_cast<Quadrant *>(_macrocell(
            level, _set_rec(current, level, cell->macrocell.nw, target, state),
            cell->macrocell.ne, cell->macrocell.sw, cell->macrocell.se));
      } else {
        current.x = center.x;
        return reinterpret_cast<Quadrant *>(_macrocell(
            level, cell->macrocell.nw,
            _set_rec(current, level, cell->macrocell.ne, target, state),
            cell->macrocell.sw, cell->macrocell.se));
      }
    } else {
      current.y = center.y;
      if (target.x < center.x) {
        return reinterpret_cast<Quadrant *>(_macrocell(
            level, cell->macrocell.nw, cell->macrocell.ne,
            _set_rec(current, level, cell->macrocell.sw, target, state),
            cell->macrocell.se));
      } else {
        current.x = center.x;
        return reinterpret_cast<Quadrant *>(_macrocell(
            level, cell->macrocell.nw, cell->macrocell.ne, cell->macrocell.sw,
            _set_rec(current, level, cell->macrocell.se, target, state)));
      }
    }
  }
}

CellState *HashUniverse::_find(Vec2 target) const {
  Quadrant *cell = reinterpret_cast<Quadrant *>(_root);
  Vec2 size(_top_level - 1);
  Vec2 current(_top_left);
  Vec2 center = current + size;
  for (size_t level = _top_level; level > 1; --level) {
    if (target.y < center.y) {
      if (target.x < center.x) {
        cell = cell->macrocell.nw;
      } else {
        current.x = center.x;
        cell = cell->macrocell.ne;
      }
    } else {
      current.y = center.y;
      if (target.x < center.x) {
        cell = cell->macrocell.sw;
      } else {
        current.x = center.x;
        cell = cell->macrocell.se;
      }
    }
    size >>= 1;
    center = current + size;
  }

  if (target.y < center.y) {
    if (target.x < center.x) {
      return &cell->minicell.nw;
    }
    return &cell->minicell.ne;
  } else {
    if (target.x < center.x) {
      return &cell->minicell.sw;
    }
    return &cell->minicell.se;
  }
}

CellState *HashUniverse::_find_path(Vec2 coord,
                                        std::vector<Quadrant *> *path) const {
  Quadrant *cell = reinterpret_cast<Quadrant *>(_root);
  Vec2 size(_top_level - 1);
  Vec2 current(_top_left);
  Vec2 center = current + size;
  for (size_t level = _top_level; level > 1; --level) {
    path->push_back(cell);
    if (coord.x < center.x) {
      if (coord.y < center.y) {
        cell = cell->macrocell.nw;
      } else {
        current.y += size.y;
        cell = cell->macrocell.ne;
      }
    } else {
      current.x += size.x;
      if (coord.y < center.y) {
        cell = cell->macrocell.sw;
      } else {
        current.y += size.y;
        cell = cell->macrocell.se;
      }
    }
    size >>= 1;
    center = current + size;
  }

  if (coord.y < center.y) {
    if (coord.x < center.x) {
      return &cell->minicell.nw;
    }
    return &cell->minicell.ne;
  } else {
    if (coord.x < center.x) {
      return &cell->minicell.sw;
    }
    return &cell->minicell.se;
  }
}

MiniCell *HashUniverse::_minicell() {
  return const_cast<MiniCell *>(
      reinterpret_cast<const MiniCell *>(&*_minicells.emplace().first));
}

MiniCell *HashUniverse::_minicell(CellState nw, CellState ne, CellState sw,
                                      CellState se) {
  return const_cast<MiniCell *>(reinterpret_cast<const MiniCell *>(
      &*_minicells.emplace(nw, ne, sw, se).first));
}

MacroCell *HashUniverse::_macrocell(size_t level) {
  return const_cast<MacroCell *>(reinterpret_cast<const MacroCell *>(
      &*_macrocell_sets[level]
            .emplace(_zeros[level - 1], _zeros[level - 1], _zeros[level - 1],
                     _zeros[level - 1])
            .first));
}

MacroCell *HashUniverse::_macrocell(size_t level, Quadrant *nw,
                                        Quadrant *ne, Quadrant *sw,
                                        Quadrant *se) {
  return const_cast<MacroCell *>(reinterpret_cast<const MacroCell *>(
      &*_macrocell_sets[level].emplace(nw, ne, sw, se).first));
}

Quadrant *HashUniverse::_quadrant(size_t level) {
  if (level <= 1) {
    return reinterpret_cast<Quadrant *>(_minicell());
  } else {
    return reinterpret_cast<Quadrant *>(
        _macrocell(level, _quadrant(level - 1), _quadrant(level - 1),
                   _quadrant(level - 1), _quadrant(level - 1)));
  }
}

size_t HashUniverse::get_top_level() { return _top_level; }

Vec2 HashUniverse::get_top_left() { return _top_left; }

void HashUniverse::_crown() {
  Quadrant *zero = _zeros[_top_level - 1];

  MacroCell *nw = _macrocell(_top_level, zero, zero, zero, _root->nw);
  MacroCell *ne = _macrocell(_top_level, zero, zero, _root->ne, zero);
  MacroCell *sw = _macrocell(_top_level, zero, _root->sw, zero, zero);
  MacroCell *se = _macrocell(_top_level, _root->se, zero, zero, zero);

  _top_level++;
  _root = _macrocell(_top_level, reinterpret_cast<Quadrant *>(nw),
                     reinterpret_cast<Quadrant *>(ne),
                     reinterpret_cast<Quadrant *>(sw),
                     reinterpret_cast<Quadrant *>(se));
}

Quadrant *HashUniverse::_result(size_t level, MacroCell *macrocell_tmp) {
  bool step_size_lock = level > _recursion_depth;

  if (macrocell_tmp->result != nullptr) {
    if (step_size_lock && !macrocell_tmp->result_advances_in_time)
      return macrocell_tmp->result;

    if (!step_size_lock && macrocell_tmp->result_advances_in_time)
      return macrocell_tmp->result;
  }

  if (level == 2) {
    int nb_nw =
        macrocell_tmp->nw->minicell.nw + macrocell_tmp->nw->minicell.ne +
        macrocell_tmp->ne->minicell.nw + macrocell_tmp->nw->minicell.sw +
        macrocell_tmp->ne->minicell.sw + macrocell_tmp->sw->minicell.nw +
        macrocell_tmp->sw->minicell.ne + macrocell_tmp->se->minicell.nw;

    int nb_ne =
        macrocell_tmp->nw->minicell.ne + macrocell_tmp->ne->minicell.nw +
        macrocell_tmp->ne->minicell.ne + macrocell_tmp->nw->minicell.se +
        macrocell_tmp->ne->minicell.se + macrocell_tmp->sw->minicell.ne +
        macrocell_tmp->se->minicell.nw + macrocell_tmp->se->minicell.ne;

    int nb_sw =
        macrocell_tmp->nw->minicell.sw + macrocell_tmp->nw->minicell.se +
        macrocell_tmp->ne->minicell.sw + macrocell_tmp->sw->minicell.nw +
        macrocell_tmp->se->minicell.nw + macrocell_tmp->sw->minicell.sw +
        macrocell_tmp->sw->minicell.se + macrocell_tmp->se->minicell.sw;

    int nb_se =
        macrocell_tmp->nw->minicell.se + macrocell_tmp->ne->minicell.sw +
        macrocell_tmp->ne->minicell.se + macrocell_tmp->sw->minicell.ne +
        macrocell_tmp->se->minicell.ne + macrocell_tmp->sw->minicell.se +
        macrocell_tmp->se->minicell.sw + macrocell_tmp->se->minicell.se;

    CellState new_nw;
    CellState new_ne;
    CellState new_sw;
    CellState new_se;
    // I check if the cell is alive
    if (macrocell_tmp->nw->minicell.se) {
      // If the cell is alive, i check if cell survive
      if ((nb_nw == 2) || (nb_nw == 3)) {
        new_nw = 1;
      } else {
        new_nw = 0;
      }
    } else {
      // if the cell is dead, i check if cell born
      if (nb_nw == 3) {
        new_nw = 1;
      } else {
        new_nw = 0;
      }
    }

    // I check if the cell is alive
    if (macrocell_tmp->ne->minicell.sw) {
      // If the cell is alive, i check if cell survive
      if ((nb_ne == 2) || (nb_ne == 3)) {
        new_ne = 1;
      } else {
        new_ne = 0;
      }
    } else {
      // if the cell is dead, i check if cell born
      if (nb_ne == 3) {
        new_ne = 1;
      } else {
        new_ne = 0;
      }
    }

    // I check if the cell is alive
    if (macrocell_tmp->sw->minicell.ne) {
      // If the cell is alive, i check if cell survive
      if ((nb_sw == 2) || (nb_sw == 3)) {
        new_sw = 1;
      } else {
        new_sw = 0;
      }
    } else {
      // if the cell is dead, i check if cell born
      if (nb_sw == 3) {
        new_sw = 1;
      } else {
        new_sw = 0;
      }
    }

    // I check if the cell is alive
    if (macrocell_tmp->se->minicell.nw) {
      // If the cell is alive, i check if cell survive
      if ((nb_se == 2) || (nb_se == 3)) {
        new_se = 1;
      } else {
        new_se = 0;
      }
    } else {
      // if the cell is dead, i check if cell born
      if (nb_se == 3) {
        new_se = 1;
      } else {
        new_se = 0;
      }
    }

    macrocell_tmp->result =
        reinterpret_cast<Quadrant *>(_minicell(new_nw, new_ne, new_sw, new_se));

    macrocell_tmp->result_advances_in_time = true;

    return macrocell_tmp->result;
  } else {
    // voir figure 4 de :
    // https://www.drdobbs.com/jvm/an-algorithm-for-compressing-space-and-t/184406478

    // a partir de ma macroCell de taille n,
    // je vais faire 6 assemblages différents de macroCell de taille n-2
    // afin de pouvoir avoir les 4 carrés centraux (de taille n-2), puis
    // reconstruire la cellules centrale (n-1).

    Quadrant *temp_nw =
        _result(level - 1, reinterpret_cast<MacroCell *>(macrocell_tmp->nw));

    Quadrant *temp_n = _result(
        level - 1, _macrocell(level - 1, macrocell_tmp->nw->macrocell.ne,
                              macrocell_tmp->ne->macrocell.nw,
                              macrocell_tmp->nw->macrocell.se,
                              macrocell_tmp->ne->macrocell.sw));

    Quadrant *temp_ne =
        _result(level - 1, reinterpret_cast<MacroCell *>(macrocell_tmp->ne));

    Quadrant *temp_w = _result(
        level - 1, _macrocell(level - 1, macrocell_tmp->nw->macrocell.sw,
                              macrocell_tmp->nw->macrocell.se,
                              macrocell_tmp->sw->macrocell.nw,
                              macrocell_tmp->sw->macrocell.ne));

    Quadrant *temp_c = _result(
        level - 1, _macrocell(level - 1, macrocell_tmp->nw->macrocell.se,
                              macrocell_tmp->ne->macrocell.sw,
                              macrocell_tmp->sw->macrocell.ne,
                              macrocell_tmp->se->macrocell.nw));

    Quadrant *temp_e = _result(
        level - 1, _macrocell(level - 1, macrocell_tmp->ne->macrocell.sw,
                              macrocell_tmp->ne->macrocell.se,
                              macrocell_tmp->se->macrocell.nw,
                              macrocell_tmp->se->macrocell.ne));

    Quadrant *temp_sw =
        _result(level - 1, reinterpret_cast<MacroCell *>(macrocell_tmp->sw));

    Quadrant *temp_s = _result(
        level - 1, _macrocell(level - 1, macrocell_tmp->sw->macrocell.ne,
                              macrocell_tmp->se->macrocell.nw,
                              macrocell_tmp->sw->macrocell.se,
                              macrocell_tmp->se->macrocell.sw));

    Quadrant *temp_se =
        _result(level - 1, reinterpret_cast<MacroCell *>(macrocell_tmp->se));
    if (!step_size_lock) {
      Quadrant *res_nw = _result(
          level - 1, _macrocell(level - 1, temp_nw, temp_n, temp_w, temp_c));
      Quadrant *res_ne = _result(
          level - 1, _macrocell(level - 1, temp_n, temp_ne, temp_c, temp_e));
      Quadrant *res_sw = _result(
          level - 1, _macrocell(level - 1, temp_w, temp_c, temp_sw, temp_s));
      Quadrant *res_se = _result(
          level - 1, _macrocell(level - 1, temp_c, temp_e, temp_s, temp_se));

      macrocell_tmp->result_advances_in_time = true;

      macrocell_tmp->result = reinterpret_cast<Quadrant *>(
          _macrocell(level - 1, res_nw, res_ne, res_sw, res_se));
    } else {
      Quadrant *res_nw;
      Quadrant *res_ne;
      Quadrant *res_sw;
      Quadrant *res_se;

      if (level == 3) {
        res_nw = reinterpret_cast<Quadrant *>(
            _minicell(temp_nw->minicell.se, temp_n->minicell.sw,
                      temp_w->minicell.ne, temp_c->minicell.nw));

        res_ne = reinterpret_cast<Quadrant *>(
            _minicell(temp_n->minicell.se, temp_ne->minicell.sw,
                      temp_c->minicell.ne, temp_e->minicell.nw));

        res_sw = reinterpret_cast<Quadrant *>(
            _minicell(temp_w->minicell.se, temp_c->minicell.sw,
                      temp_sw->minicell.ne, temp_s->minicell.nw));

        res_se = reinterpret_cast<Quadrant *>(
            _minicell(temp_c->minicell.se, temp_e->minicell.sw,
                      temp_s->minicell.ne, temp_se->minicell.nw));
      } else {
        res_nw = reinterpret_cast<Quadrant *>(
            _macrocell(level - 2, temp_nw->macrocell.se, temp_n->macrocell.sw,
                       temp_w->macrocell.ne, temp_c->macrocell.nw));

        res_ne = reinterpret_cast<Quadrant *>(
            _macrocell(level - 2, temp_n->macrocell.se, temp_ne->macrocell.sw,
                       temp_c->macrocell.ne, temp_e->macrocell.nw));
        res_sw = reinterpret_cast<Quadrant *>(
            _macrocell(level - 2, temp_w->macrocell.se, temp_c->macrocell.sw,
                       temp_sw->macrocell.ne, temp_s->macrocell.nw));
        res_se = reinterpret_cast<Quadrant *>(
            _macrocell(level - 2, temp_c->macrocell.se, temp_e->macrocell.sw,
                       temp_s->macrocell.ne, temp_se->macrocell.nw));
      }

      // DEGEULASSE
      macrocell_tmp->result = reinterpret_cast<Quadrant *>(
          _macrocell(level - 1, res_nw, res_ne, res_sw, res_se));

      macrocell_tmp->result_advances_in_time = false;
      return reinterpret_cast<Quadrant *>(
          _macrocell(level - 1, res_nw, res_ne, res_sw, res_se));
    }
    return macrocell_tmp->result;
  }
}

void HashUniverse::_get_cell_in_bounds_rec(Rect bounds,
                                              std::vector<Vec2> *coords,
                                              size_t current_level,
                                              Quadrant *current_cell,
                                              Vec2 current_coord,
                                              size_t min_level) const {
  BigInt x = current_coord.x;
  BigInt y = current_coord.y;

  if (current_level == min_level) {
    if (min_level == 1) {
      MiniCell minicell = current_cell->minicell;
      if (bounds & Vec2{x, y}) {
        if (minicell.nw)
          coords->push_back({x, y});
      }

      if (bounds & Vec2{x + 1, y}) {
        if (minicell.ne)
          coords->push_back({x + 1, y});
      }

      if (bounds & Vec2{x, y + 1}) {
        if (minicell.sw)
          coords->push_back({x, y + 1});
      }

      if (bounds & Vec2{x + 1, y + 1}) {
        if (minicell.se)
          coords->push_back({x + 1, y + 1});
      }
    } else {
      BigInt size = BigInt(1) << mp_size_t(current_level - 1);
      if (current_cell->macrocell.nw != _zeros[current_level])
        coords->push_back({x, y});
      if (current_cell->macrocell.ne != _zeros[current_level])
        coords->push_back({x + size, y});
      if (current_cell->macrocell.sw != _zeros[current_level])
        coords->push_back({x, y + size});
      if (current_cell->macrocell.se != _zeros[current_level])
        coords->push_back({x + size, y + size});
    }
  } else {
    MacroCell macrocell = current_cell->macrocell;
    if (!(macrocell == _zeros[current_level]->macrocell)) {
      BigInt size = BigInt(1) << mp_size_t(current_level - 1);
      if (bounds & Rect{{x, y}, {size, size}}) {
        _get_cell_in_bounds_rec(bounds, coords, current_level - 1, macrocell.nw,
                               {x, y}, min_level);
      }

      if (bounds & Rect{{x + size, y}, {size, size}}) {
        _get_cell_in_bounds_rec(bounds, coords, current_level - 1, macrocell.ne,
                               {x + size, y}, min_level);
      }

      if (bounds & Rect{{x, y + size}, {size, size}}) {
        _get_cell_in_bounds_rec(bounds, coords, current_level - 1, macrocell.sw,
                               {x, y + size}, min_level);
      }

      if (bounds & Rect{{x + size, y + size},
                           {size, size}}) {
        _get_cell_in_bounds_rec(bounds, coords, current_level - 1, macrocell.se,
                               {x + size, y + size}, min_level);
      }
    }
  }
}

void HashUniverse::get_cell_in_bounds(Rect bounds,
                                      std::vector<Vec2> *coords,
                                      size_t min_level) const {
  _get_cell_in_bounds_rec(bounds, coords, _top_level,
          reinterpret_cast<Quadrant *>(_root), _top_left, min_level);
}

void HashUniverse::set_step_size(size_t new_step_size) {
  _step_size_maximized = false;
  _step_size = BigInt(1) << new_step_size; // BigInt (interface)
  _recursion_depth = new_step_size + 2;
  if (_recursion_depth > _top_level)
	_grow_to(_recursion_depth);
}

void HashUniverse::set_hyperspeed(bool on) {
  _step_size_maximized = on;
  _recursion_depth = _top_level;
  _step_size = BigInt(1) << (_recursion_depth - 2);
}

const bool HashUniverse::can_set_hyperspeed() const {
  return true;
}

void HashUniverse::_grow_to(size_t target) {
  _assert_handles(target);
  while (_top_level < target) {
	  _crown(); // Increase Universe level one fold
  }
}
