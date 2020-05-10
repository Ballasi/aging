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
#include <QMessageBox>
#include <QSettings>

MainWindow::MainWindow() :
    settings("aging.conf", QSettings::NativeFormat) {
  bool first_use = settings.value("first_use", true).toBool();
  if (first_use) {
    printf("1ere fois\n");
    settings.setValue("first_use", false);
  } else {
    printf("plus d'une fois\n");
  }

  resize(720, 720);
  setFocusPolicy(Qt::FocusPolicy::ClickFocus);

  // isDarkTheme = window.palette().window().color().lightnessF() < 0.5;
  isDarkTheme = false;
  isDarkTheme = settings.value("isDarkTheme", isDarkTheme).toBool();

  universe = new HashlifeUniverse(8);
  univ_type = UniverseType::Hashlife;

  createUI();
}

MainWindow::~MainWindow() {}



/////////////////////////////////////////////////////
////           Creation de l'interface           ////
/////////////////////////////////////////////////////


void MainWindow::createUI() {
  ctxt.universe_scene = new UniverseScene(this,
    universe,
    univ_type);
  setCentralWidget(ctxt.universe_scene);

  set_theme();
  createToolBar();
  createMenuBar();
}

void MainWindow::createToolBar() {
  QToolBar *controlToolbar = addToolBar("Simulation Controls");
  ctxt.toolbar = controlToolbar;
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

  QActionGroup *checkableGroup = new QActionGroup(controlToolbar);
  checkableGroup->setExclusive(true);

  // MouseMode
  QAction *pencilAction = controlToolbar->addAction(
    QIcon(icon_dir + "pencil.svg"), "Edit");
  connect(pencilAction,
          &QAction::triggered, this, &MainWindow::funcAction_modeEdit);
  pencilAction->setCheckable(true);
  pencilAction->setActionGroup(checkableGroup);

  QAction *selectAction = controlToolbar->addAction(
    QIcon(icon_dir + "table.svg"), "Select");
  connect(selectAction,
          &QAction::triggered, this, &MainWindow::funcAction_modeSelect);
  selectAction->setCheckable(true);
  selectAction->setActionGroup(checkableGroup);

  QAction *moveAction = controlToolbar->addAction(
    QIcon(icon_dir + "cursor-move.svg"), "Move");
  connect(moveAction,
          &QAction::triggered, this, &MainWindow::funcAction_modeMove);
  moveAction->setCheckable(true);
  moveAction->setActionGroup(checkableGroup);

  // bouton d'un zoom centré
  connect(controlToolbar->addAction(QIcon(icon_dir + "zoom-in.svg"),
    "Zoom In"),
    &QAction::triggered, this, &MainWindow::funcAction_zoomIn);

  // bouton d'un dezoom centré
  connect(controlToolbar->addAction(QIcon(icon_dir + "zoom-out.svg"),
    "Zoomt Out"),
    &QAction::triggered, this, &MainWindow::funcAction_zoomOut);

  controlToolbar->addSeparator();

  // bouton exit
  connect(controlToolbar->addAction(QIcon(icon_dir + "logout.svg"),
    "Quit"),
    &QAction::triggered, this, &MainWindow::close);
}


