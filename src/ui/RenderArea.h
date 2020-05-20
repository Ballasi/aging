#ifndef UI_RENDERAREA_H_
#define UI_RENDERAREA_H_

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLWidget>
#include <QWheelEvent>
#include <string>
#include <ui/Camera2D.h>
#include <model/Rect.h>
#include <model/Universe.h>
#include <ui/Selection.h>

class RenderArea : public QOpenGLWidget, protected QOpenGLFunctions {
public:
  explicit RenderArea(QWidget *parent = 0,
                      Universe *hashlife_universe = 0,
                      UniverseType type = UniverseType::Hashlife,
                      Selection *selection = 0);


  void left();
  void right();
  void down();
  void up();

  void move_camera(QPointF vector);


  void zoomin_event(QPoint origin);
  void zoomout_event(QPoint origin);
  void zoomin();
  void zoomout();

  Vec2 map_coords_from_mouse(QPoint mouseVec2s);
  void set_colors(QColor cell_color, QColor bg_color);

  void fitPattern();

protected:
  void initializeGL() override;
  void resizeGL(int w, int h) override;
  void paintGL() override;

private:
  Universe *hashlife_universe;
  std::string vertexShaderSource;
  std::string fragmentShaderSource;

  GLuint m_posAttr;
  GLuint m_modelUniform;
  GLuint m_viewUniform;
  GLuint m_projectionUniform;
  GLuint m_colorUniform;

  GLclampf bg_r, bg_g, bg_b;
  QVector3D cell_color;

  QMatrix4x4 projectionMatrix;

  float square_vertices[8] = {
      0.0f, 0.0f, // Top-left
      1.0f, 0.0f, // Top-right
      1.0f, 1.0f, // Bottom-right
      0.0f, 1.0f, // Bottom-left
  };

  GLuint square_elements[4] = {
      0,
      1,
      3,
      2,
  };
  GLuint line_elements[4] = {
      0,
      1,
      2,
      3,
  };

  GLuint square_ebo;
  GLuint line_ebo;

  QOpenGLShaderProgram *m_program;
  Camera2D *camera;

  UniverseType type;
  Selection *selection;

  void render_gol(const QMatrix4x4 &matrix);
  void render_hashlife(const QMatrix4x4 &matrix);
};

#endif /* UI_RENDERAREA_H_ */
