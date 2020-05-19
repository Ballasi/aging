#ifndef UI_MAINWINDOW_H_
#define UI_MAINWINDOW_H_

#include <ui/UniverseScene.hpp>
#include <QMainWindow>
#include <QSettings>
#include <QTimer>
#include <QToolButton>
#include <model/Universe.h>

typedef struct SwidgetsCtxt {
  UniverseScene *universe_scene;

  QAction *playPauseAction; // Bouton qui change d'apparence
  QIcon *playIcon;  // Play
  QIcon *pauseIcon; // Pause

  Vec2 buffer_coord;
  Qt::MouseButton pressed_button;
  QPointF drag_position;
} WidgetsCtxt;



class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow();
  virtual ~MainWindow();


protected:
  // Reecriture des reactions a la souris et au clavier
  void keyPressEvent(QKeyEvent *event) override;
  void wheelEvent(QWheelEvent *event) override;
  void mousePressEvent(QMouseEvent *event) override;
  void mouseMoveEvent(QMouseEvent *event) override;
  // Fonctions actions lance par des connects
  void funcAction_newFile();
  void funcAction_openFile();
  void funcAction_saveFile();

  void funcAction_playPause();
  void funcAction_step();
  void funcAction_incSpeed();
  void funcAction_decSpeed();

  void funcAction_fitPattern();

  void funcAction_modeEdit();
  void funcAction_modeSelect();
  void funcAction_modeMove();

  void funcAction_zoomIn();
  void funcAction_zoomOut();

  void funcAction_setColorBg();
  void funcAction_setColorFg();
  void funcAction_setColorGrid();

  void funcAction_setInfiniteGrid();
  void funcAction_setRankGrid();
  void funcAction_darkTheme();

  void funcAction_help();
  void funcAction_documentation();
  void funcAction_licence();
  void funcAction_about();

  void funcAction_newUnivTypeHashlife();
  void funcAction_newUnivTypeNaive();

private:
  WidgetsCtxt ctxt;

  bool isDarkTheme;

  Universe *universe;
  UniverseType univ_type;

  void createUI();
  void createToolBar();
  void createMenuBar();
};

#endif /* UI_MAINWINDOW_H_ */
