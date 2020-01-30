#ifndef UI_MAINWINDOW_H_
#define UI_MAINWINDOW_H_

#include <QMainWindow>
#include "RenderArea.h"

class MainWindow : public QMainWindow {
	Q_OBJECT

public:
	MainWindow();
	virtual ~MainWindow();

public slots:
	void stepSimulation();
	void load();

protected:
	void keyPressEvent(QKeyEvent *event);

private:
	RenderArea *r_area;
	Game *game;
	void createUI();
	void updateStatusBar();

	bool simulationRunning;

};

#endif /* UI_MAINWINDOW_H_ */
