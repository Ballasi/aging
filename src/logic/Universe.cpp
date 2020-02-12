#include "Universe.hpp"
#include "cell/MiniCell.hpp"
#include <unordered_set>
#include <vector>


Universe::Universe(size_t top_level, Coord top_left)
  : top_left(top_left), top_level(top_level) {
  macrocell_sets.resize(top_level + 1);
  zeros.push_back(nullptr);
  zeros.push_back((Quadrant*) minicell());
  for (size_t i = 2; i <= top_level; ++i) {
    zeros.push_back((Quadrant*) macrocell(i));
  }

  root = (MacroCell*) zeros.back();
}

Universe::Universe(QString filename, Coord top_left)
: top_left(top_left) {

  QFile file(filename);
  if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) return;

  Coord boundingbox = read_rle_size(file);
  cout << "Creating a new hashlife universe of level " << top_level << '\n';
  // Starting with one extra level
  macrocell_sets.resize(top_level);
  zeros.push_back((Quadrant*) minicell());
  for (size_t i = 1; i < top_level; ++i) {
    zeros.push_back((Quadrant*) macrocell(i));
  }
  root = (MacroCell*) zeros.back();

  read_rle_data(file, boundingbox);
}

void Universe::debug() {
  ((Quadrant*)root)->debug(top_level);
}

Coord Universe::read_rle_size(QFile &file) {
  while(!file.atEnd())
  {
    QByteArray line = file.readLine();
    if(line[0] == '#'){
      continue;
    }
    if(line[0] == 'x'){
      QList<QByteArray> list = line.split(',');
      int width = ((list[0].split('='))[1].simplified()).toInt();
      int height = ((list[1].split('='))[1].simplified()).toInt();
      top_level = (width > height) ? (size_t) log2(width) + 1 : (size_t) log2(height) + 1;
      return Coord(width,height);
    }
  }
  return 6;
}

void Universe::read_rle_data(QFile &file, Coord boundingbox) {

  QByteArray data("");
  while(!file.atEnd()) {
    QByteArray line = file.readLine();
    cout << line.toStdString() << '\n';
    data.append(line);
  }

  data = data.simplified();

  int init_x = (1 << (top_level - 1)) - (int) boundingbox.x/2;
  int init_y = (1 << (top_level - 1)) - (int) boundingbox.y/2;
  int curr_x = init_x;
  int curr_y = init_y;

  QByteArray qs("");
  for(int i = 0; i < data.length(); ++i)
  {
    if(data[i] == '\0')
      continue;
    int q;
    if(data[i] == '$'){
      q = qs.isEmpty() ? 1 : qs.toInt();
      curr_y += q;
      curr_x = init_x;
      qs.clear();
    }
    if(data[i] >= '0' && data[i] <= '9'){
      qs.append(data[i]);
    }
    if(data[i] == 'o' || data[i] == 'b'){
      q = qs.isEmpty() ? 1 : qs.toInt();
      for (int n = 0; n < q; n++) {
        set(Coord(curr_x,curr_y),data[i] == 'o');
        curr_x++;
      }
      qs.clear();
    }
    if(data[i] == '!')
      break;
  }
}

void Universe::assert_handles(size_t asserted_level) {
  if (asserted_level >= macrocell_sets.size()) {
    size_t previous_asserted_level = macrocell_sets.size() - 1;

    macrocell_sets.resize(asserted_level + 1);
    for (size_t level = previous_asserted_level + 1; level <= asserted_level; ++level)
      zeros.push_back((Quadrant *) macrocell(level));
  }
}

size_t Universe::step() {
  // Assert that the creation of higher level is possible
  assert_handles(top_level + 1);
  // Add of crown of empty cell
  root = crown();
  // Calculate result of universe
  Quadrant *new_root = result(top_level + 1, root);
  // update attributes of universe
  root = (MacroCell *)new_root;
  return 1 << top_level;
}

const CellState Universe::get(Coord target) const { return *find(target); }

void Universe::set(Coord target, CellState state) {
  root = (MacroCell*) set_rec(top_left, top_level, (Quadrant*) root, target, state);
}

Quadrant *Universe::set_rec(Coord current, size_t level, Quadrant *cell, Coord target, CellState state) {
  Coord size(level - 1);
  Coord center = current + size;

  if (level <= 1) {
    if (target.y < center.y) {
      if (target.x < center.x)
        return (Quadrant*) minicell(      state      , cell->minicell.ne,
                                    cell->minicell.sw, cell->minicell.se);
      else
        return (Quadrant*) minicell(cell->minicell.nw,       state      ,
                                    cell->minicell.sw, cell->minicell.se);
    } else {
      if (target.x < center.x)
        return (Quadrant*) minicell(cell->minicell.nw, cell->minicell.ne,
                                          state      , cell->minicell.se);
      else
        return (Quadrant*) minicell(cell->minicell.nw, cell->minicell.ne,
                                    cell->minicell.sw,       state      );
    }
  }
  else {
    level -= 1;
    if (target.y < center.y) {
      if (target.x < center.x)
        return (Quadrant *) macrocell(level,
                                      set_rec(current, level, cell->macrocell.nw , target, state),
                                      cell->macrocell.ne, cell->macrocell.sw, cell->macrocell.se);
      else {
        current.x = center.x;
        return (Quadrant *) macrocell(level, cell->macrocell.nw,
                                      set_rec(current, level, cell->macrocell.ne, target, state),
                                      cell->macrocell.sw, cell->macrocell.se);
      }
    } else {
      current.y = center.y;
      if (target.x < center.x)
        return (Quadrant *) macrocell(level, cell->macrocell.nw, cell->macrocell.ne,
                                      set_rec(current, level, cell->macrocell.sw , target, state),
                                      cell->macrocell.se);
      else {
        current.x = center.x;
        return (Quadrant *) macrocell(level, cell->macrocell.nw, cell->macrocell.ne, cell->macrocell.sw,
                                      set_rec(current, level, cell->macrocell.se, target, state));
      }
    }
  }
}

