#include "RenderArea.h"

#include <QPainter>
#include <iostream>

RenderArea::RenderArea(QWidget *parent, Universe *universe)
	:QWidget(parent), universe(universe)
{
	setStyleSheet("background-color:black;");
	setAutoFillBackground(true);
}

void RenderArea::paintEvent(QPaintEvent* event) {

}

RenderArea::~RenderArea() {}

