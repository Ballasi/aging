#include "Camera2D.h"
#include <iostream>

Camera2D::Camera2D(void)
:zoom(1), pos(50.0f,50.0f), origin(0.0f,0.0f), aspect_ratio(1.0f)
{
    transform.update(pos,origin,zoom,aspect_ratio);

    generate_transformation_matrix(100.0f,100.0f);
}

void Camera2D::set_zoom(const int n_zoom) {
    zoom = n_zoom;
    zoom = zoom < 1.0f ? 1.0f : zoom;
}
int Camera2D::get_zoom() const {
    return zoom;
}

void::Camera2D::set_aspect_ratio(const float aspect) {
    aspect_ratio = aspect;
}

QMatrix4x4 Camera2D::get_transformation(float width, float height) {

    bool dirty = false;

    if (transform.last_pos != pos) { dirty = true; }
    else if (transform.last_origin != origin) { dirty = true; }
    else if (transform.last_zoom != zoom) { dirty = true; }
    else if (transform.last_aspect_ratio != aspect_ratio) { dirty = true; }
    if (dirty) generate_transformation_matrix(width, height);
    return transform.matrix;
}

void Camera2D::Transformation::update(const QVector2D& p, const QVector2D& origin, const int zoom, const float aspect_ratio) {
    last_pos = p;
    last_zoom = zoom;
    last_origin = origin;
    last_aspect_ratio = aspect_ratio;
}

void Camera2D::generate_transformation_matrix(float width, float height) {

    aspect_ratio = width / height;
    transform.update(pos,origin,zoom,aspect_ratio);

    transform.matrix.setToIdentity();
    transform.matrix.ortho(QRectF(0.0f,0.0f,1.0f * aspect_ratio,1.0f));

    transform.matrix.translate(-pos.x(),pos.y(),0);
    transform.matrix.scale(1.0f /zoom,1.0f /zoom,1);
    transform.matrix.translate(origin.x(),origin.y(),0);

    std::cout << "Matrix : " << "\n";
    for(int i = 0 ; i < 4 ; ++i){
        std::cout << "|";
        for(int j = 0 ; j < 4 ; ++j){
            std::cout << transform.matrix.column(j)[i];
            std::cout << "|";
        }
        std::cout << '\n';
    }
}