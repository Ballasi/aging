#ifndef UI_UNIVERSESCENE_H_
#define UI_UNIVERSESCENE_H_



#include <ui/RenderArea.h>
#include <logic/UniverseType.h>




class UniverseScene :  protected RenderArea {
public:
  UniverseScene(QWidget *parent = 0, Universe *hashlife_universe = 0,
                UniverseType type = UniverseType::Hashlife);


  //// INTERACTION ///
  // toggle play/pause
  void play_pause();
  
  // Lance un seul step
  void one_step();
  
  // Augmente ou diminue la vitesse (à haute vitesse, cela augmente les pas, a basse vitesse, cela fait 1 par seconde, comme golly)
  void increase_speed();
  void decrease_speed();

  // zoom centré en avant ou en arriere
  void zoom_in();
  void zoom_out();

  // Recentre la render area sur la zone interessante
  void fit_pattern();
  
  // toggle, change le mode (0:dessin, 1:mouvement, 2:select)
  void set_mode(int mode);


  //// GETTEUR/SETTEUR DES PARAMETRES ////
  // soit l'affichage de la grille est finie, et on voit les bords de l'univers
  // soit l'affichafe de la grille est infinie, et on ne voit plus les bords de l'univers
  void toggle_bord();
  // get/set les couleurs
  void set_color_dead(QColor color);
  QColor get_color_dead();
  void set_color_alive(QColor color);
  QColor get_color_alive();
  void set_color_grid(QColor color);
  QColor get_color_grid();
  
  // precise à partir de quelle taille de cellule on veut la grille (genre au dessus de 10 pixel/cell)
  void set_rank_grid(int size_cell);


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


private:
  // 3 modes pour la souris, edit, move, select
  int Mode_is_move;
  // La zone de dessin 
  RenderArea *r_area;
  // l'univers
  Universe *universe;
  // le type de l'univers
  UniverseType univ_type;



  /// speed :
  // 2 cas : 
  // -  Step = 0 (donc les saut sont de 2**Step = 1), et dans ce cas, c'est la valeur de Temp qui compte
  // -  Temp = 0 (donc il y a le minimum de temps par step) et dans ce cas, c'est la valeur de step qui compte
  /// principe :
  // On augmente temp si on veut ralentir, et on le diminue pour accelerer, et si, il est egale à zero, on y touche pas, et on modifie step
  // On augmente step si on veut accelere, et on le diminue pour ralentir, et si, il est egale à zero, on y touche pas, et on modifie temp
  
  // temps pour faire 1/4 de pas :
  // 1 -> 0.25 sec par pas
  // 2 -> 0.5 sec par pas
  // 3 -> 0.75 sec par pas
  // 4 -> 1 sec par pas
  // 5 -> 1.25 sec par pas
  int temp_by_for_step;
  
  // puissance de 2 du saut
  // 2**p_step = taille du saut (16 generations) 
  size_t p_step;

  // affichage fini ou non de la grille
  int bords
  // couleurs 
  QColor color_dead;
  QColor color_alive;
  QColor color_grid;
  // rang a partir du quel on affiche la grille
  int rank_grid;

  // La zone selectionné
  Rect select_zone;
  
}




#endif /* UI_UNIVERSESCENE_H_ */