void MainWindow::createMenuBar() {
  QMenu *fileMenu = new QMenu("File");
    // new file
    connect(fileMenu->addAction("New File"),
      &QAction::triggered, this, &MainWindow::funcAction_newFile);

    // open file
    connect(fileMenu->addAction("Open File"),
      &QAction::triggered, this, &MainWindow::funcAction_openFile);

    // save file
    connect(fileMenu->addAction("Save File"),
      &QAction::triggered, this, &MainWindow::funcAction_saveFile);

    fileMenu->addSeparator();

    // exit
    connect(fileMenu->addAction("Quit"),
      &QAction::triggered, this, &MainWindow::close);
    menuBar()->addMenu(fileMenu);

  // Option
  QMenu *optMenu = new QMenu("Options");

    QMenu *algoMenu = new QMenu("Set Algorithm");

      QActionGroup* groupAlgo = new QActionGroup(algoMenu);

      QAction* algo1 = algoMenu->addAction("Hashlife");
      QAction* algo2 = algoMenu->addAction("Basic");
      algo1->setCheckable(true);
      algo2->setCheckable(true);
      algo1->setActionGroup(groupAlgo);
      algo2->setActionGroup(groupAlgo);
      switch (univ_type) {
        case UniverseType::Hashlife:
          algo1->setChecked(true);
          break;
        case UniverseType::Life:
          algo2->setChecked(true);
          break;
      }
      connect(algo1,
        &QAction::triggered, this, &MainWindow::funcAction_newUnivTypeHashlife);
      connect(algo2,
        &QAction::triggered, this, &MainWindow::funcAction_newUnivTypeNaive);
      optMenu->addMenu(algoMenu);

    menuBar()->addMenu(optMenu);

  // Preferences
  QMenu *prefMenu = new QMenu("Preferences");
    // set color
    QMenu *prefColor = new QMenu("Set Color");
      connect(prefColor->addAction("Dead Cell"),
        &QAction::triggered, this, &MainWindow::funcAction_setColorBg);

      connect(prefColor->addAction("Alive Cell"),
        &QAction::triggered, this, &MainWindow::funcAction_setColorFg);

      connect(prefColor->addAction("Grid"),
        &QAction::triggered, this, &MainWindow::funcAction_setColorGrid);
      prefMenu->addMenu(prefColor);

    // Toggle Bord
    /*
    QAction* toggle_bord = prefMenu->addAction("Infinite Grid"); 
      toggle_bord->setCheckable(true);
      connect(toggle_bord,
        &QAction::toggled, this, &MainWindow::funcAction_setInfiniteGrid);
    */
    connect(prefMenu->addAction("Set Rank Grid"),
      &QAction::triggered, this, &MainWindow::funcAction_setRankGrid);

    prefMenu->addSeparator();

    QAction* dark_theme = prefMenu->addAction("Dark Theme");
      dark_theme->setCheckable(true);
      dark_theme->setChecked(isDarkTheme);
      connect(dark_theme,
        &QAction::toggled, this, &MainWindow::funcAction_darkTheme);

    menuBar()->addMenu(prefMenu);

  // Help
  QMenu *helpMenu = new QMenu("Help");

    connect(helpMenu->addAction("Help"),
      &QAction::triggered, this, &MainWindow::funcAction_help);

    connect(helpMenu->addAction("Documentation"),
      &QAction::triggered, this, &MainWindow::funcAction_documentation);

    helpMenu->addSeparator();

    connect(helpMenu->addAction("Licence"),
      &QAction::triggered, this, &MainWindow::funcAction_licence);

    connect(helpMenu->addAction("About"),
      &QAction::triggered, this, &MainWindow::funcAction_about);

    menuBar()->addMenu(helpMenu);
}

void MainWindow::set_theme() {
  /*
  La couleur de theme ne se propage pas, et de coup, l'interface est un peu
  moche...
  Je ne sais pas si c'est parce que je n'ai pas trouvé, ou que c'est un bug.

  https://bugreports.qt.io/browse/QTBUG-81958
  https://doc.qt.io/qt-5/stylesheet-syntax.html#inheritance
  */
  if (isDarkTheme) {
    setStyleSheet(
        "* {color: #E0E0E0;" // color-text
        "selection-color: #101010;" // color-text selected
        "background: #101010;" // bg
        "selection-background-color: lightgreen ;}"); // bg selected
  } else {
    setStyleSheet(
        "* {color: #101010;" // color-text
        "selection-color: #E0E0E0;" // color-text selected
        "background: #E0E0E0;" // bg
        "selection-background-color: darkgreen ;}"); // bg selected
  }
}

void MainWindow::resetUI() {
  menuBar()->clear();
  removeToolBar(ctxt.toolbar);
  set_theme();
  createToolBar();
  createMenuBar();
}
/////////////////////////////////////////////////////
////                   Actions                   ////
/////////////////////////////////////////////////////

