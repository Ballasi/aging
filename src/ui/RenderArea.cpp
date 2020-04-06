#include <QSettings>
#include <QSurfaceFormat>
#include <iostream>
#include <logic/HashlifeUniverse/HashlifeUniverse.h>
#include <logic/Rect.h>
#include <ui/RenderArea.h>
#include <vector>
#include <logic/NaiveUniverse/NaiveUniverse.h>
#include <cmath>

RenderArea::RenderArea(QWidget *parent,
    Universe *hashlife_universe, UniverseType type)
    : QOpenGLWidget(parent), hashlife_universe(hashlife_universe), type(type) {}

void RenderArea::initializeGL() {
  QSettings settings("aging-team", "aging");
  settings.sync();
  QColor bg_color = settings.value("renderer/bg-color").value<QColor>();
  QColor c_color = settings.value("renderer/cell-color").value<QColor>();

  set_colors(c_color, bg_color);

  this->vertexShaderSource =
      "attribute vec2 posAttr;\n"
      "uniform mat4 model;\n"
      "uniform mat4 view;\n"
      "uniform mat4 projection;\n"
      "void main() {\n"
      "  gl_Position = projection * view * model * vec4(posAttr,1,1);\n"
      "}\n";

  this->fragmentShaderSource = "uniform vec3 color;"
                               "void main() {\n"
                               "  gl_FragColor = vec4(color,1.0f);\n"
                               "}\n";

  initializeOpenGLFunctions();
  glDepthMask(0);

  m_program = new QOpenGLShaderProgram(this);
  m_program->addShaderFromSourceCode(QOpenGLShader::Vertex,
                                     vertexShaderSource.c_str());
  m_program->addShaderFromSourceCode(QOpenGLShader::Fragment,
                                     fragmentShaderSource.c_str());
  m_program->link();
  m_posAttr = m_program->attributeLocation("posAttr");
  m_modelUniform = m_program->uniformLocation("model");
  m_viewUniform = m_program->uniformLocation("view");
  m_projectionUniform = m_program->uniformLocation("projection");
  m_colorUniform = m_program->uniformLocation("color");

  glGenBuffers(1, &square_ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, square_ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(square_elements),
               square_elements, GL_STATIC_DRAW);

  float aspect_ratio =
      static_cast<float>(width()) / static_cast<float>(height());

  // We create the projection matrix
  projectionMatrix.setToIdentity();
  projectionMatrix.ortho(QRectF(0.0f, 0.0f, 1.0f * aspect_ratio, 1.0f));

  // The Camera holds the view matrix and handles zooming
  camera = new Camera2D();

  switch (type) {
    size_t zoom_level;
    case UniverseType::Life :
      zoom_level = static_cast<size_t>(ceil(log2(
        reinterpret_cast<NaiveUniverse*>
                    (hashlife_universe)->width)));
      camera->set_zoom(1 << zoom_level);
      camera->pos.setX(-0.25f);
      camera->pos.setY(-0.75f);
      break;
    case UniverseType::Hashlife :
      zoom_level = static_cast<size_t>(reinterpret_cast<HashlifeUniverse *>
                    (hashlife_universe)->get_top_level() + 1);
      camera->set_zoom(1 << zoom_level);
      camera->pos.setX(-0.25f);
      camera->pos.setY(-0.75f);
      break;
  }
}

void RenderArea::resizeGL(int w, int h) {
  const qreal retinaScale = devicePixelRatio();
  glViewport(0, 0, width(), height() * retinaScale);

  float aspect_ratio =
      static_cast<float>(width()) / static_cast<float>(height());
  projectionMatrix.setToIdentity();
  projectionMatrix.ortho(QRectF(0.0f, 0.0f, 1.0f * aspect_ratio, 1.0f));

  update();
}

void RenderArea::paintGL() {
  const qreal retinaScale = devicePixelRatio();
  glViewport(0, 0, width() * retinaScale, height() * retinaScale);

  glClearColor(bg_r, bg_g, bg_b, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  m_program->bind();
  m_program->setUniformValue(m_colorUniform, cell_color);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(m_posAttr, 2, GL_FLOAT, GL_FALSE, 0, square_vertices);

  QMatrix4x4 viewMatrix = camera->get_view();

  switch (type) {
    case UniverseType::Hashlife :
      render_hashlife(viewMatrix);
      break;
    case UniverseType::Life :
      render_gol(viewMatrix);
      break;
  }

  glDisableVertexAttribArray(0);
  m_program->release();
}

void RenderArea::render_gol(const QMatrix4x4 &viewMatrix) {
  QMatrix4x4 modelMatrix;
  modelMatrix.setToIdentity();

  m_program->setUniformValue(m_viewUniform, viewMatrix);
  m_program->setUniformValue(m_projectionUniform, projectionMatrix);

  NaiveUniverse *univ = reinterpret_cast<NaiveUniverse *>(hashlife_universe);

  for (size_t i = 0; i < univ->width; ++i) {
    for (size_t j = 0; j < univ->height; ++j) {
      modelMatrix.setToIdentity();
      if (univ->get(Coord(i, j))) {
        modelMatrix.translate(i, -static_cast<int>(j), 0);
        m_program->setUniformValue(m_modelUniform, modelMatrix);
        glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, 0);
      }
    }
  }
}

