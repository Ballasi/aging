#ifndef UI_RENDERAREA_H_
#define UI_RENDERAREA_H_

#include "../logic/Universe.hpp"
#include "../gameoflife/Game.h"
#include "Camera2D.h"
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QWheelEvent>

class RenderArea : public QOpenGLWidget , protected QOpenGLFunctions {

public:
	RenderArea(QWidget *parent = 0, CellMap *map= 0);
	void handleInput(QKeyEvent *event);

protected:
	void initializeGL() override;
	void resizeGL(int w, int h) override;
	void paintGL() override;
	void wheelEvent(QWheelEvent *event) override;
	void mousePressEvent(QMouseEvent *event);

private:
	CellMap *map;
	std::string vertexShaderSource;
	std::string fragmentShaderSource;

	GLuint m_posAttr;
	GLuint m_matrixUniform;

	float square_vertices[8] = {
		-0.5f,  0.5f,  // Top-left
		0.5f,  0.5f,  // Top-right
		0.5f, -0.5f,  // Bottom-right
		-0.5f, -0.5f,  // Bottom-left
	};
	GLuint square_elements[6] = {
		0,1,2,
		2,3,0
	};
	GLuint square_ebo;

	QOpenGLShaderProgram *m_program;
	Camera2D *camera;

};

#endif /* UI_RENDERAREA_H_ */