void MainWindow::funcAction_newFile() {
  switch (univ_type) {
    case UniverseType::Hashlife:
      funcAction_newUnivTypeHashlife();
      break;
    case UniverseType::Life:
      funcAction_newUnivTypeNaive();
      break;
  }
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
        break;
      case UniverseType::Life :
        universe = new NaiveUniverse(fileName);
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


void MainWindow::funcAction_modeEdit() {
  ctxt.universe_scene->set_mode(EDIT);
}
void MainWindow::funcAction_modeSelect() {
  ctxt.universe_scene->set_mode(SELECT);
}
void MainWindow::funcAction_modeMove() {
  ctxt.universe_scene->set_mode(MOVE);
}

void MainWindow::funcAction_zoomIn() {
  ctxt.universe_scene->zoom_in();
}
void MainWindow::funcAction_zoomOut() {
  ctxt.universe_scene->zoom_out();
}


void MainWindow::funcAction_setColorBg() {
  QColor color =
      QColorDialog::getColor(ctxt.universe_scene->get_cell_color(0),
       this, "Choose Background Color");
  ctxt.universe_scene->set_cell_color(0, color);
}
void MainWindow::funcAction_setColorFg() {
  QColor color =
      QColorDialog::getColor(ctxt.universe_scene->get_cell_color(1),
       this, "Choose Cell Color");
  ctxt.universe_scene->set_cell_color(1, color);
}
void MainWindow::funcAction_setColorGrid() {
  QColor color =
      QColorDialog::getColor(ctxt.universe_scene->get_grid_color(),
        this, "Choose Grid Color");
  ctxt.universe_scene->set_grid_color(color);
}

void MainWindow::funcAction_setInfiniteGrid() {
  ctxt.universe_scene->toggle_bord();
}
void MainWindow::funcAction_setRankGrid() {
  bool ok;
  std::string s;
  int rank = QInputDialog::getInt(this, "Set Rang Grid", s.c_str(),
                  ctxt.universe_scene->get_rank_grid(), 0, 50, 1, &ok);
  if (ok) {
    ctxt.universe_scene->set_rank_grid(rank);
  }
}
void MainWindow::funcAction_darkTheme() {
  isDarkTheme = !isDarkTheme;
  settings.setValue("isDarkTheme", isDarkTheme);
  resetUI();
}

void MainWindow::funcAction_help() {
}
void MainWindow::funcAction_documentation() {
}
void MainWindow::funcAction_licence() {
  QMessageBox msgBox;
  msgBox.setWindowTitle("Licence");
  // msgBox.setTextFormat(Qt::TextFormat::AlignHCenter);
  std::string s;
  s += "GNU GENERAL PUBLIC LICENSE";
  s += "Version 3, 29 June 2007";
  s += "Copyright (C) 2007 Free Software Foundation, Inc. <http://fsf.org/>";
  s += "Everyone is permitted to copy and distribute verbatim copies";
  s += "of this license document, but changing it is not allowed.";
  msgBox.setText(s.c_str());
  msgBox.exec();
}
void MainWindow::funcAction_about() {
  QMessageBox msgBox;
  msgBox.setWindowTitle("About");
  msgBox.setText("Lorem ipsum ?");
  msgBox.exec();
}


void MainWindow::funcAction_newUnivTypeHashlife() {
  delete universe;
  delete ctxt.universe_scene;

  universe = new HashlifeUniverse(8);
  univ_type = UniverseType::Hashlife;

  ctxt.universe_scene = new UniverseScene(this,
    universe,
    univ_type);
  setCentralWidget(ctxt.universe_scene);
}
void MainWindow::funcAction_newUnivTypeNaive() {
  delete universe;
  delete ctxt.universe_scene;

  universe = new NaiveUniverse(Coord(BigInt(1024), BigInt(1024)));
  univ_type = UniverseType::Life;

  ctxt.universe_scene = new UniverseScene(this,
    universe,
    univ_type);
  setCentralWidget(ctxt.universe_scene);
}


/////////////////////////////////////////////////////
////              GESTION DES EVENT              ////
/////////////////////////////////////////////////////

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
  if (event->delta() > 0)
    ctxt.universe_scene->zoom_in(event->pos());
  else
    ctxt.universe_scene->zoom_out(event->pos());
}

void MainWindow::mousePressEvent(QMouseEvent *event) {
  QPoint rpos = ctxt.universe_scene->mapFromParent(event->pos());
  ctxt.buffer_coord = ctxt.universe_scene->map_coords(rpos);
  ctxt.pressed_button = event->button();
  switch (ctxt.universe_scene->get_mode()) {
    case SceneMode::MOVE:
      if (ctxt.pressed_button = Qt::LeftButton) {
        ctxt.drag_position = rpos;
      }
      break;
    case SceneMode::EDIT:
      ctxt.universe_scene->set_cell(ctxt.buffer_coord,
                ctxt.pressed_button == Qt::LeftButton);
      break;
    case SceneMode::SELECT:
      break;
    default:
      break;
  }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event) {
  QPoint rpos = ctxt.universe_scene->mapFromParent(event->pos());
  Coord coord = ctxt.universe_scene->map_coords(rpos);
  switch (ctxt.universe_scene->get_mode()) {
    case SceneMode::MOVE:
      if (ctxt.pressed_button == Qt::LeftButton) {
        QPointF diff = rpos - ctxt.drag_position;
        if (diff.manhattanLength() > 42) {
          ctxt.drag_position = rpos;
          ctxt.universe_scene->
            move_camera({-diff.x() / ctxt.universe_scene->width(),
                         -diff.y() / ctxt.universe_scene->height()});
        }
      }
      break;
    case SceneMode::EDIT:
      if (!(ctxt.buffer_coord == coord)) {
        ctxt.buffer_coord = coord;
        ctxt.universe_scene->set_cell(ctxt.buffer_coord,
                  ctxt.pressed_button == Qt::LeftButton);
      }
      break;
    case SceneMode::SELECT:
      break;
    default:
      break;
  }
}
