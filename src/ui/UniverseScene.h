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
#include <ui/Selection.h>

enum SceneMode {EDIT, MOVE, SELECT};

class UniverseScene : public QWidget {
/*! \class UniverseScene
   * \brief a class that displays an univers
   *
   * The goal of this class is to display a given universe, and to provide
   * methods to easily manipulate the cellular automaton.
   */
public:
  /*!
   *  \brief Constructor
   *
   *  Constructor of UniverseScene
   *
   *  \param parent : Parent Widget
   *  \param universe : desired universe to be displayed
   *  \param type : type of universe
   */
  UniverseScene(QWidget *parent, Universe *universe,
                UniverseType type);


  /*!
   *  \brief method for changing the state of the simulation
   *
   *  This method allows to change the state of the simulation
   * (Play <-> Pause).
   */
  void play_pause();

  /*!
   *  \brief method for getting the state of the simulation
   *
   *  This method allows to get the state of the simulation
   */
  bool get_state_simulation();

  /*!
   *  \brief method of starting a single step
   *
   *  This method executes a single step of the simulation
   */
  void step();

  /*!
   *  \brief Can Increases Speed
   *
   *  Enables you to ask if you can change the speed of the simulation.
   */
  bool can_increase_speed();
  /*!
   *  \brief increases speed
   *
   *  This method increases the speed of the simulation
   */
  void increase_speed();
  /*!
   *  \brief Can Decreases Speed
   *
   *  Enables you to ask if you can change the speed of the simulation.
   */
  bool can_decrease_speed();
  /*!
   *  \brief decreases speed
   *
   *  This method decreases the speed of the simulation
   */
  void decrease_speed();

  /*!
   *  \brief Enables you to ask if you can activate a hyperSpeed mode.
   *
   *  On some universes, there is a hyperspeed mode, which will always
   *  make as many jumps as possible, in order to calculate as quickly as
   *  possible, a maximum of generation
   */
  bool can_hyperSpeed();
  /*!
   *  \brief getter for hyperSpeed mode
   *
   *  This method is used to determine whether the Hyperspeed mode is
   *  activated or not.
   */
  bool state_hyperSpeed();
  /*!
   *  \brief setter for hyperSpeed mode
   *
   *  This method allows you to change the Hyperspeed mode
   *  (Play <-> False)
   */
  void toggle_hyperSpeed();

  /*!
   *  \brief zooms in
   *
   *  Zooms in, centered on the point given in the parameters.
   */
  void zoom_in(QPoint origin);
  /*!
   *  \brief zooms back
   *
   *  Zooms back, centered on the point given in the parameters.
   */
  void zoom_out(QPoint origin);
  /*!
   *  \brief zooms in
   *
   *  Zooms in, centered on the center of the window.
   */
  void zoom_in();
  /*!
   *  \brief zooms back
   *
   *  Zooms back, centered on the center of the window.
   */
  void zoom_out();


  /*!
   *  \brief move the camera to the left
   *
   *  moves the visible area of the universe in the desired direction.
   */
  void left();
  /*!
   *  \brief move the camera to the right
   *
   *  moves the visible area of the universe in the desired direction.
   */
  void right();
  /*!
   *  \brief move the camera down
   *
   *  moves the visible area of the universe in the desired direction.
   */
  void down();
  /*!
   *  \brief move the camera up
   *
   *  moves the visible area of the universe in the desired direction.
   */
  void up();
  /*!
   *  \brief move the camera
   *
   *  moves the camera to center the point
   */
  void move_camera(QPointF vector);

  /*!
   *  \brief fit pattern
   *
   *  Refocus the camera on the area of interest.
   */
  void fit_pattern();

  /*!
   *  \brief changes the mode of mouse interaction
   *
   * if the mouse was in move mode, it switches to edit mode.
   * if the mouse was in edit mode, it switches to selection mode.
   * if the mouse was in Selection mode, it switches to move mode.
   */
  void next_mode();

  /*!
   *  \brief changes the mode of mouse interaction
   *
   *  Set the mouse mode in {edit, selection, move} mouse mode.
   */
  void set_mode(SceneMode mode);
  /*!
   *  \brief get the mode of mouse interaction
   *
   *  Get the mouse mode (in {edit, selection, move})
   */
  SceneMode get_mode();

  ////////////////////// TRISTAN //////////////////////
  ////////////////////// TRISTAN //////////////////////
  ////////////////////// TRISTAN //////////////////////
  Selection get_selection();
  void paste_selection();
  void copy_selection();
  void reset_selection();
  void update_selection(QPoint mouse);

  //// METHODE POUR LA ZONE SELECT ////
  // renvoi un nouvel univers, contenant uniquement la zone selectionnee
  // Utile pour, plus tard, enregistrer les fichiers
  Universe* get_zone();
  // supprime le contenu de la zone
  void remove_in_zone();
  // supprime tout ce qui est autour de la zone
  void remove_out_zone();
  ////////////////////// TRISTAN //////////////////////
  ////////////////////// TRISTAN //////////////////////
  ////////////////////// TRISTAN //////////////////////


  /*!
   *  \brief set the color of the given state
   */
  void set_cell_color(CellState state, QColor color);
  /*!
   *  \brief get the color of the given state
   */
  QColor get_cell_color(CellState state);
  /*!
   *  \brief set the color of the grid
   */
  void set_grid_color(QColor color);
  /*!
   *  \brief set the color of the grid
   */
  QColor get_grid_color();

  /*!
   *  \brief get a number of generation
   */
  QString get_generation();

  /*!
   *  \brief get a speed of simulation
   */
  QString get_speed();


  /*!
   *  \brief refresh the status bar
   */
  void updateStatusBar();
  /*!
   *  \brief set one cell
   *
   * set the state of the specified cell
   *
   */
  void set_cell(Vec2 coord, CellState state);
  /*!
   *  \brief convert Qpoint to Vec2
   *
   * convert a QPoint in a Vec2
   *
   */
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

  bool hyperspeed_state;

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

  // La zone selectionné
  Rect select_zone;

  QMainWindow *_parent;
  QSettings settings;
  Selection selection;
};




#endif /* UI_UNIVERSESCENE_H_ */