CellState *Universe::find(Coord target) const {
  Quadrant *cell = (Quadrant*) root;
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

CellState *Universe::find_path(Coord coord, vector<Quadrant*> &path) const {
  Quadrant *cell = (Quadrant*) root;
  Coord size(top_level - 1);
  Coord current(top_left);
  Coord center = current + size;
  for (size_t level = top_level; level > 1; --level) {
    path.push_back(cell);
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

MiniCell *Universe::minicell() {
  return (MiniCell *)&*minicells.emplace().first;
}

MiniCell *Universe::minicell(CellState nw, CellState ne,
                             CellState sw, CellState se) {
  return (MiniCell*) &*minicells.emplace(nw, ne, sw, se).first;
}

MacroCell *Universe::macrocell(size_t level) {
  return (MacroCell*) &*macrocell_sets[level]
      .emplace(zeros[level - 1], zeros[level - 1],
               zeros[level - 1], zeros[level - 1])
      .first;
}

MacroCell *Universe::macrocell(size_t level,
                               Quadrant *nw, Quadrant *ne,
                               Quadrant *sw, Quadrant *se) {
  return (MacroCell*) &*macrocell_sets[level].emplace(nw, ne, sw, se).first;
}

Quadrant *Universe::quadrant(size_t level) {
  if (level <= 1) {
    return (Quadrant*) minicell();
  } else {
    return (Quadrant*) macrocell(level,
                                 quadrant(level - 1), quadrant(level - 1),
                                 quadrant(level - 1), quadrant(level - 1));
  }
}

size_t Universe::get_top_level() {
  return top_level;
}

Coord Universe::get_top_left(){
  return top_left;
}

MacroCell *Universe::crown() {

  Quadrant* zero = zeros[top_level - 1];

  MacroCell* nw = macrocell(top_level, zero, zero, zero, root->nw);
  MacroCell* ne = macrocell(top_level, zero, zero, root->ne, zero);
  MacroCell *sw = macrocell(top_level, zero, root->sw, zero, zero);
  MacroCell *se = macrocell(top_level, root->se, zero, zero, zero);

  return macrocell(top_level + 1, (Quadrant *) nw, (Quadrant *) ne,
                                  (Quadrant *) sw, (Quadrant *) se);
}

Quadrant *Universe::result(size_t level, MacroCell* macrocell_tmp) {

  if (macrocell_tmp->result != nullptr) return macrocell_tmp->result;

  if (level <= 2) {
    cout << "0" << endl;
    int nb_nw =  macrocell_tmp->nw->minicell.nw + macrocell_tmp->nw->minicell.ne + macrocell_tmp->ne->minicell.nw  +
                 macrocell_tmp->nw->minicell.sw +                                  macrocell_tmp->ne->minicell.sw  +
                 macrocell_tmp->sw->minicell.nw + macrocell_tmp->sw->minicell.ne + macrocell_tmp->se->minicell.nw;

    cout << "1" << endl;
    int nb_ne =  macrocell_tmp->nw->minicell.ne + macrocell_tmp->ne->minicell.nw + macrocell_tmp->ne->minicell.ne  +
                 macrocell_tmp->nw->minicell.se +                                  macrocell_tmp->ne->minicell.se  +
                 macrocell_tmp->sw->minicell.ne + macrocell_tmp->se->minicell.nw + macrocell_tmp->se->minicell.ne;

    cout << "2" << endl;
    int nb_sw =  macrocell_tmp->nw->minicell.sw + macrocell_tmp->nw->minicell.se + macrocell_tmp->ne->minicell.sw  +
                 macrocell_tmp->sw->minicell.nw +                                  macrocell_tmp->se->minicell.nw  +
                 macrocell_tmp->sw->minicell.sw + macrocell_tmp->sw->minicell.se + macrocell_tmp->se->minicell.sw;

    cout << "3" << endl;
    int nb_se =  macrocell_tmp->nw->minicell.se + macrocell_tmp->ne->minicell.sw + macrocell_tmp->ne->minicell.se  +
                 macrocell_tmp->sw->minicell.ne +                                  macrocell_tmp->se->minicell.ne  +
                 macrocell_tmp->sw->minicell.se + macrocell_tmp->se->minicell.sw + macrocell_tmp->se->minicell.se;

    cout << "4" << endl;
    CellState new_nw ;
    CellState new_ne ;
    CellState new_sw ;
    CellState new_se ;
    // I check if the cell is alive
    cout << "5" << endl;
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
        if ( (nb_ne == 2) || (nb_ne == 3) ) { new_ne = 1; }
        else { new_ne = 0; }
    } else {
        // if the cell is dead, i check if cell born
        if ( nb_ne == 3 ) { new_ne = 1; }
        else { new_ne = 0; }
    }

    // I check if the cell is alive
    if (macrocell_tmp->sw->minicell.ne) {
        // If the cell is alive, i check if cell survive
        if ( (nb_sw == 2) || (nb_sw == 3) ) { new_sw = 1; }
        else { new_sw = 0; }
    } else {
        // if the cell is dead, i check if cell born
        if ( nb_sw == 3 ) { new_sw = 1; }
        else { new_sw = 0; }
    }

    // I check if the cell is alive
    if (macrocell_tmp->se->minicell.nw) {
        // If the cell is alive, i check if cell survive
        if ( (nb_se == 2) || (nb_se == 3) ) { new_se = 1; }
        else { new_se = 0; }
    } else {
        // if the cell is dead, i check if cell born
        if ( nb_se == 3 ) { new_se = 1; }
        else { new_se = 0; }
    }

    cout << "6" << endl;
    return (Quadrant *)minicell(new_nw, new_ne, new_se, new_sw);
  } else {

    // voir figure 4 de : https://www.drdobbs.com/jvm/an-algorithm-for-compressing-space-and-t/184406478

    // a partir de ma macroCell de taille n,
    // je vais faire 6 assemblages différents de macroCell de taille n-2
    // afin de pouvoir avoir les 4 carrés centraux (de taille n-2), puis reconstruire la cellules centrale (n-1).
    cout << "7" << endl;
    Quadrant *temp_nw = result(level - 1, (MacroCell *)macrocell_tmp->nw);

    cout << "8" << endl;
    Quadrant *temp_n =
        result(level - 1, macrocell(level - 1, macrocell_tmp->nw->macrocell.ne,
                                    macrocell_tmp->ne->macrocell.nw,
                                    macrocell_tmp->nw->macrocell.se,
                                    macrocell_tmp->ne->macrocell.sw));

    cout << "9" << endl;
    Quadrant *temp_ne = result(level - 1, (MacroCell *)macrocell_tmp->ne);

    cout << "10" << endl;
    Quadrant *temp_w =
        result(level - 1, macrocell(level - 1, macrocell_tmp->nw->macrocell.sw,
                                    macrocell_tmp->nw->macrocell.se,
                                    macrocell_tmp->sw->macrocell.nw,
                                    macrocell_tmp->sw->macrocell.ne));

    cout << "11" << endl;
    Quadrant *temp_c =
        result(level - 1, macrocell(level - 1, macrocell_tmp->nw->macrocell.se,
                                    macrocell_tmp->ne->macrocell.sw,
                                    macrocell_tmp->sw->macrocell.ne,
                                    macrocell_tmp->se->macrocell.nw));

    cout << "12" << endl;
    Quadrant *temp_e =
        result(level - 1, macrocell(level - 1, macrocell_tmp->ne->macrocell.sw,
                                    macrocell_tmp->ne->macrocell.se,
                                    macrocell_tmp->se->macrocell.nw,
                                    macrocell_tmp->se->macrocell.ne));

    cout << "13" << endl;
    Quadrant *temp_sw = result(level - 1, (MacroCell *)macrocell_tmp->sw);

    cout << "14" << endl;
    Quadrant *temp_s =
        result(level - 1, macrocell(level - 1, macrocell_tmp->sw->macrocell.ne,
                                    macrocell_tmp->se->macrocell.nw,
                                    macrocell_tmp->sw->macrocell.se,
                                    macrocell_tmp->se->macrocell.sw));

    cout << "15" << endl;
    Quadrant *temp_se = result(level - 1, (MacroCell *)macrocell_tmp->se);

    cout << "16" << endl;
    Quadrant *res_nw = result(
        level - 1, macrocell(level - 1, temp_nw, temp_n, temp_w, temp_c));
    cout << "17" << endl;
    Quadrant *res_ne = result(
        level - 1, macrocell(level - 1, temp_n, temp_ne, temp_c, temp_e));
    cout << "18" << endl;
    Quadrant *res_sw = result(
        level - 1, macrocell(level - 1, temp_w, temp_c, temp_sw, temp_s));
    cout << "19" << endl;
    Quadrant *res_se = result(
        level - 1, macrocell(level - 1, temp_c, temp_e, temp_s, temp_se));

    cout << "20" << endl;
    macrocell_tmp->result =
        (Quadrant *)macrocell(level - 1, res_nw, res_ne, res_sw, res_se);

    cout << "21" << endl;
    return macrocell_tmp->result;
  }
}
