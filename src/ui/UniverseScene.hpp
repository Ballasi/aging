#ifndef UI_UNIVERSESCENE_H_
#define UI_UNIVERSESCENE_H_

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLWidget>
#include <QWheelEvent>
#include <ui/RenderArea.h>
#include <QMainWindow>
#include <QTimer>
#include <model/Universe.h>
#include <QSettings>

enum SceneMode {EDIT, MOVE, SELECT};



class UniverseScene : public QWidget  {
public:
  UniverseScene(QWidget *parent, Universe *universe,
                UniverseType type);


  //// INTERACTION ///
  // toggle play/pause
  void play_pause();
  bool get_state_simulation();

  // Lance un seul step
  void step();

  // Augmente ou diminue la vitesse (à haute vitesse, cela augmente les
  // pas, a basse vitesse, cela fait 1 par seconde, comme golly)
  void increase_speed();
  void decrease_speed();

  // zoom centré en avant ou en arriere
  void zoom_in(QPoint origin);
  void zoom_out(QPoint origin);

  void zoom_in();
  void zoom_out();


  // deplacement :
  void left();
  void right();
  void down();
  void up();

  void move_camera(QPointF vector);

  // Recentre la render area sur la zone interessante
  void fit_pattern();

  // toggle, change le mode (0:dessin, 1:mouvement, 2:select)
  void next_mode();
  void set_mode(SceneMode mode);
  SceneMode get_mode();


  //// GETTEUR/SETTEUR DES PARAMETRES ////
  // soit l'affichage de la grille est finie,
  //    et on voit les bords de l'univers
  // soit l'affichage de la grille est infinie,
  //    et on ne voit plus les bords de l'univers
  void toggle_bord();
  bool get_infinite_grid();

  // get/set les couleurs
  void set_cell_color(CellState state, QColor color);
  QColor get_cell_color(CellState state);
  void set_grid_color(QColor color);
  QColor get_grid_color();


  // precise à partir de quelle taille de cellule on veut la grille
  // (genre au dessus de 10 pixel/cell)
  void up_rank_grid();
  void down_rank_grid();
  int get_rank_grid();
  void set_rank_grid(int rank);


  //// GETTEUR DES INFOS DE UNIVERSE ////

  QString get_generation();
  QString get_speed();


  //// METHODE POUR LA ZONE SELECT ////
  // renvoi un nouvel univers, contenant uniquement la zone selectionnee
  // Utile pour, plus tard, enregistrer les fichiers
  Universe* get_zone();
  // supprime le contenu de la zone
  void remove_in_zone();
  // supprime tout ce qui est autour de la zone
  void remove_out_zone();

  void updateStatusBar();
  void set_cell(Vec2 coord, CellState state);
  Vec2 map_coords(QPoint mouse);

protected:
  void resizeEvent(QResizeEvent *event);

private:
  // Etat de la simulation
  bool isSimulationRun;
  // 3 modes pour la souris, edit, move, select
  SceneMode mode;
  // La zone de dessin
  RenderArea *r_area;
  // l'univers
  Universe *universe;
  // le type de l'univers
  UniverseType univ_type;
  QTimer *stepTimer;



  /// speed :
  // 2 cas :
  // -  Step = 0 (donc les saut sont de 2**Step = 1), et dans ce cas,
  //    c'est la valeur de Temp qui compte
  // -  Temp = 0 (donc il y a le minimum de temps par step) et dans ce
  //    cas, c'est la valeur de step qui compte
  /// principe :
  // On augmente temp si on veut ralentir, et on le diminue pour accelerer,
  // et si, il est egale à zero, on y touche pas, et on modifie step
  // On augmente step si on veut accelere, et on le diminue pour ralentir,
  // et si, il est egale à zero, on y touche pas, et on modifie temp

  int refresh_time_ms;

  // puissance de 2 du saut
  // 2**p_step = taille du saut (16 generations)
  int p_step;

  // affichage fini ou non de la grille
  bool bords;
  // couleurs
  QColor colors[16];
  QColor color_grid;
  // rang a partir du quel on affiche la grille
  int rank_grid;

  // La zone selectionné
  Rect select_zone;

  QMainWindow *_parent;
  QSettings settings;
};




#endif /* UI_UNIVERSESCENE_H_ */