void RenderArea::render_hashlife(const QMatrix4x4 &viewMatrix) {
  Coord top_left =
      (static_cast<HashlifeUniverse *>(hashlife_universe))->get_top_left();
  size_t level =
      (static_cast<HashlifeUniverse *>(hashlife_universe))->get_top_level();

  Rect bounds = camera->get_view_bounds(static_cast<float>(width()) /
                                            static_cast<float>(height()),
                                        hashlife_universe);

  std::vector<Coord> coords;

  static_cast<HashlifeUniverse *>(hashlife_universe)
      ->get_cell_in_bounds(bounds, &coords);

  QMatrix4x4 modelMatrix;
  modelMatrix.setToIdentity();

  m_program->setUniformValue(m_viewUniform, viewMatrix);
  m_program->setUniformValue(m_projectionUniform, projectionMatrix);

  /*
  std::cout << "Top left : " << bounds.top_left.x << ',' << bounds.top_left.y
            << '\n';
  std::cout << "Bottom right : " << bounds.bottom_right.x << ','
            << bounds.bottom_right.y << '\n';
  */

  for (size_t i = 0; i < coords.size(); ++i) {
    modelMatrix.setToIdentity();
    modelMatrix.translate(coords[i].x.get_si(), -coords[i].y.get_si(), 0);
    m_program->setUniformValue(m_modelUniform, modelMatrix);
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, 0);
  }
}

void RenderArea::wheelEvent(QWheelEvent *event) {
  // camera->set_zoom(camera->get_zoom() - event->delta() / 100.0f);
  // update();
}
void RenderArea::zoomin_event(QPoint origin) {
  Coord c = map_coords_from_mouse(origin);
  camera->set_zoom(camera->get_zoom() / 2);
  camera->look_at(c);
  update();
}
void RenderArea::zoomout_event(QPoint origin) {
  Coord c = map_coords_from_mouse(origin);
  camera->set_zoom(camera->get_zoom() * 2);
  camera->look_at(c);
  update();
}

Coord RenderArea::map_coords_from_mouse(QPoint mouseCoords) {
  float aspect_ratio =
      static_cast<float>(width()) / static_cast<float>(height());
  float x_relative = static_cast<float>(mouseCoords.x()) /
                     static_cast<float>(width()) * aspect_ratio;
  float y_relative =
      static_cast<float>(mouseCoords.y()) / static_cast<float>(height());

  QMatrix4x4 viewinv = camera->get_view().inverted();
  Coord c;
  QPointF p = viewinv.map(QPointF(x_relative, y_relative));
  c.x = BigInt(std::floor(p.x()));
  c.y = BigInt(std::ceil(-p.y()));

  return c;
}

void RenderArea::handleInput(QKeyEvent *event) {
  switch (event->key()) {
  case Qt::Key_Z:
  case Qt::Key_Up:
    camera->pos.setY(camera->pos.y() - 0.05f);
    break;
  case Qt::Key_S:
  case Qt::Key_Down:
    camera->pos.setY(camera->pos.y() + 0.05f);
    break;
  case Qt::Key_Q:
  case Qt::Key_Left:
    camera->pos.setX(camera->pos.x() - 0.05f);
    break;
  case Qt::Key_D:
  case Qt::Key_Right:
    camera->pos.setX(camera->pos.x() + 0.05f);
    break;
  case Qt::Key_Plus:
    zoomin_event(QPoint(width() / 2, height() / 2));
    break;
  case Qt::Key_Minus:
    zoomout_event(QPoint(width() / 2, height() / 2));
    break;

  default:
    break;
  }
  update();
}

void RenderArea::set_colors(QColor c_color, QColor bg_color) {
  bg_r = bg_color.redF();
  bg_g = bg_color.greenF();
  bg_b = bg_color.blueF();

  cell_color.setX(c_color.redF());
  cell_color.setY(c_color.greenF());
  cell_color.setZ(c_color.blueF());
}
