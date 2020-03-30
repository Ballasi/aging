#include <logic/HashlifeUniverse/HashlifeUniverse.h>
#include <logic/Universe.h>
#include <ui/MainWindow.h>

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
  this->resize(720, 720);
  this->setFocusPolicy(Qt::FocusPolicy::ClickFocus);

  simulationRunning = false;
  /*
  game = new Game(100,100);

  CellMap *map = game->getMap();
  for (size_t c = 0; c < map->getWidth(); ++c)
          for (size_t l = 0; l < map->getHeight(); ++l)
                  map->changeCellState(c,l,c % 3);
  */
  game = nullptr;
  hashlife_universe = new HashlifeUniverse(3);

  hashlife_universe->set(Coord(0, 0), 1);
  hashlife_universe->set(Coord(0, 7), 1);
  hashlife_universe->set(Coord(7, 0), 1);
  hashlife_universe->set(Coord(7, 7), 1);

  isDarkTheme = this->palette().window().color().lightnessF() < 0.5;

  createUI();
  stepTimer = new QTimer(this);
  connect(stepTimer, &QTimer::timeout, this, &MainWindow::stepSimulation);
}

void MainWindow::createUI() {
  updateStatusBar();

  // Render Area
  r_area = new RenderArea(this, hashlife_universe);
  setCentralWidget(r_area);

  r_area->setCursor(Qt::ArrowCursor);

  // Simulation control toolbar
  QToolBar *controlToolbar = addToolBar("Simulation Controls");
  addToolBar(Qt::LeftToolBarArea, controlToolbar);
  if (isDarkTheme)
    playIcon = new QIcon("../res/icons/dark/play.svg");
  else
    playIcon = new QIcon("../res/icons/light/play.svg");
  if (isDarkTheme)
    pauseIcon = new QIcon("../res/icons/dark/pause.svg");
  else
    pauseIcon = new QIcon("../res/icons/light/pause.svg");
  if (isDarkTheme)
    playPauseAction = controlToolbar->addAction(*playIcon, "Play/Pause");
  else
    playPauseAction = controlToolbar->addAction(*playIcon, "Play/Pause");
  QIcon *stepIcon = new QIcon("../res/icons/light/step.svg");
  if (isDarkTheme)
    stepIcon = new QIcon("../res/icons/dark/step.svg");

  QAction *stepAction =
      controlToolbar->addAction(*stepIcon, "Advance one step");

  connect(playPauseAction, &QAction::triggered, this, &MainWindow::playPause);
  connect(stepAction, &QAction::triggered, this, &MainWindow::stepSimulation);

  // Control toolbox
  QToolBar *toolboxToolbar = addToolBar("Toolbox");
  QButtonGroup *toolboxGroup = new QButtonGroup();
  toolboxGroup->setExclusive(true);
  addToolBar(Qt::LeftToolBarArea, toolboxToolbar);

  pencilButton = new QToolButton(toolboxToolbar);
  if (isDarkTheme)
    pencilButton->setIcon(QIcon("../res/icons/dark/pencil.svg"));
  else
    pencilButton->setIcon(QIcon("../res/icons/light/pencil.svg"));
  pencilButton->setCheckable(true);
  toolboxGroup->addButton(pencilButton);
  toolboxToolbar->addWidget(pencilButton);

  eraserButton = new QToolButton(toolboxToolbar);
  if (isDarkTheme)
    eraserButton->setIcon(QIcon("../res/icons/dark/eraser.svg"));
  else
    eraserButton->setIcon(QIcon("../res/icons/light/eraser.svg"));
  eraserButton->setCheckable(true);
  toolboxGroup->addButton(eraserButton);
  toolboxToolbar->addWidget(eraserButton);

  zoominButton = new QToolButton(toolboxToolbar);
  if (isDarkTheme)
    zoominButton->setIcon(QIcon("../res/icons/dark/zoom-in.svg"));
  else
    zoominButton->setIcon(QIcon("../res/icons/light/zoom-in.svg"));
  zoominButton->setCheckable(true);
  toolboxGroup->addButton(zoominButton);
  toolboxToolbar->addWidget(zoominButton);

  zoomoutButton = new QToolButton(toolboxToolbar);
  if (isDarkTheme)
    zoomoutButton->setIcon(QIcon("../res/icons/dark/zoom-out.svg"));
  else
    zoomoutButton->setIcon(QIcon("../res/icons/light/zoom-out.svg"));
  zoomoutButton->setCheckable(true);
  toolboxGroup->addButton(zoomoutButton);
  toolboxToolbar->addWidget(zoomoutButton);

  // Menu Bar
  QMenu *fileMenu = new QMenu("File");
  QMenu *prefMenu = new QMenu("Preferences");
  QAction *loadAction = fileMenu->addAction("Load pattern");
  QAction *changeColorsAction = prefMenu->addAction("Choose colors");
  QMenu *stepMenu = prefMenu->addMenu("Step size");
  QAction *stepSizeSelectAction = stepMenu->addAction("Set step size");
  stepSizeMaxAction = stepMenu->addAction("Maximize step size");
  stepSizeMaxAction->setCheckable(true);

  connect(stepSizeSelectAction, &QAction::triggered,this,&MainWindow::set_step_size);
  connect(stepSizeMaxAction, &QAction::changed, this, &MainWindow::set_step_size_maximized);
  connect(loadAction, &QAction::triggered, this, &MainWindow::load);
  connect(changeColorsAction, &QAction::triggered, this,
          &MainWindow::chooseColors);
  loadAction->setShortcut(QKeySequence::Open);

  menuBar()->addMenu(fileMenu);
  menuBar()->addMenu(prefMenu);
}

