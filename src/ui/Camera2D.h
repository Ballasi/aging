#ifndef UI_CAMERA2D_H_
#define UI_CAMERA2D_H_

#include <model/Rect.h>
#include <model/Universe.h>
#include <model/Vec2.h>
#include <qmatrix4x4.h>
#include <qvector2d.h>

class Camera2D {
public:
  Camera2D(void);
  void set_zoom(const int n_zoom);
  int get_zoom() const;
  QMatrix4x4 get_view();

  Rect get_view_bounds(float aspect_ratio, Universe *universe);
  QVector2D pos;
  QVector2D origin;

  void look_at(Vec2 c);

protected:
  void generate_view_matrix();

  struct Transformation {
    QMatrix4x4 matrix;
    QVector2D last_pos;
    int last_zoom;
    QVector2D last_origin;
    void update(const QVector2D &p, const QVector2D &origin, const int zoom);
  };

  int zoom;
  Transformation transform;
};

#endif /* UI_CAMERA2D_H_ */
