#include <logic/HashlifeUniverse/HashlifeUniverse.h>
// #include <iterator>
#include <unordered_set>

HashlifeUniverse::HashlifeUniverse(size_t top_level, Coord top_left)
    : Universe(top_level, top_left),
    top_left(top_left), top_level(top_level) , step_size_maximized(true) {
  macrocell_sets.resize(top_level + 1);
  zeros.push_back(nullptr);
  zeros.push_back(reinterpret_cast<Quadrant *>(minicell()));
  for (size_t i = 2; i <= top_level; ++i) {
    zeros.push_back(reinterpret_cast<Quadrant *>(macrocell(i)));
  }

  root = reinterpret_cast<MacroCell *>(zeros.back());
}

HashlifeUniverse::HashlifeUniverse(QString filename, Coord top_left)
    : Universe(filename, top_left),
    top_left(top_left), step_size_maximized(true) {
  QFile file(filename);
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    return;

  Coord boundingbox = read_rle_size(&file);
  // Starting with one extra level
  macrocell_sets.resize(top_level + 1);
  zeros.push_back(nullptr);
  zeros.push_back(reinterpret_cast<Quadrant *>(minicell()));
  for (size_t i = 2; i <= top_level; ++i) {
    zeros.push_back(reinterpret_cast<Quadrant *>(macrocell(i)));
  }
  root = reinterpret_cast<MacroCell *>(zeros.back());

  read_rle_data(&file, boundingbox);
  file.close();

  step_size = top_level - 2;
}

void HashlifeUniverse::debug() {
  (reinterpret_cast<Quadrant *>(root))->debug(top_level);
}

Coord HashlifeUniverse::read_rle_size(QFile *file) {
  while (!file->atEnd()) {
    QByteArray line = file->readLine();
    if (line[0] == '#') {
      continue;
    }
    if (line[0] == 'x') {
      QList<QByteArray> list = line.split(',');
      int width = ((list[0].split('='))[1].simplified()).toInt();
      int height = ((list[1].split('='))[1].simplified()).toInt();
      top_level =
          (width > height) ? (size_t)log2(width) + 1 : (size_t)log2(height) + 1;
      return Coord(width, height);
    }
  }
  return Coord(6);
}

void HashlifeUniverse::read_rle_data(QFile *file, Coord boundingbox) {
  QByteArray data("");
  while (!file->atEnd()) {
    QByteArray line = file->readLine();
    data.append(line);
  }

  data = data.simplified();
  int boundingbox_x = boundingbox.x.get_si();
  int boundingbox_y = boundingbox.y.get_si();

  int init_x = (1 << (top_level - 1)) - boundingbox_x / 2;
  int init_y = (1 << (top_level - 1));
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
        set(Coord(curr_x, curr_y), data[i] == 'o');
        curr_x++;
      }
      qs.clear();
    }
    if (data[i] == '!')
      break;
  }
}

void HashlifeUniverse::assert_handles(size_t asserted_level) {
  if (asserted_level >= macrocell_sets.size()) {
    std::cout << "Asserted level greater\n";
    size_t previous_asserted_level = macrocell_sets.size() - 1;

    macrocell_sets.resize(asserted_level + 1);
    for (size_t level = previous_asserted_level + 1; level <= asserted_level;
         ++level)
      zeros.push_back(reinterpret_cast<Quadrant *>(macrocell(level)));
  }
}

