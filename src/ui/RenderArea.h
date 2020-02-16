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
	RenderArea(QWidget *parent = 0, CellMap *gol_map= 0);
	RenderArea(QWidget *parent = 0, Universe *hashlife_universe = 0);
	void handleInput(QKeyEvent *event);
	void zoomin_event(QPoint origin);
	void zoomout_event(QPoint origin);

protected:
	void initializeGL() override;
	void resizeGL(int w, int h) override;
	void paintGL() override;
	void wheelEvent(QWheelEvent *event) override;

private:
	CellMap *gol_map;
	Universe *hashlife_universe;
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
	GLuint square_elements[4] = {
		0,1,3,2,
	};
	GLuint square_ebo;

	QOpenGLShaderProgram *m_program;
	Camera2D *camera;

	void render_gol(QMatrix4x4 &matrix);
	void render_hashlife(QMatrix4x4 &matrix);

};

#endif /* UI_RENDERAREA_H_ */
