#include <model/Universe.h>
#include <ui/MainWindow.h>
#include <universes/hash/HashUniverse.h>
#include <universes/life/LifeUniverse.h>

#include <QButtonGroup>
#include <QColorDialog>
#include <QFile>
#include <QFileDialog>
#include <QInputDialog>
#include <QMenuBar>
#include <QMessageBox>
#include <QSettings>
#include <QStatusBar>
#include <QTextStream>
#include <QToolBar>
#include <QToolButton>
#include <iostream>
#include <string>

MainWindow::MainWindow() : settings("aging.conf", QSettings::NativeFormat) {
  isSystemTheme = settings.value("isSystemTheme", true).toBool();
  if (isSystemTheme) {
    isDarkTheme = window()->palette().window().color().lightnessF() < 0.5;
  } else {
    isDarkTheme = settings.value("isDarkTheme", false).toBool();
  }

  resources = ResourceManager(isDarkTheme);

  resize(720, 720);
  setFocusPolicy(Qt::FocusPolicy::ClickFocus);

  universe = new HashUniverse(8);
  univ_type = UniverseType::Hashlife;
  for (int i = 64; i < 192; ++i) {
    for (int j = 64; j < 192; ++j) {
      universe->set(Vec2(i, j), ((i + j) % 12 == 0 || (i - j) % 11 == 0));
    }
  }

  createUI();
}

MainWindow::~MainWindow() {}

/////////////////////////////////////////////////////
////           Creation de l'interface           ////
/////////////////////////////////////////////////////

void MainWindow::createUI() {
  createCentralWidget();
  createToolBar();
  createMenuBar();
}

void MainWindow::createCentralWidget() {
  ctxt.universe_scene = new UniverseScene(this, universe, univ_type);
  setCentralWidget(ctxt.universe_scene);

  QString color_theme = settings.value("colorTheme", "#8bb158").toString();
  QString bg;
  if (isDarkTheme) {
    bg = "#181913";
  } else {
    bg = "#C5C5C5";
  }

  QString color1 = settings.value("colorGrid", color_theme).toString();
  QString color2 = settings.value("colorBg", bg).toString();
  QString color3 = settings.value("colorFg", color_theme).toString();

  ctxt.universe_scene->set_grid_color(QColor(color1));
  ctxt.universe_scene->set_cell_color(0, QColor(color2));
  ctxt.universe_scene->set_cell_color(1, QColor(color3));
}