void HashlifeUniverse::step() {
  // Assert that the creation of higher level is possible
  assert_handles(top_level + 2);
  // Add of crown of empty cell
  crown();
  crown();

  // Setting the step size
  // Step size either grows with each step or stays user-defined.
  if (step_size_maximized)
    step_size = top_level - 2;

  // Calculate result of universe
  root = reinterpret_cast<MacroCell *>(result(top_level--, root));

  generation_count += BigInt(1) << mp_size_t(step_size);

  Quadrant *z = zeros[top_level - 2];

  // Checking for crown
  if (root->nw->macrocell.se == z && root->nw->macrocell.nw == z &&
      root->nw->macrocell.ne == z && root->ne->macrocell.nw == z &&
      root->ne->macrocell.ne == z && root->ne->macrocell.se == z &&
      root->se->macrocell.ne == z && root->se->macrocell.se == z &&
      root->se->macrocell.sw == z && root->sw->macrocell.se == z &&
      root->sw->macrocell.sw == z && root->sw->macrocell.nw == z) {
    std::cout << "Allo ?" << '\n';
    top_level--;
    root = macrocell(top_level, root->nw->macrocell.se, root->ne->macrocell.sw,
                     root->sw->macrocell.ne, root->se->macrocell.nw);
  } else {
    top_left -= Coord(top_level - 2);
  }

  std::cout << "Universe size : " << top_level << '\n';
}

const CellState HashlifeUniverse::get(Coord target) const {
  if (target.y >= top_left.y &&
      target.y < top_left.y + (BigInt(1) << mp_size_t(top_level)) &&
      target.x >= top_left.x &&
      target.x < top_left.x + (BigInt(1) << mp_size_t(top_level)))
    return *find(target);
  else
    return 0;
}

void HashlifeUniverse::set(Coord target, CellState state) {
  root = reinterpret_cast<MacroCell *>(set_rec(
      top_left, top_level, reinterpret_cast<Quadrant *>(root), target, state));
}

Quadrant *HashlifeUniverse::set_rec(Coord current, size_t level, Quadrant *cell,
                                    Coord target, CellState state) {
  Coord size(level - 1);
  Coord center = current + size;

  if (level == 1) {
    if (target.y < center.y) {
      if (target.x < center.x)
        return reinterpret_cast<Quadrant *>(minicell(
            state, cell->minicell.ne, cell->minicell.sw, cell->minicell.se));
      else
        return reinterpret_cast<Quadrant *>(minicell(
            cell->minicell.nw, state, cell->minicell.sw, cell->minicell.se));
    } else {
      if (target.x < center.x)
        return reinterpret_cast<Quadrant *>(minicell(
            cell->minicell.nw, cell->minicell.ne, state, cell->minicell.se));
      else
        return reinterpret_cast<Quadrant *>(minicell(
            cell->minicell.nw, cell->minicell.ne, cell->minicell.sw, state));
    }
  } else {
    level -= 1;
    if (target.y < center.y) {
      if (target.x < center.x) {
        return reinterpret_cast<Quadrant *>(macrocell(
            level, set_rec(current, level, cell->macrocell.nw, target, state),
            cell->macrocell.ne, cell->macrocell.sw, cell->macrocell.se));
      } else {
        current.x = center.x;
        return reinterpret_cast<Quadrant *>(macrocell(
            level, cell->macrocell.nw,
            set_rec(current, level, cell->macrocell.ne, target, state),
            cell->macrocell.sw, cell->macrocell.se));
      }
    } else {
      current.y = center.y;
      if (target.x < center.x) {
        return reinterpret_cast<Quadrant *>(macrocell(
            level, cell->macrocell.nw, cell->macrocell.ne,
            set_rec(current, level, cell->macrocell.sw, target, state),
            cell->macrocell.se));
      } else {
        current.x = center.x;
        return reinterpret_cast<Quadrant *>(macrocell(
            level, cell->macrocell.nw, cell->macrocell.ne, cell->macrocell.sw,
            set_rec(current, level, cell->macrocell.se, target, state)));
      }
    }
  }
}

