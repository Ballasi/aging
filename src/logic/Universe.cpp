#include "Universe.hpp"
#include "cell/MiniCell.hpp"
#include <vector>


Universe::Universe(size_t top_level, Coord top_left)
  : top_left(top_left), top_level(top_level) {
  // Starting with one extra level
  macrocell_sets.resize(top_level);
  zeros.push_back((Quadrant*) minicell());
  for (size_t i = 1; i < top_level; ++i) {
    zeros.push_back((Quadrant*) macrocell(i));
  }
  root = (MacroCell*) zeros.back();
}

Universe::Universe(QString filename,Coord top_left)
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
  root = zeros.back();

  read_rle_data(file,boundingbox);
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
void Universe::read_rle_data(QFile &file, Coord boundingbox){
  
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

size_t Universe::step() {
  // Add of crown of empty cell
  root = crown(top_level);
  // Calculate result of universe
  Quadrant *new_root = result(top_level, root);
  // update attributes of universe
  root = (MacroCell *)new_root;
  return 0;
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

MacroCell *Universe::crown(size_t level) {

  Quadrant* zero = zeros[level - 1];

  MacroCell* nw = macrocell(level, zero, zero, zero, root->nw);
  MacroCell* ne = macrocell(level, zero, zero, root->ne, zero);
  MacroCell *sw = macrocell(level, zero, root->sw, zero, zero);
  MacroCell *se = macrocell(level, root->se, zero, zero, zero);

  return macrocell(level+1, (Quadrant *) nw, (Quadrant *) ne,
                            (Quadrant *) sw, (Quadrant *) se);
}

Quadrant *Universe::result(size_t level, MacroCell* macrocell_tmp) {

  if (macrocell_tmp->result != nullptr) return macrocell_tmp->result;

  if (level <= 2) {
    // Calcule la mini-Cell de 2x2
    // a partir d'une macroCell de 4x4
    // en naif
    // TODO

    return (Quadrant*) minicell();
  } else {

    // voir figure 4 de : https://www.drdobbs.com/jvm/an-algorithm-for-compressing-space-and-t/184406478

    // a partir de ma macroCell de taille n,
    // je vais faire 6 assemblages différents de macroCell de taille n-2
    // afin de pouvoir avoir les 4 carrés centraux (de taille n-2), puis reconstruire la cellules centrale (n-1).
    Quadrant* temp_nw = result(level-1, (MacroCell*) macrocell_tmp->nw);

    Quadrant* temp_n  = result(level-1,  macrocell(level-1,
                                                   macrocell_tmp->nw->macrocell.ne,
                                                   macrocell_tmp->ne->macrocell.nw,
                                                   macrocell_tmp->nw->macrocell.se,
                                                   macrocell_tmp->ne->macrocell.sw));
    Quadrant* temp_ne = result(level-1, (MacroCell*) macrocell_tmp->ne);

    Quadrant* temp_w  = result(level-1, macrocell(level-1,
                                                  macrocell_tmp->nw->macrocell.sw,
                                                  macrocell_tmp->nw->macrocell.se,
                                                  macrocell_tmp->sw->macrocell.nw,
                                                  macrocell_tmp->sw->macrocell.ne ));

    Quadrant* temp_c  = result(level-1, macrocell(level-1,
                                                  macrocell_tmp->nw->macrocell.se,
                                                  macrocell_tmp->ne->macrocell.sw,
                                                  macrocell_tmp->sw->macrocell.ne,
                                                  macrocell_tmp->se->macrocell.nw ));

    Quadrant* temp_e  = result(level-1, macrocell(level-1,
                                                  macrocell_tmp->ne->macrocell.sw,
                                                  macrocell_tmp->ne->macrocell.se,
                                                  macrocell_tmp->se->macrocell.nw,
                                                  macrocell_tmp->se->macrocell.ne ));

    Quadrant* temp_sw = result(level-1, (MacroCell*) macrocell_tmp->sw);

    Quadrant* temp_s  = result(level-1, macrocell(level-1,
                                                  macrocell_tmp->sw->macrocell.ne,
                                                  macrocell_tmp->se->macrocell.nw,
                                                  macrocell_tmp->sw->macrocell.se,
                                                  macrocell_tmp->se->macrocell.sw ));

    Quadrant* temp_se = result(level-1, (MacroCell*) macrocell_tmp->se);

    Quadrant* res_nw = result(level-1, macrocell(level-1, temp_nw, temp_n , temp_w , temp_c ));
    Quadrant* res_ne = result(level-1, macrocell(level-1, temp_n , temp_ne, temp_c , temp_e ));
    Quadrant* res_sw = result(level-1, macrocell(level-1, temp_w , temp_c , temp_sw, temp_s ));
    Quadrant* res_se = result(level-1, macrocell(level-1, temp_c , temp_e , temp_s , temp_se));

    macrocell_tmp->result = (Quadrant*) macrocell(level-1, res_nw, res_ne, res_sw, res_se);

    return macrocell_tmp->result;
  }
}



