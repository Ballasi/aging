#include "RenderArea.h"
#include <iostream>
#include <QSurfaceFormat>

RenderArea::RenderArea(QWidget *parent, CellMap *map)
	:QOpenGLWidget(parent), map(map)
{

}

void RenderArea::initializeGL() {

	this->vertexShaderSource =
		"attribute lowp vec2 posAttr;\n"
		"uniform lowp mat4 matrix;\n"
		"void main() {\n"
		"	gl_Position = matrix * vec4(posAttr,1,1);\n"
		"}\n";

	this->fragmentShaderSource = 
		"void main() {\n"
		"	gl_FragColor = vec4(1.0f,1.0f,1.0f,1.0f);\n"
		"}\n";
	

	initializeOpenGLFunctions();

	m_program = new QOpenGLShaderProgram(this);
	m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource.c_str());
	m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource.c_str());
	m_program->link();
	m_posAttr = m_program->attributeLocation("posAttr");
	m_matrixUniform = m_program->uniformLocation("matrix");

	glGenBuffers(1,&square_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,square_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(square_elements), square_elements, GL_STATIC_DRAW);

	camera = new Camera2D();
	
	camera->pos.setX(0.0f);
	camera->pos.setY(0.0f);
	camera->set_zoom(1.0f);

	glClearColor(0.0f,0.0f,0.0f,1.0f);
	glDepthMask(0);
}

void RenderArea::resizeGL(int w, int h) {
	
	const qreal retinaScale = devicePixelRatio();
	glViewport(0, 0, width(), height() * retinaScale);
	camera->set_aspect_ratio((float)w / (float) h);
	update();
}

void RenderArea::paintGL() {
	std::cout << "Refreshed OpenGL display" << '\n';


	const qreal retinaScale = devicePixelRatio();
	glViewport(0, 0, width() * retinaScale, height() * retinaScale);

	glClear(GL_COLOR_BUFFER_BIT);

	m_program->bind();
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(m_posAttr, 2, GL_FLOAT, GL_FALSE, 0, square_vertices);

	QMatrix4x4 matrix = camera->get_transformation((float)width(), (float) height());

	for(size_t c = 0; c< map->getWidth();c++) {

		matrix.translate(1.0f,0.0f,0.0f);
		for(size_t l = 0; l< map->getHeight(); l++) {
			matrix.translate(0.0f,-1.0f,0.0f);
			if(map->isAlive(c,l)){
				m_program->setUniformValue(m_matrixUniform, matrix);
				glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, 0);
			}
		}
		matrix.translate(0.0f,map->getHeight(),0.0f);

	}
	glDisableVertexAttribArray(0);
	m_program->release();

}

void RenderArea::wheelEvent(QWheelEvent *event) {
	camera->set_zoom(camera->get_zoom() - event->delta() / 100.0f);
	update();
}
void RenderArea::mousePressEvent(QMouseEvent *event){
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
	
	default:
		break;
	}
	update();

}