void MainWindow::createToolBar() {
  QToolBar *controlToolbar = addToolBar("Simulation Controls");
  ctxt.toolbar = controlToolbar;
  addToolBar(Qt::LeftToolBarArea, controlToolbar);

  // new file
  connect(controlToolbar->addAction(resources.getIcon("file"), "New File"),
          &QAction::triggered, this, &MainWindow::action_newFile);

  // open file
  connect(
      controlToolbar->addAction(resources.getIcon("folder-open"), "Open File"),
      &QAction::triggered, this, &MainWindow::action_openFile);

  // save file
  connect(controlToolbar->addAction(resources.getIcon("floppy"), "Save File"),
          &QAction::triggered, this, &MainWindow::action_saveFile);

  controlToolbar->addSeparator();

  // Button Play/pause
  ctxt.playIcon = new QIcon(resources.getIconPath("play"));
  ctxt.pauseIcon = new QIcon(resources.getIconPath("pause"));
  ctxt.playPauseAction =
      controlToolbar->addAction(*(ctxt.playIcon), "Play/Pause");
  ctxt.playPauseAction->setShortcut(QKeySequence(Qt::Key_Space));
  connect(ctxt.playPauseAction, &QAction::triggered, this,
          &MainWindow::action_playPause);

  // Button One Step
  QAction *act_step = controlToolbar->addAction(resources.getIcon("step"),
                                                "Advance one step");
  act_step->setShortcut(QKeySequence(Qt::Key_P));
  connect(act_step, &QAction::triggered, this, &MainWindow::action_step);

  // Button Increase Speed
  QAction *act_inc_speed = controlToolbar->addAction(
      resources.getIcon("fast-forward"), "Increase Speed");
  act_inc_speed->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Plus));
  connect(act_inc_speed, &QAction::triggered, this,
          &MainWindow::action_incSpeed);

  // Button Decrease Speed
  QAction *act_dec_speed = controlToolbar->addAction(
      resources.getIcon("rewind"), "Decrease Speed");
  act_dec_speed->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Minus));
  connect(act_dec_speed, &QAction::triggered, this,
          &MainWindow::action_decSpeed);

  controlToolbar->addSeparator();

  // Bouton Fit Pattern
  QAction *act_fit = controlToolbar->addAction(resources.getIcon("bullseye"),
                                               "Fit Pattern");
  act_fit->setShortcut(QKeySequence(Qt::Key_F));
  connect(act_fit, &QAction::triggered, this, &MainWindow::action_fitPattern);

  controlToolbar->addSeparator();

  // MouseMode
  QActionGroup *checkableGroup = new QActionGroup(controlToolbar);
  checkableGroup->setExclusive(true);

  QAction *pencilAction =
      controlToolbar->addAction(resources.getIcon("pencil"), "Edit");
  connect(pencilAction, &QAction::triggered, this,
          &MainWindow::action_modeEdit);
  pencilAction->setCheckable(true);
  pencilAction->setActionGroup(checkableGroup);

  QAction *selectAction =
      controlToolbar->addAction(resources.getIcon("table"), "Select");
  connect(selectAction, &QAction::triggered, this,
          &MainWindow::action_modeSelect);
  selectAction->setCheckable(true);
  selectAction->setActionGroup(checkableGroup);

  QAction *moveAction =
      controlToolbar->addAction(resources.getIcon("cursor-move"), "Move");
  connect(moveAction, &QAction::triggered, this, &MainWindow::action_modeMove);
  moveAction->setCheckable(true);
  moveAction->setActionGroup(checkableGroup);

  controlToolbar->addSeparator();

  // bouton d'un zoom centré
  QAction *act_zoomIn =
      controlToolbar->addAction(resources.getIcon("zoom-in"), "Zoom In");
  act_zoomIn->setShortcut(QKeySequence(Qt::Key_Plus));
  connect(act_zoomIn, &QAction::triggered, this, &MainWindow::action_zoomIn);

  // bouton d'un dezoom centré
  QAction *act_zoomOut =
      controlToolbar->addAction(resources.getIcon("zoom-out"), "Zoomt Out");
  act_zoomOut->setShortcut(QKeySequence(Qt::Key_Minus));
  connect(act_zoomOut, &QAction::triggered, this, &MainWindow::action_zoomOut);

  controlToolbar->addSeparator();

  // bouton exit
  connect(controlToolbar->addAction(resources.getIcon("logout"), "Quit"),
          &QAction::triggered, this, &MainWindow::close);
}

