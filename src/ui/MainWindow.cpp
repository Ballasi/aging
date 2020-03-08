#include "MainWindow.h"
#include "../logic/Universe.hpp"
#include "../logic/HashlifeUniverse/HashlifeUniverse.hpp"

#include <iostream>
#include <QToolBar>
#include <QStatusBar>
#include <QMenuBar>
#include <QFileDialog>
#include <QToolButton>
#include <QButtonGroup>

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

	hashlife_universe->set(Coord(0,0),1);
	hashlife_universe->set(Coord(0,7),1);
	hashlife_universe->set(Coord(7,0),1);
	hashlife_universe->set(Coord(7,7),1);

	createUI();
	stepTimer = new QTimer(this);
	connect(stepTimer, &QTimer::timeout, this, &MainWindow::stepSimulation);
}

void MainWindow::createUI() {

	updateStatusBar();

	//Render Area
	r_area = new RenderArea(this,hashlife_universe);
	setCentralWidget(r_area);

	r_area->setCursor(Qt::ArrowCursor);

	//Simulation control toolbar
	QToolBar *controlToolbar = addToolBar("Simulation Controls");
	addToolBar(Qt::LeftToolBarArea, controlToolbar);
	playIcon = new QIcon("../res/icons/play.svg");
	pauseIcon = new QIcon("../res/icons/pause.svg");
	playPauseAction = controlToolbar->addAction(*playIcon,"Play/Pause");
	QIcon *stepIcon = new QIcon("../res/icons/step.svg");
	QAction *stepAction = controlToolbar->addAction(*stepIcon,"Advance one step");

	connect(playPauseAction, &QAction::triggered, this, &MainWindow::playPause);
	connect(stepAction, &QAction::triggered, this, &MainWindow::stepSimulation);

	//Control toolbox
	QToolBar *toolboxToolbar = addToolBar("Toolbox");
	QButtonGroup *toolboxGroup = new QButtonGroup();
	toolboxGroup->setExclusive(true);
	addToolBar(Qt::LeftToolBarArea, toolboxToolbar);

	pencilButton = new QToolButton(toolboxToolbar);
	pencilButton->setIcon(QIcon("../res/icons/pencil.svg"));
	pencilButton->setCheckable(true);
	toolboxGroup->addButton(pencilButton);
	toolboxToolbar->addWidget(pencilButton);

	eraserButton = new QToolButton(toolboxToolbar);
	eraserButton->setIcon(QIcon("../res/icons/eraser.svg"));
	eraserButton->setCheckable(true);
	toolboxGroup->addButton(eraserButton);
	toolboxToolbar->addWidget(eraserButton);

	zoominButton = new QToolButton(toolboxToolbar);
	zoominButton->setIcon(QIcon("../res/icons/zoom-in.svg"));
	zoominButton->setCheckable(true);
	toolboxGroup->addButton(zoominButton);
	toolboxToolbar->addWidget(zoominButton);

	zoomoutButton = new QToolButton(toolboxToolbar);
	zoomoutButton->setIcon(QIcon("../res/icons/zoom-out.svg"));
	zoomoutButton->setCheckable(true);
	toolboxGroup->addButton(zoomoutButton);
	toolboxToolbar->addWidget(zoomoutButton);


	//Menu Bar
	QMenu *fileMenu = new QMenu("File");
	QMenu *prefMenu = new QMenu("Preferences");
	QAction *loadAction = fileMenu->addAction("Load pattern");

	connect(loadAction, &QAction::triggered, this, &MainWindow::load);
	loadAction->setShortcut(QKeySequence::Open);

	menuBar()->addMenu(fileMenu);
	menuBar()->addMenu(prefMenu);
}

void MainWindow::playPause() {

	simulationRunning = !simulationRunning;
	if(simulationRunning) {
		playPauseAction->setIcon(*pauseIcon);
		stepTimer->start(50);
	}else{
		playPauseAction->setIcon(*playIcon);
		stepTimer->stop();
	}

}

void MainWindow::updateStatusBar() {
	std::string s;
	s += "Generation : ";
	if(game != nullptr)
		s += std::to_string(game->getGeneration());
	else if(hashlife_universe != nullptr)
		s += std::to_string(hashlife_universe->get_generation());
	statusBar()->showMessage(QString(s.c_str()));
}

void MainWindow::stepSimulation() {
	if(game != nullptr)
		 game->nextGeneration();
	else if(hashlife_universe != nullptr) {
      hashlife_universe->step();
  }
  r_area->update();
	updateStatusBar();
}

void MainWindow::load() {
	QString fileName = QFileDialog::getOpenFileName(this, "Open File","","Run Length Encoded (*.rle)");
	if(game != nullptr) {
		game->loadRLE(fileName);
	} else if(hashlife_universe != nullptr) {
		delete hashlife_universe;
		hashlife_universe = new HashlifeUniverse(fileName);
		delete r_area;
		r_area = new RenderArea(this,hashlife_universe);
		setCentralWidget(r_area);
	}
	updateStatusBar();

}

MainWindow::~MainWindow() {
	delete r_area;
}


void MainWindow::keyPressEvent(QKeyEvent *event) {
	r_area->handleInput(event);
}

void MainWindow::mousePressEvent(QMouseEvent *event) {
	if(r_area->underMouse()){
		QPoint pos = event->pos();
		if(zoominButton->isChecked()){
			r_area->zoomin_event(r_area->mapFromParent(pos));
		}
		else if(zoomoutButton->isChecked()){
			r_area->zoomout_event(r_area->mapFromParent(pos));
		}
		else if(pencilButton->isChecked()){
			Coord c = r_area->map_coords_from_mouse(r_area->mapFromParent(pos));
			std::cout << "Coords : " << c.x << "," << c.y << "\n";
		}
		else if(eraserButton->isChecked()){

		}	
	}
}

