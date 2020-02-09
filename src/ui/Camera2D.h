#ifndef UI_CAMERA2D_H_
#define UI_CAMERA2D_H_

#include <qmatrix4x4.h>
#include <qvector2d.h>

class Camera2D {

public:
    Camera2D(void);
    void set_zoom(const int n_zoom);
    int get_zoom() const;
    void set_aspect_ratio(const float aspect);
    float get_aspect_ratio() const;
    QMatrix4x4 get_transformation(float width, float height);

    QVector2D pos;
    QVector2D origin;
    

protected:
    void generate_transformation_matrix(float width, float height);

    struct Transformation
    {
        QMatrix4x4 matrix;
        QVector2D last_pos;
        int last_zoom;
        float last_aspect_ratio;
        QVector2D last_origin;
        void update(const QVector2D& p, const QVector2D& origin, const int zoom, const float aspect);
    };

    int zoom;
    float aspect_ratio;
    Transformation transform;
    

};


#endif /* UI_CAMERA2D_H_ */