void MainWindow::createMenuBar() {
  // File
  QMenu *fileMenu = new QMenu("File");
  // new file
  QAction *act_new_file = fileMenu->addAction("New File");
  act_new_file->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_N));
  connect(act_new_file, &QAction::triggered, this, &MainWindow::action_newFile);

  // open file
  QAction *act_open_file = fileMenu->addAction("Open File");
  act_open_file->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_O));
  connect(act_open_file, &QAction::triggered, this,
          &MainWindow::action_openFile);

  // save file
  QAction *act_save_file = fileMenu->addAction("Save File");
  act_save_file->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_S));
  connect(act_save_file, &QAction::triggered, this,
          &MainWindow::action_saveFile);

  fileMenu->addSeparator();

  // exit
  QAction *act_quit = fileMenu->addAction("Quit");
  act_quit->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Q));
  connect(act_quit, &QAction::triggered, this, &MainWindow::close);
  menuBar()->addMenu(fileMenu);

  // Option
  QMenu *optMenu = new QMenu("Options");

  QMenu *algoMenu = new QMenu("Set Algorithm");

  QActionGroup *groupAlgo = new QActionGroup(algoMenu);

  QAction *algo1 = algoMenu->addAction("Hashlife");
  QAction *algo2 = algoMenu->addAction("Basic");
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
  connect(algo1, &QAction::triggered, this,
          &MainWindow::action_newUnivTypeHashlife);
  connect(algo2, &QAction::triggered, this,
          &MainWindow::action_newUnivTypeNaive);
  optMenu->addMenu(algoMenu);

  optMenu->addSeparator();

  QAction *expansive = optMenu->addAction("Force expansive");
  expansive->setCheckable(true);
  connect(expansive, &QAction::toggled, this, &MainWindow::action_forceExpanse);

  menuBar()->addMenu(optMenu);

  // Preferences
  QMenu *prefMenu = new QMenu("Preferences");
  // set color
  QMenu *prefColor = new QMenu("Set Color");
  connect(prefColor->addAction("Background"), &QAction::triggered, this,
          &MainWindow::action_setColorBg);

  connect(prefColor->addAction("Cell"), &QAction::triggered, this,
          &MainWindow::action_setColorFg);

  connect(prefColor->addAction("Grid"), &QAction::triggered, this,
          &MainWindow::action_setColorGrid);
  prefMenu->addMenu(prefColor);

  // Toggle Bord
  /*
  QAction* toggle_bord = prefMenu->addAction("Infinite Grid");
    toggle_bord->setCheckable(true);
    connect(toggle_bord,
      &QAction::toggled, this, &MainWindow::action_setInfiniteGrid);
  */
  connect(prefMenu->addAction("Set Rank Grid"), &QAction::triggered, this,
          &MainWindow::action_setRankGrid);

  prefMenu->addSeparator();

  QAction *sysThem = prefMenu->addAction("System Theme");
  sysThem->setCheckable(true);
  sysThem->setChecked(isSystemTheme);
  connect(sysThem, &QAction::triggered, this, &MainWindow::action_systemeTheme);

  QAction *darkThem = prefMenu->addAction("Dark Theme");
  darkThem->setCheckable(true);
  darkThem->setChecked(isDarkTheme);
  connect(darkThem, &QAction::triggered, this, &MainWindow::action_darkTheme);

  QAction *colThem = prefMenu->addAction("Color Theme");
  connect(colThem, &QAction::triggered, this, &MainWindow::action_colorTheme);

  if (isSystemTheme) {
    darkThem->setEnabled(false);
    colThem->setEnabled(false);
  }

  menuBar()->addMenu(prefMenu);

  // Help
  QMenu *helpMenu = new QMenu("Help");

  connect(helpMenu->addAction("Help"), &QAction::triggered, this,
          &MainWindow::action_help);

  connect(helpMenu->addAction("Documentation"), &QAction::triggered, this,
          &MainWindow::action_documentation);

  helpMenu->addSeparator();

  connect(helpMenu->addAction("Licence"), &QAction::triggered, this,
          &MainWindow::action_licence);

  connect(helpMenu->addAction("About"), &QAction::triggered, this,
          &MainWindow::action_about);

  menuBar()->addMenu(helpMenu);
}

/////////////////////////////////////////////////////
////                   Actions                   ////
/////////////////////////////////////////////////////