CellState *HashlifeUniverse::find(Coord target) const {
  Quadrant *cell = reinterpret_cast<Quadrant *>(root);
  Coord size(top_level - 1);
  Coord current(top_left);
  Coord center = current + size;
  for (size_t level = top_level; level > 1; --level) {
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

CellState *HashlifeUniverse::find_path(Coord coord,
                                       std::vector<Quadrant *> *path) const {
  Quadrant *cell = reinterpret_cast<Quadrant *>(root);
  Coord size(top_level - 1);
  Coord current(top_left);
  Coord center = current + size;
  for (size_t level = top_level; level > 1; --level) {
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

MiniCell *HashlifeUniverse::minicell() {
  return const_cast<MiniCell *>(
      reinterpret_cast<const MiniCell *>(&*minicells.emplace().first));
}

MiniCell *HashlifeUniverse::minicell(CellState nw, CellState ne, CellState sw,
                                     CellState se) {
  return const_cast<MiniCell *>(reinterpret_cast<const MiniCell *>(
      &*minicells.emplace(nw, ne, sw, se).first));
}

MacroCell *HashlifeUniverse::macrocell(size_t level) {
  return const_cast<MacroCell *>(reinterpret_cast<const MacroCell *>(
      &*macrocell_sets[level]
            .emplace(zeros[level - 1], zeros[level - 1], zeros[level - 1],
                     zeros[level - 1])
            .first));
}

MacroCell *HashlifeUniverse::macrocell(size_t level, Quadrant *nw, Quadrant *ne,
                                       Quadrant *sw, Quadrant *se) {
  return const_cast<MacroCell *>(reinterpret_cast<const MacroCell *>(
      &*macrocell_sets[level].emplace(nw, ne, sw, se).first));
}

Quadrant *HashlifeUniverse::quadrant(size_t level) {
  if (level <= 1) {
    return reinterpret_cast<Quadrant *>(minicell());
  } else {
    return reinterpret_cast<Quadrant *>(
        macrocell(level, quadrant(level - 1), quadrant(level - 1),
                  quadrant(level - 1), quadrant(level - 1)));
  }
}

size_t HashlifeUniverse::get_top_level() { return top_level; }

Coord HashlifeUniverse::get_top_left() { return top_left; }

void HashlifeUniverse::crown() {
  Quadrant *zero = zeros[top_level - 1];

  MacroCell *nw = macrocell(top_level, zero, zero, zero, root->nw);
  MacroCell *ne = macrocell(top_level, zero, zero, root->ne, zero);
  MacroCell *sw = macrocell(top_level, zero, root->sw, zero, zero);
  MacroCell *se = macrocell(top_level, root->se, zero, zero, zero);

  top_level++;
  root = macrocell(top_level, reinterpret_cast<Quadrant *>(nw),
                   reinterpret_cast<Quadrant *>(ne),
                   reinterpret_cast<Quadrant *>(sw),
                   reinterpret_cast<Quadrant *>(se));
}

Quadrant *HashlifeUniverse::result(size_t level, MacroCell *macrocell_tmp) {
  bool step_size_lock = level > step_size + 2;

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
        reinterpret_cast<Quadrant *>(minicell(new_nw, new_ne, new_sw, new_se));

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
        result(level - 1, reinterpret_cast<MacroCell *>(macrocell_tmp->nw));

    Quadrant *temp_n =
        result(level - 1, macrocell(level - 1, macrocell_tmp->nw->macrocell.ne,
                                    macrocell_tmp->ne->macrocell.nw,
                                    macrocell_tmp->nw->macrocell.se,
                                    macrocell_tmp->ne->macrocell.sw));

    Quadrant *temp_ne =
        result(level - 1, reinterpret_cast<MacroCell *>(macrocell_tmp->ne));

    Quadrant *temp_w =
        result(level - 1, macrocell(level - 1, macrocell_tmp->nw->macrocell.sw,
                                    macrocell_tmp->nw->macrocell.se,
                                    macrocell_tmp->sw->macrocell.nw,
                                    macrocell_tmp->sw->macrocell.ne));

    Quadrant *temp_c =
        result(level - 1, macrocell(level - 1, macrocell_tmp->nw->macrocell.se,
                                    macrocell_tmp->ne->macrocell.sw,
                                    macrocell_tmp->sw->macrocell.ne,
                                    macrocell_tmp->se->macrocell.nw));

    Quadrant *temp_e =
        result(level - 1, macrocell(level - 1, macrocell_tmp->ne->macrocell.sw,
                                    macrocell_tmp->ne->macrocell.se,
                                    macrocell_tmp->se->macrocell.nw,
                                    macrocell_tmp->se->macrocell.ne));

    Quadrant *temp_sw =
        result(level - 1, reinterpret_cast<MacroCell *>(macrocell_tmp->sw));

    Quadrant *temp_s =
        result(level - 1, macrocell(level - 1, macrocell_tmp->sw->macrocell.ne,
                                    macrocell_tmp->se->macrocell.nw,
                                    macrocell_tmp->sw->macrocell.se,
                                    macrocell_tmp->se->macrocell.sw));

    Quadrant *temp_se =
        result(level - 1, reinterpret_cast<MacroCell *>(macrocell_tmp->se));
    if (!step_size_lock) {
      Quadrant *res_nw = result(
          level - 1, macrocell(level - 1, temp_nw, temp_n, temp_w, temp_c));
      Quadrant *res_ne = result(
          level - 1, macrocell(level - 1, temp_n, temp_ne, temp_c, temp_e));
      Quadrant *res_sw = result(
          level - 1, macrocell(level - 1, temp_w, temp_c, temp_sw, temp_s));
      Quadrant *res_se = result(
          level - 1, macrocell(level - 1, temp_c, temp_e, temp_s, temp_se));


      macrocell_tmp->result_advances_in_time = true;

      macrocell_tmp->result = reinterpret_cast<Quadrant *>(
          macrocell(level - 1, res_nw, res_ne, res_sw, res_se));
    } else {
      Quadrant *res_nw;
      Quadrant *res_ne;
      Quadrant *res_sw;
      Quadrant *res_se;

      if (level == 3) {
        res_nw = reinterpret_cast<Quadrant *>(
          minicell(temp_nw->minicell.se, temp_n->minicell.sw,
                   temp_w->minicell.ne, temp_c->minicell.nw));

        res_ne = reinterpret_cast<Quadrant *>(
          minicell(temp_n->minicell.se, temp_ne->minicell.sw,
                   temp_c->minicell.ne, temp_e->minicell.nw));

        res_sw = reinterpret_cast<Quadrant *>(
          minicell(temp_w->minicell.se, temp_c->minicell.sw,
                   temp_sw->minicell.ne, temp_s->minicell.nw));

        res_se = reinterpret_cast<Quadrant *>(
          minicell(temp_c->minicell.se, temp_e->minicell.sw,
                   temp_s->minicell.ne, temp_se->minicell.nw));
      } else {
        res_nw = reinterpret_cast<Quadrant *>(
          macrocell(level - 2, temp_nw->macrocell.se, temp_n->macrocell.sw,
                               temp_w->macrocell.ne, temp_c->macrocell.nw));

        res_ne = reinterpret_cast<Quadrant *>(
          macrocell(level - 2, temp_n->macrocell.se, temp_ne->macrocell.sw,
                               temp_c->macrocell.ne, temp_e->macrocell.nw));
        res_sw = reinterpret_cast<Quadrant *>(
          macrocell(level - 2, temp_w->macrocell.se, temp_c->macrocell.sw,
                               temp_sw->macrocell.ne, temp_s->macrocell.nw));
        res_se = reinterpret_cast<Quadrant *>(
          macrocell(level - 2, temp_c->macrocell.se, temp_e->macrocell.sw,
                               temp_s->macrocell.ne, temp_se->macrocell.nw));
      }

      //DEGEULASSE
      macrocell_tmp->result = reinterpret_cast<Quadrant *>(
          macrocell(level - 1, res_nw, res_ne, res_sw, res_se));

      macrocell_tmp->result_advances_in_time = false;
      return reinterpret_cast<Quadrant *>(
        macrocell(level - 1, res_nw, res_ne, res_sw, res_se));
    }
    return macrocell_tmp->result;
  }
}

void HashlifeUniverse::grid(int *L, int width, Quadrant *r, int level, int x,
                            int y) {
  if (level == 1) {
    L[(x) + (width) * (y)] = r->minicell.nw;
    L[(x + 1) + (width) * (y)] = r->minicell.ne;
    L[(x) + (width) * (y + 1)] = r->minicell.sw;
    L[(x + 1) + (width) * (y + 1)] = r->minicell.se;
  } else {
    int dec = 1 << (level - 1);
    grid(L, width, r->macrocell.nw, level - 1, x, y);
    grid(L, width, r->macrocell.ne, level - 1, x + dec, y);
    grid(L, width, r->macrocell.sw, level - 1, x, y + dec);
    grid(L, width, r->macrocell.se, level - 1, x + dec, y + dec);
  }
}

void HashlifeUniverse::print_grid(Quadrant *r, size_t level) {
  int cote = 1 << level;
  int T[cote * cote];

  grid(T, cote, r, static_cast<int>(level), 0, 0);

  for (int i = 0; i < cote; ++i) {
    for (int j = 0; j < cote; ++j) {
      if (1) {
        if (T[i * cote + j]) {
          printf("# ");
        } else {
          printf(". ");
        }
      } else {
        if (T[i * cote + j]) {
          printf("█▉");
        } else {
          printf("╶╴");
        }
      }
    }
    printf("\n");
  }
}

void HashlifeUniverse::get_cell_in_bounds_rec(Rect bounds,
                                              std::vector<Coord> *coords,
                                              size_t current_level,
                                              Quadrant *current_cell,
                                              Coord current_coord) const {
  BigInt x = current_coord.x;
  BigInt y = current_coord.y;

  if (current_level == 1) {
    MiniCell minicell = current_cell->minicell;
    if (bounds.is_in({x, y})) {
      if (minicell.nw)
        coords->push_back({x, y});
    }

    if (bounds.is_in({x + 1, y})) {
      if (minicell.ne)
        coords->push_back({x + 1, y});
    }

    if (bounds.is_in({x, y + 1})) {
      if (minicell.sw)
        coords->push_back({x, y + 1});
    }

    if (bounds.is_in({x + 1, y + 1})) {
      if (minicell.se)
        coords->push_back({x + 1, y + 1});
    }
  } else {
    MacroCell macrocell = current_cell->macrocell;
    if (!(macrocell == zeros[current_level]->macrocell)) {
      BigInt size = BigInt(1) << mp_size_t(current_level - 1);
      if (bounds.collides({{x, y}, {x + size, y + size}})) {
        get_cell_in_bounds_rec(bounds, coords, current_level - 1, macrocell.nw,
                               {x, y});
      }

      if (bounds.collides({{x + size, y}, {x + BigInt(2) * size, y + size}})) {
        get_cell_in_bounds_rec(bounds, coords, current_level - 1, macrocell.ne,
                               {x + size, y});
      }

      if (bounds.collides({{x, y + size}, {x + size, y + BigInt(2) * size}})) {
        get_cell_in_bounds_rec(bounds, coords, current_level - 1, macrocell.sw,
                               {x, y + size});
      }

      if (bounds.collides({{x + size, y + size},
                           {x + BigInt(2) * size, y + BigInt(2) * size}})) {
        get_cell_in_bounds_rec(bounds, coords, current_level - 1, macrocell.se,
                               {x + size, y + size});
      }
    }
  }
}

void HashlifeUniverse::get_cell_in_bounds(Rect bounds,
                                          std::vector<Coord> *coords) const {
  get_cell_in_bounds_rec(bounds, coords, top_level,
                         reinterpret_cast<Quadrant *>(root), top_left);
}

void HashlifeUniverse::set_step_size(size_t new_step_size) {
  if (new_step_size <= top_level - 2) {
    step_size = new_step_size;
    step_size_maximized = false;
  } else {
    std::cout << "Step size is too big for this universe !" << std::endl;
  }
}

void HashlifeUniverse::set_step_size_maximized(bool is_maximized) {
  step_size_maximized = is_maximized;
  if (step_size_maximized)
    set_step_size(top_level - 2);
}
