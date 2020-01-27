#include "RenderArea.h"
#include <iostream>
#include <QSurfaceFormat>

RenderArea::RenderArea(QWidget *parent, CellMap *map)
	:QOpenGLWidget(parent), map(map)
{

}

void RenderArea::initializeGL() {

	this->vertexShaderSource =
		"attribute highp vec4 posAttr;\n"
		"attribute lowp vec4 colAttr;\n"
		"varying lowp vec4 col;\n"
		"uniform highp mat4 matrix;\n"
		"void main() {\n"
		"	col = colAttr;\n"
		"	gl_Position = matrix * posAttr;\n"
		"}\n";

	this->fragmentShaderSource = 
		"varying lowp vec4 col;\n"
		"void main() {\n"
		"	gl_FragColor = vec4(1.0f,1.0f,1.0f,1.0f);\n"
		"}\n";
	

	initializeOpenGLFunctions();

	m_program = new QOpenGLShaderProgram(this);
	m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource.c_str());
	m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource.c_str());
	m_program->link();
	m_posAttr = m_program->attributeLocation("posAttr");
	m_colAttr = m_program->attributeLocation("colAttr");
	m_matrixUniform = m_program->uniformLocation("matrix");

	camera = new Camera2D();
	
	//camera->origin.setX(width() / 2.0f);
	//camera->origin.setY(height() / 2.0f);
	camera->pos.setX(0.0f);
	camera->pos.setY(0.0f);
	camera->set_zoom(1.0f);

	glClearColor(0.0f,0.0f,0.0f,1.0f);
}

void RenderArea::resizeGL(int w, int h) {
	update();
}

void RenderArea::paintGL() {
	std::cout << "Refreshed OpenGL display" << '\n';


	const qreal retinaScale = devicePixelRatio();
	glViewport(0, 0, width() * retinaScale, height() * retinaScale);

	glClear(GL_COLOR_BUFFER_BIT);

	m_program->bind();

	QMatrix4x4 matrix = camera->get_transformation();


	float vertices[] = {
    -0.5f,  0.5f,  // Top-left
     0.5f,  0.5f,  // Top-right
     0.5f, -0.5f,  // Bottom-right

     0.5f, -0.5f,  // Bottom-right
    -0.5f, -0.5f,  // Bottom-left
    -0.5f,  0.5f,  // Top-left
	};

	GLfloat colors[] = {
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f
	};

	for(size_t c = 0; c< map->getWidth();c++) {

		matrix.translate(1.0f,0.0f,0.0f);
		for(size_t l = 0; l< map->getHeight(); l++) {
			matrix.translate(0.0f,-1.0f,0.0f);
			if(map->isAlive(c,l)){
				m_program->setUniformValue(m_matrixUniform, matrix);

				glVertexAttribPointer(m_posAttr, 2, GL_FLOAT, GL_FALSE, 0, vertices);
				glVertexAttribPointer(m_colAttr, 3, GL_FLOAT, GL_FALSE, 0, colors);

				glEnableVertexAttribArray(0);
				glEnableVertexAttribArray(1);

				glDrawArrays(GL_TRIANGLES, 0, 6);

				glDisableVertexAttribArray(1);
				glDisableVertexAttribArray(0);
			}
		}
		matrix.translate(0.0f,map->getHeight(),0.0f);

	}
	m_program->release();

}

void RenderArea::wheelEvent(QWheelEvent *event) {
	camera->set_zoom(camera->get_zoom() - event->delta() / 100.0f);
	update();
}
void RenderArea::mousePressEvent(QMouseEvent *event){
	map->nextGeneration();
	update();
}

void RenderArea::handleInput(QKeyEvent *event) {
	switch (event->key())
	{
	case Qt::Key_Space:
		map->nextGeneration();
		break;
	case Qt::Key_Z:
		camera->pos.setY(camera->pos.y() + 0.05f);
		break;
	case Qt::Key_S:
		camera->pos.setY(camera->pos.y() - 0.05f);
		break;
	case Qt::Key_Q:
		camera->pos.setX(camera->pos.x() - 0.05f);
		break;
	case Qt::Key_D:
		camera->pos.setX(camera->pos.x() + 0.05f);
		break;
	
	default:
		break;
	}
	update();

}