#include <cstdio>
#include <ui/UniverseScene.hpp>

UniverseScene::UniverseScene(QWidget *parent, Universe *universe,
                UniverseType type): universe(universe), univ_type(type) {
    r_area = new RenderArea(this, universe, type);
    mode = MOVE;
    isSimulationRun = false;
    bords = true;

    refresh_time_ms = 0;
    p_step = 1;

    color_grid = Qt::white;
    colors[0] = Qt::black;
    colors[1] = Qt::white;

    stepTimer = new QTimer(this);
    connect(stepTimer, &QTimer::timeout, this, &UniverseScene::step);
}


void UniverseScene::play_pause() {
  isSimulationRun = !isSimulationRun;
  if (isSimulationRun) {
    stepTimer->start(refresh_time_ms);
  } else {
    stepTimer->stop();
  }

  printf("UniverseScene::play_pause() not Implemented\n");
}

bool UniverseScene::get_state_simulation() {
  return isSimulationRun;
}

void UniverseScene::step() {
    universe->step();
    r_area->update();
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
}

void UniverseScene::zoom_in(QPoint origin) {
    r_area->zoomin_event(origin);
}

void UniverseScene::zoom_out(QPoint origin) {
    r_area->zoomout_event(origin);
}

void UniverseScene::zoom_in() {
  r_area->zoomin();
}

void UniverseScene::zoom_out() {
  r_area->zoomout();
}





void UniverseScene::left() {
    r_area->left();
}
void UniverseScene::right() {
    r_area->right();
}
void UniverseScene::down() {
    r_area->down();
}
void UniverseScene::up() {
    r_area->up();
}

void UniverseScene::fit_pattern() {
  r_area->fitPattern();
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
    r_area->set_colors(colors[1], colors[0]);
    r_area->update();
}
QColor UniverseScene::get_cell_color(CellState state) {
    return colors[state];
}

QColor UniverseScene::get_grid_color() { return color_grid ;}
void UniverseScene::set_grid_color(QColor color) { color_grid = color ;}
int UniverseScene::get_rank_grid() { return rank_grid ;}
void UniverseScene::set_rank_grid(int rank) { rank_grid = rank ;}
void UniverseScene::up_rank_grid() { rank_grid += 1; }
void UniverseScene::down_rank_grid() { rank_grid -= 1; }

QString UniverseScene::get_generation() {
  return QString(bigint_to_str(universe->get_generation()).c_str());
}

QString UniverseScene::get_speed() {
  QString message;
  if (p_step == 0) {
    message.asprintf("%d ms/step\n", refresh_time_ms);
  } else {
    message.asprintf("2^%d step\n", p_step);
  }
  return message;
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

