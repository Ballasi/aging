#include <cstdio>
#include <ui/UniverseScene.hpp>
#include <QStatusBar>
#include <string>
#include <iostream>

UniverseScene::UniverseScene(QWidget *parent, Universe *universe,
                UniverseType type): universe(universe), univ_type(type),
                settings("agingUS.conf", QSettings::NativeFormat) {
    r_area = new RenderArea(this, universe, type);
    mode = MOVE;
    isSimulationRun = false;
    bords = true;

    refresh_time_ms = 0;
    p_step = 1;


    QVariant variant1;
    QVariant variant2;
    QVariant variant3;

    variant1 = QVariant(QColor("#8bb158").name());
    variant2 = QVariant(QColor("#181913").name());
    variant3 = QVariant(QColor("#8bb158").name());

    variant1 = settings.value("colorGrid", variant1);
    variant2 = settings.value("colorBg", variant2);
    variant3 = settings.value("colorFg", variant3);

    set_grid_color(QColor(variant1.toString()));
    set_cell_color(0, QColor(variant2.toString()));
    set_cell_color(1, QColor(variant3.toString()));

    std::cout << "bg :" << colors[0].red() << ", " << colors[0].green()
    << ", " << colors[0].blue() << "\n";
    std::cout << "fg :" << colors[1].red() << ", " << colors[1].green()
    << ", " << colors[1].blue() << "\n";

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
    universe->step();
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

void UniverseScene::toggle_bord() {
  bords = !bords;
}

bool UniverseScene::get_infinite_grid() {
  return !bords;
}

void UniverseScene::set_cell_color(CellState state, QColor color) {
    colors[state] = color;
    settings.setValue("colorBg", colors[0].name());
    settings.setValue("colorFg", colors[1].name());

    r_area->set_colors(colors[1], colors[0]);
    r_area->update();
}
QColor UniverseScene::get_cell_color(CellState state) {
    return colors[state];
}

QColor UniverseScene::get_grid_color() { return color_grid ;}
void UniverseScene::set_grid_color(QColor color) {
  color_grid = color;
  settings.setValue("colorGrid", color_grid.name());
}
int UniverseScene::get_rank_grid() { return rank_grid ;}
void UniverseScene::set_rank_grid(int rank) { rank_grid = rank ;}
void UniverseScene::up_rank_grid() { rank_grid += 1; }
void UniverseScene::down_rank_grid() { rank_grid -= 1; }

QString UniverseScene::get_generation() {
  return QString(bigint_to_str(universe->get_generation()).c_str());
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

Coord UniverseScene::map_coords(QPoint mouse) {
  return r_area->map_coords_from_mouse(mouse);
}

void UniverseScene::updateStatusBar() {
  QString s;
  s += "Generation : ";
  s += get_generation();
  s += " | Position : (";
  Coord center = map_coords(QPoint(width() / 2, height() / 2));
  s += bigint_to_str(center.x).c_str();
  s += ", ";
  s += bigint_to_str(center.y).c_str();
  s += ") | Universe size : ";
  Coord size = universe->get_size();
  s += bigint_to_str(size.x).c_str();
  s += " x ";
  s += bigint_to_str(size.y).c_str();
  QStatusBar *bar = _parent->statusBar();
  bar->showMessage(s);
}

void UniverseScene::set_cell(Coord coord, CellState state) {
  universe->set(coord, state);
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

