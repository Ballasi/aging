#include <logic/HashlifeUniverse/HashlifeUniverse.h>
#include <logic/Universe.h>
#include <ui/MainWindow.h>
#include <logic/NaiveUniverse/NaiveUniverse.h>

#include <QButtonGroup>
#include <QColorDialog>
#include <QFileDialog>
#include <QInputDialog>
#include <QMenuBar>
#include <QStatusBar>
#include <QToolBar>
#include <QToolButton>
#include <iostream>
#include <string>


MainWindow::MainWindow() {
  resize(720, 720);
  setFocusPolicy(Qt::FocusPolicy::ClickFocus);
  isDarkTheme = false;

  universe = new HashlifeUniverse(8);
  univ_type = UniverseType::Hashlife;
  for (int i = 64; i < 192; ++i) {
    for (int j = 64; j < 192; ++j) {
      universe->set(Coord(i, j), (i + j) % 12 == 0);
    }
  }

  createUI();
}

void MainWindow::createUI() {
  ctxt.universe_scene = new UniverseScene(this,
    universe,
    UniverseType::Hashlife);
  setCentralWidget(ctxt.universe_scene);

  QToolBar *controlToolbar = addToolBar("Simulation Controls");
  addToolBar(Qt::LeftToolBarArea, controlToolbar);

  // chargement des icones
  QString icon_dir("../res/icons/light/");
  if (isDarkTheme)
    icon_dir = "../res/icons/dark/";

  // Button Play/pause
  ctxt.playIcon = new QIcon(icon_dir + "play.svg");
  ctxt.pauseIcon = new QIcon(icon_dir + "pause.svg");
  ctxt.playPauseAction = controlToolbar->addAction(*(ctxt.playIcon),
    "Play/Pause");
  connect(ctxt.playPauseAction, &QAction::triggered, this,
    &MainWindow::funcAction_playPause);

  // Button One Step
  connect(controlToolbar->addAction(QIcon(icon_dir + "step.svg"),
    "Advance one step"),
    &QAction::triggered, this, &MainWindow::funcAction_step);

  controlToolbar->addSeparator();

  // Bouton Fit Pattern
  connect(controlToolbar->addAction(QIcon(icon_dir + "bullseye.svg"),
    "Fit Pattern"),
    &QAction::triggered, this, &MainWindow::funcAction_fitPattern);

  controlToolbar->addSeparator();

  // Bouton Change Mode
  ctxt.moveIcon = new QIcon(icon_dir + "cursor-move.svg");
  ctxt.editIcon = new QIcon(icon_dir + "pencil.svg");
  ctxt.selectIcon = new QIcon(icon_dir + "table.svg");
  ctxt.mouseModeAction = controlToolbar->addAction(*ctxt.moveIcon ,
    "Change Mode");
  connect(ctxt.mouseModeAction, &QAction::triggered, this,
    &MainWindow::funcAction_mode);
  controlToolbar->addSeparator();


  // bouton d'un zoom centré
  connect(controlToolbar->addAction(QIcon(icon_dir + "zoom-in.svg"),
    "Zoom In"),
    &QAction::triggered, this, &MainWindow::funcAction_zoomIn);

  // bouton d'un dezoom centré
  connect(controlToolbar->addAction(QIcon(icon_dir + "zoom-out.svg"),
    "Zoomt Out"),
    &QAction::triggered, this, &MainWindow::funcAction_zoomOut);
}



MainWindow::~MainWindow() {}


void MainWindow::funcAction_playPause() {
  ctxt.universe_scene->play_pause();
  if (ctxt.universe_scene->get_state_simulation()) {
    ctxt.playPauseAction->setIcon(*(ctxt.pauseIcon));
  } else {
    ctxt.playPauseAction->setIcon(*(ctxt.playIcon));
  }
}

void MainWindow::funcAction_step() {
  ctxt.universe_scene->step();
}

void MainWindow::funcAction_fitPattern() {
  ctxt.universe_scene->fit_pattern();
}

void MainWindow::funcAction_mode() {
  ctxt.universe_scene->next_mode();
  switch (ctxt.universe_scene->get_mode()) {
    case SceneMode::MOVE:
      ctxt.mouseModeAction->setIcon(*(ctxt.moveIcon));
      break;
    case SceneMode::EDIT:
      ctxt.mouseModeAction->setIcon(*(ctxt.editIcon));
      break;
    case SceneMode::SELECT:
      ctxt.mouseModeAction->setIcon(*(ctxt.selectIcon));
      break;
    default:
      break;
  }
}

void MainWindow::funcAction_zoomIn() {
  ctxt.universe_scene->zoom_in();
}

void MainWindow::funcAction_zoomOut() {
  ctxt.universe_scene->zoom_out();
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
  switch (event->key()) {
    case Qt::Key_Z:
    case Qt::Key_Up:
      ctxt.universe_scene->up();
      break;
    case Qt::Key_S:
    case Qt::Key_Down:
      ctxt.universe_scene->down();
      break;
    case Qt::Key_Q:
    case Qt::Key_Left:
      ctxt.universe_scene->left();
      break;
    case Qt::Key_D:
    case Qt::Key_Right:
      ctxt.universe_scene->right();
      break;
    case Qt::Key_Space:
      ctxt.universe_scene->step();
      break;
    case Qt::Key_Plus:
      ctxt.universe_scene->zoom_in();
      break;
    case Qt::Key_Minus:
      ctxt.universe_scene->zoom_out();
      break;
    default:
      break;
  }
  update();
}

void MainWindow::wheelEvent(QWheelEvent *event) {
  if (event->delta() < 0)
    ctxt.universe_scene->zoom_in(event->pos());
  else
    ctxt.universe_scene->zoom_out(event->pos());
}