void MainWindow::playPause() {
  simulationRunning = !simulationRunning;
  if (simulationRunning) {
    playPauseAction->setIcon(*pauseIcon);
    stepTimer->start(50);
  } else {
    playPauseAction->setIcon(*playIcon);
    stepTimer->stop();
  }
}

void MainWindow::updateStatusBar() {
  std::string s;
  s += "Generation : ";
  if (game != nullptr)
    s += std::to_string(game->getGeneration());
  else if (hashlife_universe != nullptr)
    s += bigint_to_str(hashlife_universe->get_generation());
    s += " | Step size : 2^";
    s += std::to_string(hashlife_universe->get_step_size());
  statusBar()->showMessage(QString(s.c_str()));
}

void MainWindow::stepSimulation() {
  if (game != nullptr)
    game->nextGeneration();
  else if (hashlife_universe != nullptr)
    hashlife_universe->step();

  r_area->update();
  updateStatusBar();
}

void MainWindow::load() {
  QString fileName = QFileDialog::getOpenFileName(this, "Open File", "",
                                                  "Run Length Encoded (*.rle)");
  if (game != nullptr) {
    game->loadRLE(fileName);
  } else if (hashlife_universe != nullptr) {
    delete hashlife_universe;
    hashlife_universe = new HashlifeUniverse(fileName);
    delete r_area;
    r_area = new RenderArea(this, hashlife_universe);
    setCentralWidget(r_area);
  }
  updateStatusBar();
}

void MainWindow::set_step_size() {
  bool ok;
  std::string s;
  size_t max = ((HashlifeUniverse*)hashlife_universe)->get_top_level() - 2;
  s += "Step size (enter the exponent) max : ";
  s += std::to_string(max);
  int i = QInputDialog::getInt(this, "Enter a step size",s.c_str(), hashlife_universe->get_step_size(), 0, max, 1, &ok);

  if (ok) {
    stepSizeMaxAction->setChecked(false);
    ((HashlifeUniverse*)hashlife_universe)->set_step_size(i);
  }

  updateStatusBar();
}

void MainWindow::set_step_size_maximized() {

  ((HashlifeUniverse*)hashlife_universe)->set_step_size_maximized(stepSizeMaxAction->isChecked());
  updateStatusBar();
}

MainWindow::~MainWindow() { delete r_area; }

void MainWindow::keyPressEvent(QKeyEvent *event) { r_area->handleInput(event); }

void MainWindow::mousePressEvent(QMouseEvent *event) {
  if (r_area->underMouse()) {
    QPoint pos = event->pos();
    if (zoominButton->isChecked()) {
      r_area->zoomin_event(r_area->mapFromParent(pos));
    } else if (zoomoutButton->isChecked()) {
      r_area->zoomout_event(r_area->mapFromParent(pos));
    } else if (pencilButton->isChecked()) {
      Coord c = r_area->map_coords_from_mouse(r_area->mapFromParent(pos));
      std::cout << "Coords : " << c.x << "," << c.y << "\n";
      hashlife_universe->set(c, 1);
      r_area->update();
    } else if (eraserButton->isChecked()) {
      Coord c = r_area->map_coords_from_mouse(r_area->mapFromParent(pos));
      std::cout << "Coords : " << c.x << "," << c.y << "\n";
      hashlife_universe->set(c, 0);
      r_area->update();
    }
  }
}

void MainWindow::chooseColors() {
  QColor cell_color =
      QColorDialog::getColor(Qt::white, this, "Choose cell color");
  QColor bg_color =
      QColorDialog::getColor(Qt::black, this, "Choose background color");

  QSettings settings("aging-team", "aging");
  settings.setValue("renderer/bg-color", bg_color);
  settings.setValue("renderer/cell-color", cell_color);

  r_area->set_colors(cell_color, bg_color);
  r_area->update();
}
