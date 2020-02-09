#include "MainWindow.h"
#include "../logic/Universe.hpp"

#include <iostream>
#include <QToolBar>
#include <QStatusBar>
#include <QMenuBar>
#include <QFileDialog>
#include <QToolButton>

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
	hashlife_universe = new Universe(Coord(0,0),3);

	hashlife_universe->debug();

	hashlife_universe->set(Coord(0,0),1);
	hashlife_universe->set(Coord(1,0),1);
	hashlife_universe->set(Coord(2,0),1);
	hashlife_universe->set(Coord(3,0),1);
	hashlife_universe->set(Coord(4,0),1);

	hashlife_universe->debug();

	createUI();
	stepTimer = new QTimer(this);
	connect(stepTimer, &QTimer::timeout, this, &MainWindow::stepSimulation);
}

void MainWindow::createUI() {

	updateStatusBar();

	//Render Area
	r_area = new RenderArea(this,hashlife_universe);
	setCentralWidget(r_area);

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
	addToolBar(Qt::LeftToolBarArea, toolboxToolbar);

	QToolButton *pencilButton = new QToolButton(toolboxToolbar);
	pencilButton->setIcon(QIcon("../res/icons/pencil.svg"));
	pencilButton->setCheckable(true);	
	toolboxToolbar->addWidget(pencilButton);

	QToolButton *eraserButton = new QToolButton(toolboxToolbar);
	eraserButton->setIcon(QIcon("../res/icons/eraser.svg"));
	eraserButton->setCheckable(true);
	toolboxToolbar->addWidget(eraserButton);

	QToolButton *zoominButton = new QToolButton(toolboxToolbar);
	zoominButton->setIcon(QIcon("../res/icons/zoom-in.svg"));
	zoominButton->setCheckable(true);
	toolboxToolbar->addWidget(zoominButton);

	QToolButton *zoomoutButton = new QToolButton(toolboxToolbar);
	zoomoutButton->setIcon(QIcon("../res/icons/zoom-out.svg"));
	zoomoutButton->setCheckable(true);
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

void MainWindow::playPause(){
	simulationRunning = !simulationRunning;
	if(simulationRunning) {
		playPauseAction->setIcon(*pauseIcon);
		stepTimer->start(50);
	}else{
		playPauseAction->setIcon(*playIcon);
		stepTimer->stop();
	}

}

void MainWindow::updateStatusBar(){
	std::string s;
	s += "Generation : ";
	//s += std::to_string(game->getGeneration());
	statusBar()->showMessage(QString(s.c_str()));
}

void MainWindow::stepSimulation(){
	game->nextGeneration();
	r_area->update();
	updateStatusBar();
}

void MainWindow::load(){
	QString fileName = QFileDialog::getOpenFileName(this, "Open File","","Run Length Encoded (*.rle)");
	game->loadRLE(fileName);
	updateStatusBar();

}

MainWindow::~MainWindow() {
	delete r_area;
}


void MainWindow::keyPressEvent(QKeyEvent *event){
	r_area->handleInput(event);
}

