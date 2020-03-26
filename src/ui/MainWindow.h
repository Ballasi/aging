#ifndef UI_MAINWINDOW_H_
#define UI_MAINWINDOW_H_

#include <ui/RenderArea.h>
#include <QMainWindow>
#include <QSettings>
#include <QTimer>
#include <QToolButton>

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow();
  virtual ~MainWindow();

public slots:
  void stepSimulation();
  void load();
  void playPause();
  void chooseColors();

protected:
  void keyPressEvent(QKeyEvent *event);
  void mousePressEvent(QMouseEvent *event);

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

  QToolButton *zoominButton, *zoomoutButton, *pencilButton, *eraserButton;
};

#endif /* UI_MAINWINDOW_H_ */
