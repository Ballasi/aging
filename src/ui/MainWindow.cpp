#include "MainWindow.h"
#include "../logic/Universe.h"

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
			map->changeCellState(c,l,c % 3);

	createUI();
	stepTimer = new QTimer(this);
	connect(stepTimer, &QTimer::timeout, this, &MainWindow::stepSimulation);
}

void MainWindow::createUI() {

	//Render Area
	r_area = new RenderArea(this,game->getMap());
	setCentralWidget(r_area);

	//Simulation control tollbar
	QToolBar *controlToolbar = addToolBar("Controls");
	addToolBar(Qt::LeftToolBarArea, controlToolbar);
	playIcon = new QIcon("../res/icons/play.svg");
	pauseIcon = new QIcon("../res/icons/pause.svg");
	playPauseAction = controlToolbar->addAction(*playIcon,"Play/Pause");
	QIcon *stepIcon = new QIcon("../res/icons/step.svg");
	QAction *stepAction = controlToolbar->addAction(*stepIcon,"Advance one step");

	connect(playPauseAction, &QAction::triggered, this, &MainWindow::playPause);

	updateStatusBar();

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

