#ifndef UI_MAINWINDOW_H_
#define UI_MAINWINDOW_H_

#include <QMainWindow>
#include <QTimer>
#include "RenderArea.h"

class MainWindow : public QMainWindow {
	Q_OBJECT

public:
	MainWindow();
	virtual ~MainWindow();

public slots:
	void stepSimulation();
	void load();
	void playPause();

protected:
	void keyPressEvent(QKeyEvent *event);

private:
	RenderArea *r_area;
	Game *game;
	Universe *hashlife_universe;
	void createUI();
	void updateStatusBar();

	bool simulationRunning;
	QAction *playPauseAction;
	QIcon *playIcon, *pauseIcon;
	QTimer *stepTimer;

};

#endif /* UI_MAINWINDOW_H_ */
