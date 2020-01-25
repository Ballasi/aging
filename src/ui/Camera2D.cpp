#include "Camera2D.h"

Camera2D::Camera2D(void)
:zoom(1), pos(0.0f,0.0f), origin(0.0f,0.0f)
{
    transform.update(pos,origin,zoom);
    generate_transformation_matrix();
}

void Camera2D::set_zoom(const int n_zoom) {
    zoom = n_zoom;
    zoom = zoom < 1.0f ? 1.0f : zoom;
}
int Camera2D::get_zoom() const {
    return zoom;
}

QMatrix4x4 Camera2D::get_transformation() {

    bool dirty = false;

    if (transform.last_pos != pos) { dirty = true; }
    else if (transform.last_origin != origin) { dirty = true; }
    else if (transform.last_zoom != zoom) { dirty = true; }
    if (dirty) generate_transformation_matrix();
    return transform.matrix;
}

void Camera2D::Transformation::update(const QVector2D& p, const QVector2D& origin, const int zoom) {
    last_pos = p;
    last_zoom = zoom;
    last_origin = origin;
}

void Camera2D::generate_transformation_matrix() {

    transform.update(pos,origin,zoom);

    transform.matrix.setToIdentity();

    transform.matrix.translate(-pos.x(),-pos.y(),0);
    transform.matrix.scale(1.0f /zoom,1.0f /zoom,1);
    transform.matrix.translate(origin.x(),origin.y(),0);
}