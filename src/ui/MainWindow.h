#ifndef UI_MAINWINDOW_H_
#define UI_MAINWINDOW_H_

#include <ui/UniverseScene.hpp>
#include <QMainWindow>
#include <QSettings>
#include <QTimer>
#include <QToolButton>
#include <logic/UniverseType.h>

typedef struct SwidgetsCtxt {
  UniverseScene *universe_scene;

  QAction *playPauseAction; // Bouton qui change d'apparence
    QIcon *playIcon;  // Play
    QIcon *pauseIcon; // Pause

  QAction *mouseModeAction; // Bouton qui change d'apparence
    QIcon *moveIcon;   // Move
    QIcon *editIcon;   // Edit
    QIcon *selectIcon; // Select
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

  // Fonctions actions lance par des connects
  void funcAction_playPause();
  void funcAction_step();
  void funcAction_fitPattern();
  void funcAction_zoomIn();
  void funcAction_zoomOut();
  void funcAction_mode();

private:
  WidgetsCtxt ctxt;

  bool isDarkTheme;

  Universe *universe;
  UniverseType univ_type;

  void createUI();
};

#endif /* UI_MAINWINDOW_H_ */
