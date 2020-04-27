#include <ui/UniverseScene.hpp>

UniverseScene::UniverseScene(QWidget *parent, Universe *universe,
                UniverseType type): universe(universe), univ_type(type) {
    r_area = new RenderArea(this, universe, UniverseType::Hashlife);
    mode = MOVE;
    isSimulationRun = false;
    refresh_time_ms = 1;
    p_step = 0;
    bords = 0;
}


void UniverseScene::play_pause() {
  isSimulationRun = !isSimulationRun;
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
    printf("UniverseScene::increase_speed() not Implemented\n");
}

void UniverseScene::decrease_speed() {
    printf("UniverseScene::decrease_speed() not Implemented\n");
}

void UniverseScene::zoom_in(QPoint origin) {
    r_area->zoomin_event(origin);
}

void UniverseScene::zoom_out(QPoint origin) {
    r_area->zoomout_event(origin);
}

void UniverseScene::zoom_in() {
   r_area->zoomin_event(QPoint(width() / 2, height() / 2));
}

void UniverseScene::zoom_out() {
   r_area->zoomout_event(QPoint(width() / 2, height() / 2));
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
    printf("UniverseScene::toggle_bord() not Implemented\n");
}

void UniverseScene::set_cell_color(CellState state, QColor color) {
    colors[state] = color;
}
QColor UniverseScene::get_cell_color(CellState state) {
    return colors[state];
}

QColor UniverseScene::get_grid_color() { return color_grid ;}
void UniverseScene::set_grid_color(QColor color) { color_grid = color ;}
int UniverseScene::get_rank_grid() { return rank_grid ;}
void UniverseScene::set_rank_grid(int size_cell) { rank_grid ;}

QString UniverseScene::get_generation() {
    printf("UniverseScene::get_generation() not Implemented\n");
    return (QString) "";
}

QString UniverseScene::get_speed() {
    printf("UniverseScene::get_speed() not Implemented\n");
    return (QString) "";
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

