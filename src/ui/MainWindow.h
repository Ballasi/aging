#ifndef UI_MAINWINDOW_H_
#define UI_MAINWINDOW_H_

#include <ui/UniverseScene.hpp>
#include <QMainWindow>
#include <QTimer>
#include <QSettings>
#include <QToolButton>
#include <logic/UniverseType.h>

typedef struct SwidgetsCtxt {
  UniverseScene *universe_scene;

  QAction *playPauseAction; // Bouton qui change d'apparence
    QIcon *playIcon;  // Play
    QIcon *pauseIcon; // Pause

  Coord buffer_coord;
  Qt::MouseButton pressed_button;
  QPointF drag_position;
  QToolBar* toolbar;
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
  void action_newFile();
  void action_openFile();
  void action_saveFile();

  void action_playPause();
  void action_step();
  void action_incSpeed();
  void action_decSpeed();

  void action_fitPattern();

  void action_modeEdit();
  void action_modeSelect();
  void action_modeMove();

  void action_zoomIn();
  void action_zoomOut();

  void action_forceExpanse();

  void action_setColorBg();
  void action_setColorFg();
  void action_setColorGrid();

  void action_setInfiniteGrid();
  void action_setRankGrid();
  void action_darkTheme();

  void action_help();
  void action_documentation();
  void action_licence();
  void action_about();

  void action_newUnivTypeHashlife();
  void action_newUnivTypeNaive();

private:
  WidgetsCtxt ctxt;

  bool isDarkTheme;

  Universe *universe;
  UniverseType univ_type;

  QSettings settings;

  void createUI();
  void createToolBar();
  void createMenuBar();
  void resetUI();
  void set_theme();
};

#endif /* UI_MAINWINDOW_H_ */
