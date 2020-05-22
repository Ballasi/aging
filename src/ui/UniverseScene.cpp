#include <cstdio>
#include <ui/UniverseScene.h>
#include <QStatusBar>
#include <string>
#include <iostream>

UniverseScene::UniverseScene(QWidget *parent, Universe *universe,
                UniverseType type): universe(universe), univ_type(type),
                settings("agingUS.conf", QSettings::NativeFormat) {
    mode = MOVE;
    isSimulationRun = false;
    bords = true;

    selection.state = SelectionState::None;
    selection.copy_available = false;

    refresh_time_ms = 0;
    p_step = 1;

    r_area = new RenderArea(this, universe, type, &selection);


    stepTimer = new QTimer(this);
    connect(stepTimer, &QTimer::timeout, this, &UniverseScene::step);

    _parent = reinterpret_cast<QMainWindow *>(parent);
    //updateStatusBar();
}


void UniverseScene::play_pause() {
  isSimulationRun = !isSimulationRun;
  if (isSimulationRun) {
    stepTimer->start(refresh_time_ms);
  } else {
    stepTimer->stop();
  }
}

bool UniverseScene::get_state_simulation() {
  return isSimulationRun;
}

void UniverseScene::step() {
    universe->update();
    r_area->update();
    updateStatusBar();
}

void UniverseScene::increase_speed() {
  if (refresh_time_ms <= 0) {
    p_step += 1;
  } else {
    refresh_time_ms -= 250;
    if (refresh_time_ms <= 0) {
      p_step = 1;
      refresh_time_ms = 0;
    }
  }
  updateStatusBar();
}

void UniverseScene::decrease_speed() {
  if (p_step == 0) {
    refresh_time_ms += 250;
  } else {
    p_step -= 1;
    if (p_step <= 0) {
      refresh_time_ms = 250;
      p_step = 0;
    }
  }
  updateStatusBar();
}

void UniverseScene::zoom_in(QPoint origin) {
    r_area->zoomin_event(origin);
    updateStatusBar();
}

void UniverseScene::zoom_out(QPoint origin) {
    r_area->zoomout_event(origin);
    updateStatusBar();
}

void UniverseScene::zoom_in() {
  r_area->zoomin();
  updateStatusBar();
}

void UniverseScene::zoom_out() {
  r_area->zoomout();
  updateStatusBar();
}





void UniverseScene::left() {
    r_area->left();
    updateStatusBar();
}
void UniverseScene::right() {
    r_area->right();
    updateStatusBar();
}
void UniverseScene::down() {
    r_area->down();
    updateStatusBar();
}
void UniverseScene::up() {
    r_area->up();
    updateStatusBar();
}

void UniverseScene::fit_pattern() {
  r_area->fitPattern();
  updateStatusBar();
}

void UniverseScene::move_camera(QPointF vector) {
  r_area->move_camera(vector);
}


void UniverseScene::next_mode() {
  switch (mode) {
    case SceneMode::MOVE:
      mode = EDIT;
      break;
    case SceneMode::EDIT:
      mode = SELECT;
      break;
    case SceneMode::SELECT:
      mode = MOVE;
      break;
    default:
      break;
  }
}

void UniverseScene::set_mode(SceneMode mode) {
  this->mode = mode;
}

SceneMode UniverseScene::get_mode() {
  return mode;
}


void UniverseScene::set_cell_color(CellState state, QColor color) {
    colors[state] = color;
    r_area->set_colors(colors[1], colors[0]);
    r_area->update();
}
QColor UniverseScene::get_cell_color(CellState state) {
    return colors[state];
}

QColor UniverseScene::get_grid_color() { return color_grid ;}
void UniverseScene::set_grid_color(QColor color) {
  color_grid = color;
}

QString UniverseScene::get_generation() {
  return QString(bigint_to_str(universe->generation()).c_str());
}

QString UniverseScene::get_speed() {
  std::string str;
  if (p_step == 0) {
    str += std::to_string(refresh_time_ms);
    str += " ms/step";
  } else {
    str += "2^";
    str += std::to_string(p_step);
    str += " step size";
  }
  return QString(str.c_str());
}

Universe* UniverseScene::get_zone() {
  printf("UniverseScene::get_zone() not Implemented\n");
    return nullptr;
}

void UniverseScene::remove_in_zone() {
  printf("UniverseScene::remove_in_zone() not Implemented\n");
}

void UniverseScene::remove_out_zone() {
  printf("UniverseScene::remove_out_zone() not Implemented\n");
}


