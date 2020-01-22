#ifndef UI_RENDERAREA_H_
#define UI_RENDERAREA_H_

#include "../logic/Universe.h"
#include "../gameoflife/Game.h"
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QWheelEvent>

class RenderArea : public QOpenGLWidget , protected QOpenGLFunctions {

public:
	RenderArea(QWidget *parent = 0, Universe *universe = 0);

protected:
	void initializeGL() override;
	void resizeGL(int w, int h) override;
	void paintGL() override;
	void wheelEvent(QWheelEvent *event) override;

private:
	Universe *universe;
	std::string vertexShaderSource;
	std::string fragmentShaderSource;

	GLuint m_posAttr;
	GLuint m_colAttr;
	GLuint m_matrixUniform;

	QOpenGLShaderProgram *m_program;
};

#endif /* UI_RENDERAREA_H_ */
