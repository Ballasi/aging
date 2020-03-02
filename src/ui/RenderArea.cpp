#include "RenderArea.h"
#include <iostream>
#include <QSurfaceFormat>
#include "../logic/Rect.hpp"
#include "../logic/HashlifeUniverse/Iterator.hpp"
#include "../logic/HashlifeUniverse/HashlifeUniverse.hpp"

RenderArea::RenderArea(QWidget *parent, CellMap *gol_map)
	:QOpenGLWidget(parent), gol_map(gol_map), hashlife_universe(nullptr)
{

}
RenderArea::RenderArea(QWidget *parent, Universe *hashlife_universe)
	:QOpenGLWidget(parent), hashlife_universe(hashlife_universe), gol_map(nullptr)
{

}

void RenderArea::initializeGL() {

	this->vertexShaderSource =
		"attribute vec2 posAttr;\n"
		"uniform mat4 model;\n"
		"uniform mat4 view;\n"
		"uniform mat4 projection;\n"
		"void main() {\n"
		"	gl_Position = projection * view * model * vec4(posAttr,1,1);\n"
		"}\n";

	this->fragmentShaderSource = 
		"void main() {\n"
		"	gl_FragColor = vec4(1.0f,1.0f,1.0f,1.0f);\n"
		"}\n";
	

	initializeOpenGLFunctions();

	glClearColor(0.0f,0.0f,0.0f,1.0f);
	glDepthMask(0);

	m_program = new QOpenGLShaderProgram(this);
	m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource.c_str());
	m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource.c_str());
	m_program->link();
	m_posAttr = m_program->attributeLocation("posAttr");
	m_modelUniform = m_program->uniformLocation("model");
	m_viewUniform = m_program->uniformLocation("view");
	m_projectionUniform = m_program->uniformLocation("projection");

	glGenBuffers(1,&square_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,square_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(square_elements), square_elements, GL_STATIC_DRAW);

	float aspect_ratio = (float) width() / (float) height();

	// We create the projection matrix
	projectionMatrix.setToIdentity();
	projectionMatrix.ortho(QRectF(0.0f,0.0f,1.0f * aspect_ratio,-1.0f));


	// The Camera holds the view matrix and handles zooming
	camera = new Camera2D();
	camera->pos.setX(0.0f);
	camera->pos.setY(0.0f);

	if(hashlife_universe != nullptr)
		camera->set_zoom(1 << (hashlife_universe->get_top_level() + 1));
}

void RenderArea::resizeGL(int w, int h) {
	
	const qreal retinaScale = devicePixelRatio();
	glViewport(0, 0, width(), height() * retinaScale);

	float aspect_ratio = (float) width() / (float) height();
	projectionMatrix.setToIdentity();
	projectionMatrix.ortho(QRectF(0.0f,0.0f,1.0f * aspect_ratio,-1.0f));

	update();
}

void RenderArea::paintGL() {

	const qreal retinaScale = devicePixelRatio();
	glViewport(0, 0, width() * retinaScale, height() * retinaScale);

	glClear(GL_COLOR_BUFFER_BIT);

	m_program->bind();
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(m_posAttr, 2, GL_FLOAT, GL_FALSE, 0, square_vertices);

	QMatrix4x4 viewMatrix = camera->get_view();

	if(gol_map != nullptr) {
		render_gol(viewMatrix);
	} else if(hashlife_universe != nullptr) {
		render_hashlife(viewMatrix);
	} else {
		std::cout << "Error : No Universe detected" << std::endl;
	}

	glDisableVertexAttribArray(0);
	m_program->release();
}

void RenderArea::render_gol(QMatrix4x4 &matrix) {

	/*
	for(size_t c = 0; c< gol_map->getWidth();c++) {

		matrix.translate(1.0f,0.0f,0.0f);
		for(size_t l = 0; l< gol_map->getHeight(); l++) {
			matrix.translate(0.0f,-1.0f,0.0f);
			if(gol_map->isAlive(c,l)){
				m_program->setUniformValue(m_matrixUniform, matrix);
				glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, 0);
			}
		}
		matrix.translate(0.0f,gol_map->getHeight(),0.0f);

	}
	*/

}
//Rect RenderArea::get_visible_area()

void RenderArea::render_hashlife(QMatrix4x4 &viewMatrix) {

	Coord top_left = hashlife_universe->get_top_left();
	size_t level = hashlife_universe->get_top_level();

	//TODO : Calculate bounds that fit within camera's view
	Rect bounds;
	bounds.top_left = top_left;
	bounds.bottom_right = top_left + Coord(1 << level, 1 << level);


	std::cout << top_left.x << ',' << top_left.y << '\n';

	std::cout << "Camera : " << camera->pos.x() << ',' << camera->pos.y() << '\n';

	QMatrix4x4 modelMatrix;
	modelMatrix.setToIdentity();
	modelMatrix.translate(top_left.x,top_left.y,0);

	m_program->setUniformValue(m_viewUniform,viewMatrix);
	m_program->setUniformValue(m_projectionUniform,projectionMatrix);

	/*

	HashlifeUniverse::Iterator iterator((HashlifeUniverse *) hashlife_universe, bounds);
	Coord current_coord;
	CellState current_state;

	while(iterator.next(current_coord,current_state)){
		std::cout << current_coord.x << ',' << current_coord.y << '\n';
	}
	*/

	for(size_t c = 0; c < (1 << level);c++) {

		modelMatrix.translate(1.0f,0.0f,0.0f);
		for(size_t l = 0; l < (1 << level); l++) {
			modelMatrix.translate(0.0f,-1.0f,0.0f);
			if(hashlife_universe->get(Coord(c,l)) == 1) {
				m_program->setUniformValue(m_modelUniform, modelMatrix);
				glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, 0);
			}
		}
		modelMatrix.translate(0.0f,(1 << level),0.0f);
	}
}

void RenderArea::wheelEvent(QWheelEvent *event) {
	//camera->set_zoom(camera->get_zoom() - event->delta() / 100.0f);
	//update();
}
void RenderArea::zoomin_event(QPoint origin) {
	camera->set_zoom(camera->get_zoom() / 2);
	update();
}
void RenderArea::zoomout_event(QPoint origin) {
	camera->set_zoom(camera->get_zoom() * 2);
	update();
}

void RenderArea::handleInput(QKeyEvent *event) {
	switch (event->key())
	{
	case Qt::Key_Z:
		camera->pos.setY(camera->pos.y() + 0.08f);
		break;
	case Qt::Key_S:
		camera->pos.setY(camera->pos.y() - 0.08f);
		break;
	case Qt::Key_Q:
		camera->pos.setX(camera->pos.x() - 0.08f);
		break;
	case Qt::Key_D:
		camera->pos.setX(camera->pos.x() + 0.08f);
		break;
	case Qt::Key_Plus:
		zoomin_event(QPoint(0,0));
		break;
	case Qt::Key_Minus:
		zoomout_event(QPoint(0,0));
		break;
	
	default:
		break;
	}
	update();

}
