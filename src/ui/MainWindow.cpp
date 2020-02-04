#include "MainWindow.h"
#include "../logic/Universe.hpp"

#include <iostream>
#include <QToolBar>
#include <QStatusBar>
#include <QMenuBar>
#include <QFileDialog>

MainWindow::MainWindow() {
	this->resize(720, 720);
	this->setFocusPolicy(Qt::FocusPolicy::ClickFocus);

	simulationRunning = false;

	game = new Game(100,100);

	CellMap *map = game->getMap();
	for (size_t c = 0; c < map->getWidth(); ++c)
		for (size_t l = 0; l < map->getHeight(); ++l)
			map->changeCellState(c,l,(c + l) % 2);

	createUI();
}

void MainWindow::createUI() {

	//Render Area
	r_area = new RenderArea(this,game->getMap());
	setCentralWidget(r_area);

	//Simulation control tollbar
	QToolBar *controlToolbar = addToolBar("Controls");
	addToolBar(Qt::LeftToolBarArea, controlToolbar);
	QIcon *playIcon = new QIcon("../res/icons/play.svg");
	QAction *playAction = controlToolbar->addAction(*playIcon,"Play");
	QIcon *pauseIcon = new QIcon("../res/icons/pause.svg");
	QAction *pauseAction = controlToolbar->addAction(*pauseIcon,"Pause");
	QIcon *stepIcon = new QIcon("../res/icons/step.svg");
	QAction *stepAction = controlToolbar->addAction(*stepIcon,"Advance one step");

	connect(stepAction, &QAction::triggered, this, &MainWindow::stepSimulation);

	updateStatusBar();

	QMenu *fileMenu = new QMenu("File");
	QMenu *prefMenu = new QMenu("Preferences");
	QAction *loadAction = fileMenu->addAction("Load pattern");

	connect(loadAction, &QAction::triggered, this, &MainWindow::load);
	loadAction->setShortcut(QKeySequence::Open);

	menuBar()->addMenu(fileMenu);
	menuBar()->addMenu(prefMenu);
}

void MainWindow::updateStatusBar(){
	std::string s;
	s += "Generation : ";
	s += std::to_string(game->getGeneration());
	statusBar()->showMessage(QString(s.c_str()));
}

void MainWindow::stepSimulation(){
	game->nextGeneration();
	r_area->update();
	updateStatusBar();
}

void MainWindow::load(){
	QString fileName = QFileDialog::getOpenFileName(this, "Open File","","Run Length Encoded (*.rle)");
	updateStatusBar();
}

MainWindow::~MainWindow() {
	delete r_area;
}


void MainWindow::keyPressEvent(QKeyEvent *event){
	r_area->handleInput(event);
}