void MainWindow::action_newFile() {
  switch (univ_type) {
  case UniverseType::Hashlife:
    action_newUnivTypeHashlife();
    break;
  case UniverseType::Life:
    action_newUnivTypeNaive();
    break;
  }
}
void MainWindow::action_openFile() {
  QString fileName, acceptedFormats;
  switch (univ_type) {
  case UniverseType::Hashlife:
    acceptedFormats = "Pattern files (*.rle *.mc)";
    break;
  case UniverseType::Life:
    acceptedFormats = "Pattern files (*.rle)";
    break;
  }

  fileName =
      QFileDialog::getOpenFileName(this, "Open File", "", acceptedFormats);
  if (!fileName.isNull()) {
    delete universe;
    delete ctxt.universe_scene;
    switch (univ_type) {
    case UniverseType::Hashlife:
      universe = new HashUniverse(fileName);
      break;
    case UniverseType::Life:
      universe = new LifeUniverse(fileName);
      break;
    }
    createCentralWidget();
  }
}
void MainWindow::action_saveFile() {
  printf("MainWindow::action_saveFile() not Implemented\n");
}

void MainWindow::action_playPause() {
  ctxt.universe_scene->play_pause();
  if (ctxt.universe_scene->get_state_simulation()) {
    ctxt.playPauseAction->setIcon(*(ctxt.pauseIcon));
  } else {
    ctxt.playPauseAction->setIcon(*(ctxt.playIcon));
  }
}
void MainWindow::action_step() { ctxt.universe_scene->step(); }
void MainWindow::action_incSpeed() {
  ctxt.universe_scene->increase_speed();
  ctxt.universe_scene->get_speed();
}
void MainWindow::action_decSpeed() {
  ctxt.universe_scene->decrease_speed();
  ctxt.universe_scene->get_speed();
}

void MainWindow::action_fitPattern() { ctxt.universe_scene->fit_pattern(); }

void MainWindow::action_modeEdit() { ctxt.universe_scene->set_mode(EDIT); }
void MainWindow::action_modeSelect() { ctxt.universe_scene->set_mode(SELECT); }
void MainWindow::action_modeMove() { ctxt.universe_scene->set_mode(MOVE); }

void MainWindow::action_zoomIn() { ctxt.universe_scene->zoom_in(); }
void MainWindow::action_zoomOut() { ctxt.universe_scene->zoom_out(); }

void MainWindow::action_forceExpanse() {
  printf("Toggle hyperspeed mode not Implemented\n");
}

void MainWindow::action_setColorBg() {
  QColor color = QColorDialog::getColor(ctxt.universe_scene->get_cell_color(0),
                                        this, "Choose Background Color");
  settings.setValue("colorBg", color.name());
  ctxt.universe_scene->set_cell_color(0, color);
}
void MainWindow::action_setColorFg() {
  QColor color = QColorDialog::getColor(ctxt.universe_scene->get_cell_color(1),
                                        this, "Choose Cell Color");

  settings.setValue("colorFg", color.name());
  ctxt.universe_scene->set_cell_color(1, color);
}
void MainWindow::action_setColorGrid() {
  QColor color = QColorDialog::getColor(ctxt.universe_scene->get_grid_color(),
                                        this, "Choose Grid Color");
  ctxt.universe_scene->set_grid_color(color);
  settings.setValue("colorGrid", color.name());
}

void MainWindow::action_setInfiniteGrid() {
  ctxt.universe_scene->toggle_bord();
}
void MainWindow::action_setRankGrid() {
  bool ok;
  std::string s;
  int rank =
      QInputDialog::getInt(this, "Set Rang Grid", s.c_str(),
                           ctxt.universe_scene->get_rank_grid(), 0, 50, 1, &ok);
  if (ok) {
    ctxt.universe_scene->set_rank_grid(rank);
  }
}

void MainWindow::action_colorTheme() {
  QMessageBox msgBox;
  msgBox.setText("Changing the theme will take effect after "
                 "restarting the application.\nContinue ?");
  msgBox.setStandardButtons(QMessageBox::Cancel | QMessageBox::Yes);
  msgBox.setDefaultButton(QMessageBox::Cancel);
  int ret = msgBox.exec();

  if (ret == QMessageBox::Yes) {
    QColor color = QColorDialog::getColor(
        QColor(settings.value("colorTheme", "#8bb158").toString()), this,
        "Choose Cell Color");

    settings.setValue("colorTheme", color.name());
    close();
  }
}

