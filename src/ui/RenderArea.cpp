#include "RenderArea.h"
#include <iostream>

RenderArea::RenderArea(QWidget *parent, Universe *universe)
	:QOpenGLWidget(parent), universe(universe)
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
		"	gl_FragColor = col;\n"
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
	//matrix.perspective(60.0f, 16.0f/9.0f, 0.1f, 100.0f);

	m_program->setUniformValue(m_matrixUniform, matrix);

	GLfloat vertices[] = {
		0.0f, 1.0f,
		-1.0f, -1.0f,
		1.0f, -1.0f
	};

	GLfloat colors[] = {
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f
	};


	glVertexAttribPointer(m_posAttr, 2, GL_FLOAT, GL_FALSE, 0, vertices);
	glVertexAttribPointer(m_colAttr, 3, GL_FLOAT, GL_FALSE, 0, colors);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glDrawArrays(GL_TRIANGLES, 0, 3);

	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);

	m_program->release();

}

void RenderArea::wheelEvent(QWheelEvent *event) {
	camera->set_zoom(camera->get_zoom() - event->delta() / 100.0f);
	update();
}