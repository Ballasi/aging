#include "Camera2D.h"
#include <iostream>
#include <cmath>

Camera2D::Camera2D(void)
:zoom(1), pos(0.0f,0.0f), origin(0.0f,0.0f)
{
    transform.update(pos,origin,zoom);

    generate_view_matrix();
}

void Camera2D::set_zoom(const int n_zoom) {
    zoom = n_zoom;
    zoom = zoom < 1.0f ? 1.0f : zoom;
}
int Camera2D::get_zoom() const {
    return zoom;
}

QMatrix4x4 Camera2D::get_view() {

    bool dirty = false;

    if (transform.last_pos != pos) { dirty = true; }
    else if (transform.last_origin != origin) { dirty = true; }
    else if (transform.last_zoom != zoom) { dirty = true; }
    if (dirty) generate_view_matrix();
    return transform.matrix;
}

void Camera2D::Transformation::update(const QVector2D& p, const QVector2D& origin, const int zoom) {
    last_pos = p;
    last_zoom = zoom;
    last_origin = origin;
}

void Camera2D::generate_view_matrix() {

    transform.update(pos,origin,zoom);

    transform.matrix.setToIdentity();
    transform.matrix.translate(-pos.x(),-pos.y(),0);
    transform.matrix.scale(1.0f/zoom, 1.0f/zoom, 1);
    transform.matrix.translate(origin.x(),origin.y(),0);

}

Rect Camera2D::get_view_bounds(float aspect_ratio, Universe *universe) {

    bool invertible;
    QMatrix4x4 inv = get_view().inverted(&invertible);
    if(!invertible)
        std::cout << "Non-invertible view matrix" << '\n';
    
    QRectF rectf = inv.mapRect(QRectF(QPointF(0.0f,0.0f),QPointF(1.0f * aspect_ratio,1.0f)));

    Coord top_left = Coord((BigInt) std::ceil(rectf.topLeft().x()), (BigInt) std::ceil(rectf.topLeft().y()));
    Coord bottom_right = Coord((BigInt) std::ceil(rectf.bottomRight().x()), (BigInt) std::ceil(rectf.bottomRight().y()));

    //top_left.y = (1 << universe->get_top_level()) - top_left.y;
    //bottom_right.y = (1 << universe->get_top_level()) - bottom_right.y;

    BigInt diff_y = bottom_right.y - top_left.y;

    top_left.y *= -1;
    bottom_right.y = top_left.y - diff_y;

    Rect rect(top_left,bottom_right);
    return rect;
}
