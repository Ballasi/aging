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
      universe->set(Coord(i, j),  ( (i + j) % 12 == 0 || (i - j) % 12 == 0) );
    }
  }

  createUI();
}

MainWindow::~MainWindow() {}

void MainWindow::createUI() {
  ctxt.universe_scene = new UniverseScene(this,
    universe,
    univ_type);
  setCentralWidget(ctxt.universe_scene);
  createToolBar();
  createMenuBar();
}

void MainWindow::createToolBar() {
  QToolBar *controlToolbar = addToolBar("Simulation Controls");
  addToolBar(Qt::LeftToolBarArea, controlToolbar);

  // chargement des icones
  QString icon_dir("../res/icons/light/");
  if (isDarkTheme)
    icon_dir = "../res/icons/dark/";


  // new file
  connect(controlToolbar->addAction(QIcon(icon_dir + "file.svg"),
    "New File"),
    &QAction::triggered, this, &MainWindow::funcAction_newFile);

  // open file
  connect(controlToolbar->addAction(QIcon(icon_dir + "folder-open.svg"),
    "Open File"),
    &QAction::triggered, this, &MainWindow::funcAction_openFile);

  // save file
  connect(controlToolbar->addAction(QIcon(icon_dir + "floppy.svg"),
    "Save File"),
    &QAction::triggered, this, &MainWindow::funcAction_saveFile);


  controlToolbar->addSeparator();

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

  // Button Increase Speed
  connect(controlToolbar->addAction(QIcon(icon_dir + "fast-forward.svg"),
    "Increase Speed"),
    &QAction::triggered, this, &MainWindow::funcAction_incSpeed);

  // Button Decrease Speed
  connect(controlToolbar->addAction(QIcon(icon_dir + "rewind.svg"),
    "Decrease Speed"),
    &QAction::triggered, this, &MainWindow::funcAction_decSpeed);


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
    "Change Mouse Mode");
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


void MainWindow::createMenuBar() {
}


void MainWindow::funcAction_newFile() {
  delete universe;
  delete ctxt.universe_scene;

  universe = new HashlifeUniverse(8);
  univ_type = UniverseType::Hashlife;

  ctxt.universe_scene = new UniverseScene(this,
    universe,
    univ_type);
  setCentralWidget(ctxt.universe_scene);
}

void MainWindow::funcAction_openFile() {
  QString fileName, acceptedFormats;
  switch (univ_type) {
    case UniverseType::Hashlife:
      acceptedFormats = "Pattern files (*.rle *.mc)";
      break;
    case UniverseType::Life:
      acceptedFormats = "Pattern files (*.rle)";
      break;
  }

  fileName = QFileDialog::getOpenFileName(this, "Open File",
                                          "", acceptedFormats);
  if (!fileName.isNull()) {
    delete universe;
    delete ctxt.universe_scene;
    switch (univ_type) {
      case UniverseType::Hashlife :
        universe = new HashlifeUniverse(fileName);
        univ_type = UniverseType::Hashlife;
        break;
      case UniverseType::Life :
        universe = new NaiveUniverse(fileName);
        univ_type = UniverseType::Life;
        break;
    }
    ctxt.universe_scene = new UniverseScene(this,
      universe,
      univ_type);
    setCentralWidget(ctxt.universe_scene);
  }
}

void MainWindow::funcAction_saveFile() {
  printf("MainWindow::funcAction_saveFile() not Implemented\n");
}

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

void MainWindow::funcAction_incSpeed() {
  ctxt.universe_scene->increase_speed();
  ctxt.universe_scene->get_speed();
}

void MainWindow::funcAction_decSpeed() {
  ctxt.universe_scene->decrease_speed();
  ctxt.universe_scene->get_speed();
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

