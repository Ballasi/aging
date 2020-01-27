#ifndef UI_MAINWINDOW_H_
#define UI_MAINWINDOW_H_

#include <QMainWindow>
#include "RenderArea.h"

class MainWindow : public QMainWindow {

public:
	MainWindow();
	virtual ~MainWindow();

protected:
	void keyPressEvent(QKeyEvent *event);

private:
	RenderArea *r_area;

};

#endif /* UI_MAINWINDOW_H_ */