void MainWindow::action_systemeTheme() {
  QMessageBox msgBox;
  msgBox.setText("Changing the theme will take effect after "
                 "restarting the application.");
  msgBox.setStandardButtons(QMessageBox::Cancel | QMessageBox::Ok);
  msgBox.setDefaultButton(QMessageBox::Cancel);
  int ret = msgBox.exec();

  if (ret == QMessageBox::Ok) {
    isSystemTheme = !isSystemTheme;
    settings.setValue("isSystemTheme", isSystemTheme);
    close();
  }
}

void MainWindow::action_darkTheme() {
  QMessageBox msgBox;
  msgBox.setText("Changing the theme will take effect after "
                 "restarting the application.");
  msgBox.setStandardButtons(QMessageBox::Cancel | QMessageBox::Ok);
  msgBox.setDefaultButton(QMessageBox::Cancel);
  int ret = msgBox.exec();

  if (ret == QMessageBox::Ok) {
    isDarkTheme = !isDarkTheme;
    settings.setValue("isDarkTheme", isDarkTheme);
    close();
  }
}

void MainWindow::action_help() {
  QFile file("../res/html/help.html");
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    return;
  }
  QString text;
  QTextStream in(&file);
  while (!in.atEnd()) {
    text += in.readLine();
  }
  QMessageBox msgBox;
  msgBox.setWindowTitle("Help");
  msgBox.setTextFormat(Qt::RichText);
  msgBox.setText(text);
  msgBox.exec();
}
void MainWindow::action_documentation() {
  QFile file("../res/html/doc.html");
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    return;
  }
  QString text;
  QTextStream in(&file);
  while (!in.atEnd()) {
    text += in.readLine();
  }
  QMessageBox msgBox;
  msgBox.setWindowTitle("Documentation");
  msgBox.setTextFormat(Qt::RichText);
  msgBox.setText(text);
  msgBox.exec();
}
void MainWindow::action_licence() {
  QFile file("../res/html/licence.html");
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    return;
  }
  QString text;
  QTextStream in(&file);
  while (!in.atEnd()) {
    text += in.readLine();
  }
  QMessageBox msgBox;
  msgBox.setWindowTitle("Licence");
  msgBox.setTextFormat(Qt::RichText);
  msgBox.setText(text);
  msgBox.exec();
}
void MainWindow::action_about() {
  QFile file("../res/html/about.html");
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    return;
  }
  QString text;
  QTextStream in(&file);
  while (!in.atEnd()) {
    text += in.readLine();
  }
  QMessageBox msgBox;
  msgBox.setWindowTitle("About");
  msgBox.setTextFormat(Qt::RichText);
  msgBox.setText(text);
  msgBox.exec();
}

void MainWindow::action_newUnivTypeHashlife() {
  delete universe;
  delete ctxt.universe_scene;

  universe = new HashUniverse(8);
  univ_type = UniverseType::Hashlife;

  createCentralWidget();
}
void MainWindow::action_newUnivTypeNaive() {
  delete universe;
  delete ctxt.universe_scene;

  universe = new LifeUniverse(Vec2(BigInt(1024), BigInt(1024)));
  univ_type = UniverseType::Life;

  createCentralWidget();
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
  Vec2 coord = ctxt.universe_scene->map_coords(rpos);
  switch (ctxt.universe_scene->get_mode()) {
  case SceneMode::MOVE:
    if (ctxt.pressed_button == Qt::LeftButton) {
      QPointF diff = rpos - ctxt.drag_position;
      if (diff.manhattanLength() > 42) {
        ctxt.drag_position = rpos;
        ctxt.universe_scene->move_camera(
            {-diff.x() / ctxt.universe_scene->width(),
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
