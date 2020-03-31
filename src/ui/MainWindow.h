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
  void set_step_size();
  void set_step_size_maximized();
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
  QAction *playPauseAction, *stepSizeMaxAction;
  QIcon *playIcon, *pauseIcon;
  QTimer *stepTimer;

  QToolButton *zoominButton, *zoomoutButton, *pencilButton, *eraserButton;

  bool isDarkTheme;
};

#endif /* UI_MAINWINDOW_H_ */
