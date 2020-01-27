#include "MainWindow.h"
#include "../logic/Universe.h"

#include <iostream>

MainWindow::MainWindow() {
	this->resize(1000, 1000);

	this->setFocusPolicy(Qt::FocusPolicy::ClickFocus);

	CellMap *map = new CellMap(100,100);
	for (size_t c = 0; c < map->getWidth(); ++c)
    	for (size_t l = 0; l < map->getHeight(); ++l)
      		map->changeCellState(c,l,(c + l) % 2);

	r_area = new RenderArea(this,map);
	this->setCentralWidget(r_area);
}

MainWindow::~MainWindow() {
}

void MainWindow::keyPressEvent(QKeyEvent *event){
	r_area->handleInput(event);
}

