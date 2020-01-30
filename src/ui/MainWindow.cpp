#include "MainWindow.h"
#include "../logic/Universe.h"

#include <iostream>
#include <QToolBar>

MainWindow::MainWindow() {
	this->resize(720, 720);

	this->setFocusPolicy(Qt::FocusPolicy::ClickFocus);

	CellMap *map = new CellMap(100,100);
	for (size_t c = 0; c < map->getWidth(); ++c)
    	for (size_t l = 0; l < map->getHeight(); ++l)
      		map->changeCellState(c,l,(c + l) % 2);

	r_area = new RenderArea(this,map);
	this->setCentralWidget(r_area);
	QToolBar *controlToolbar = this->addToolBar("Controls");
	QIcon *playIcon = new QIcon("../res/icons/play.svg");
	QAction *playAction = controlToolbar->addAction(*playIcon,"Play");
	QIcon *pauseIcon = new QIcon("../res/icons/pause.svg");
	QAction *pauseAction = controlToolbar->addAction(*pauseIcon,"Pause");
}

MainWindow::~MainWindow() {
}

void MainWindow::keyPressEvent(QKeyEvent *event){
	r_area->handleInput(event);
}