void UniverseScene::resizeEvent(QResizeEvent *event) {
    r_area->resize(event->size().width(), event->size().height());
}

Vec2 UniverseScene::map_coords(QPoint mouse) {
  return r_area->map_coords_from_mouse(mouse);
}

void UniverseScene::updateStatusBar() {
  QString s;
  s += "Generation : ";
  s += get_generation();
  s += " | Position : (";
  Vec2 center = map_coords(QPoint(width() / 2, height() / 2));
  s += bigint_to_str(center.x).c_str();
  s += ", ";
  s += bigint_to_str(center.y).c_str();
  s += ") | Universe size : ";
  Vec2 size = universe->bounds().size;
  s += bigint_to_str(size.x).c_str();
  s += " x ";
  s += bigint_to_str(size.y).c_str();
  if (mode == SceneMode::SELECT) {
    s += " | Selection state : ";
    switch (selection.state) {
      case SelectionState::None:
        s += "None";
        break;
      case SelectionState::First:
        s += "One coord";
        break;
      case SelectionState::Second:
        s += "Valid";
        break;
    }
  }
  QStatusBar *bar = _parent->statusBar();
  bar->showMessage(s);
}

void UniverseScene::set_cell(Vec2 coord, CellState state) {
  universe->set(coord, state);
  r_area->update();
}

void UniverseScene::update_selection(QPoint mouse) {
  Vec2 coord = map_coords(mouse);
  switch (selection.state) {
    case SelectionState::None:
      selection.first = coord;
      selection.state = SelectionState::First;
      break;
    case SelectionState::First:
      if (coord.x >= selection.first.x && coord.y >= selection.first.y) {
        selection.second = coord;
        selection.state = SelectionState::Second;
      } else if (coord.x <= selection.first.x && coord.y <= selection.first.y) {
        selection.second = selection.first;
        selection.first = coord;
        selection.state = SelectionState::Second;
      } else if (coord.x <= selection.first.x && coord.y >= selection.first.y) {
        Vec2 top_left(coord.x, selection.first.y);
        Vec2 bottom_right(selection.first.x, coord.y);
        selection.first = top_left;
        selection.second = bottom_right;
        selection.state = SelectionState::Second;
      } else if (coord.x >= selection.first.x && coord.y <= selection.first.y) {
        Vec2 top_left(selection.first.x, coord.y);
        Vec2 bottom_right(coord.x, selection.first.y);
        selection.first = top_left;
        selection.second = bottom_right;
        selection.state = SelectionState::Second;
      } else {
        std::cout << "Invalid selection" << std::endl;
        selection.state = SelectionState::None;
      }
      break;
    case SelectionState::Second:
      selection.first = coord;
      selection.state = SelectionState::First;
      break;
  }
  r_area->update();
  updateStatusBar();
}
Selection UniverseScene::get_selection() {
  return selection;
}

void UniverseScene::copy_selection() {
  selection.c_first = selection.first;
  selection.c_second = selection.second;

  selection.copy_available = true;
}
void UniverseScene::paste_selection() {
  std::cout << "Paste !" << std::endl;
  Vec2 coord = selection.first;
  std::cout << coord.x << ", " << coord.y << std::endl;
  for (BigInt i = 0; i <= selection.c_second.x - selection.c_first.x; ++i) {
    for (BigInt j = 0; j <= selection.c_second.y - selection.c_first.y; ++j) {
      universe->set(Vec2(coord.x + i, coord.y + j),
      universe->get(Vec2(selection.c_first.x + i, selection.c_first.y + j)));
    }
  }
  updateStatusBar();
  r_area->update();
}
void UniverseScene::reset_selection() {
  selection.copy_available = false;
  selection.state = SelectionState::None;
  updateStatusBar();
  r_area->update();
}

/*
Vec2 UniverseScene::map_Vec2_from_mouse(QPoint mouse) {
  float aspect_ratio =
      static_cast<float>(width()) / static_cast<float>(height());
  float x_relative = static_cast<float>(mouseVec2s.x()) /
                     static_cast<float>(width()) * aspect_ratio;
  float y_relative =
      static_cast<float>(mouseVec2s.y()) / static_cast<float>(height());


  QMatrix4x4 viewinv = camera->get_view().inverted();

  Vec2 c;
  QPointF p = viewinv.map(QPointF(x_relative, y_relative));
  c.x = BigInt(std::floor(p.x()));
  c.y = BigInt(std::ceil(-p.y()));

  return c;
}*/

