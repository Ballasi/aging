#include <cmath>
#include <iostream>
#include <ui/Camera2D.h>

Camera2D::Camera2D(void) : zoom(1), pos(0.0f, 0.0f), origin(0.0f, 0.0f) {
  transform.update(pos, origin, zoom);

  generate_view_matrix();
}

void Camera2D::set_zoom(const int n_zoom) {
  zoom = n_zoom;
  zoom = zoom < 1.0f ? 1.0f : zoom;

  transform.update(pos, origin, zoom);
  generate_view_matrix();
}

int Camera2D::get_zoom() const { return zoom; }

QMatrix4x4 Camera2D::get_view() {
  bool dirty = false;

  if (transform.last_pos != pos) {
    dirty = true;
  } else if (transform.last_origin != origin) {
    dirty = true;
  } else if (transform.last_zoom != zoom) {
    dirty = true;
  }

  if (dirty)
    generate_view_matrix();

  return transform.matrix;
}

void Camera2D::Transformation::update(const QVector2D &p,
                                      const QVector2D &origin, const int zoom) {
  last_pos = p;
  last_zoom = zoom;
  last_origin = origin;
}

void Camera2D::generate_view_matrix() {
  transform.update(pos, origin, zoom);

  transform.matrix.setToIdentity();
  transform.matrix.translate(-pos.x(), -pos.y(), 0);
  transform.matrix.scale(1.0f / zoom, 1.0f / zoom, 1);
  transform.matrix.translate(origin.x(), origin.y(), 0);
}

Rect Camera2D::get_view_bounds(float aspect_ratio, Universe *universe) {
  bool invertible;
  QMatrix4x4 inv = get_view().inverted(&invertible);
  if (!invertible)
    std::cout << "Non-invertible view matrix" << '\n';

  QRectF rectf = inv.mapRect(
      QRectF(QPointF(0.0f, 0.0f), QPointF(1.0f * aspect_ratio, 1.0f)));

  Coord top_left =
      Coord(BigInt(static_cast<int>(std::ceil(rectf.topLeft().x())) - 1),
            BigInt(static_cast<int>(std::ceil(rectf.bottomRight().y()))));
  Coord bottom_right =
      Coord(BigInt(static_cast<int>(std::ceil(rectf.bottomRight().x()))),
            BigInt(static_cast<int>(std::ceil(rectf.topLeft().y()))) - 1);

  BigInt temp = top_left.y;
  top_left.y = bottom_right.y;
  bottom_right.y = temp;

  Rect rect(top_left, bottom_right);
  return rect;
}

void Camera2D::look_at(Coord c) {
  pos.setX((static_cast<float>(c.x.get_si()) / static_cast<float>(zoom))
            - 0.5f + 0.5f / static_cast<float>(zoom));
  pos.setY((static_cast<float>(c.y.get_si()) / static_cast<float>(zoom))
            - 0.5f + 0.5f / static_cast<float>(zoom));

  transform.update(pos, origin, zoom);
  generate_view_matrix();
}
