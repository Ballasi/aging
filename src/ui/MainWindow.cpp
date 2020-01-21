#include "MainWindow.h"
#include "RenderArea.h"
#include "../logic/Universe.h"

MainWindow::MainWindow() {
	this->resize(1280, 720);

	Universe *universe = new Universe();

	RenderArea *r_area = new RenderArea(this,universe);
	this->setCentralWidget(r_area);
}

MainWindow::~MainWindow() {
}

