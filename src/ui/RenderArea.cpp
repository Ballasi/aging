#include <QSettings>
#include <QSurfaceFormat>
#include <cmath>
#include <iostream>
#include <model/Rect.h>
#include <model/Universe.h>
#include <ui/RenderArea.h>
#include <ui/UniverseScene.h>
#include <universes/hash/HashUniverse.h>
#include <universes/life/LifeUniverse.h>
#include <utility>
#include <vector>

RenderArea::RenderArea(QWidget *parent, Universe *hashlife_universe,
                       UniverseType type, Selection *selection)
    : QOpenGLWidget(parent), hashlife_universe(hashlife_universe), type(type),
      selection(selection) {}

void RenderArea::initializeGL() {
  QColor bg_color = QColor("#000000");
  QColor c_color = QColor("#ffffff");

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
                               "  gl_FragColor = vec4(color,1.0);\n"
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
  glGenBuffers(1, &line_ebo);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, square_ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(square_elements),
               square_elements, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, line_ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(line_elements), line_elements,
               GL_STATIC_DRAW);

  float aspect_ratio =
      static_cast<float>(width()) / static_cast<float>(height());

  // We create the projection matrix
  projectionMatrix.setToIdentity();
  projectionMatrix.ortho(QRectF(0.0f, 0.0f, 1.0f * aspect_ratio, 1.0f));

  // The Camera holds the view matrix and handles zooming
  camera = new Camera2D();

  switch (type) {
    size_t zoom_level;
  case UniverseType::Life:
    zoom_level = static_cast<size_t>(
        ceil(log2(reinterpret_cast<LifeUniverse *>(hashlife_universe)
                      ->bounds()
                      .width()
                      .get_si())));
    camera->set_zoom(1 << zoom_level);
    camera->pos.setX(-0.25f);
    camera->pos.setY(-0.25f);
    break;
  case UniverseType::Hashlife:
    zoom_level = static_cast<size_t>(
        reinterpret_cast<HashUniverse *>(hashlife_universe)->get_top_level() +
        1);
    camera->set_zoom(1 << zoom_level);
    camera->pos.setX(-0.25f);
    camera->pos.setY(-0.25f);
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
  case UniverseType::Hashlife:
    render_hashlife(viewMatrix);
    break;
  case UniverseType::Life:
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

  LifeUniverse *univ = reinterpret_cast<LifeUniverse *>(hashlife_universe);

  Rect bounds = camera->get_view_bounds(
      static_cast<float>(width()) / static_cast<float>(height()), univ);

  bounds.top_left.x = (bounds.top_left.x < 0) ? 0 : bounds.top_left.x;
  bounds.top_left.y = (bounds.top_left.y < 0) ? 0 : bounds.top_left.y;

  bounds.size.x = (bounds.bottom_right().x > univ->bounds().width() - 1)
                      ? univ->bounds().width() - bounds.top_left.x
                      : bounds.size.x;

  bounds.size.y = (bounds.bottom_right().y > univ->bounds().height() - 1)
                      ? univ->bounds().height() - bounds.top_left.y
                      : bounds.size.y;

  std::cout << "Top left : (" << bounds.top_left.x << ',' << bounds.top_left.y
            << ")" << std::endl;
  std::cout << "Bottom right : (" << bounds.bottom_right().x << ','
            << bounds.bottom_right().y << ")" << std::endl;

  float width_f = static_cast<float>(univ->bounds().width().get_si()) - 1;
  float height_f = static_cast<float>(univ->bounds().height().get_si()) - 1;

  float border_vertices[8] = {
      0.0f,    0.0f,     // Top-left
      width_f, 0.0f,     // Top-right
      width_f, height_f, // Bottom-right
      0.0f,    height_f, // Bottom-left
  };

  modelMatrix.setToIdentity();
  m_program->setUniformValue(m_modelUniform, modelMatrix);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, line_ebo);
  glVertexAttribPointer(m_posAttr, 2, GL_FLOAT, GL_FALSE, 0, border_vertices);
  glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_INT, 0);

  glVertexAttribPointer(m_posAttr, 2, GL_FLOAT, GL_FALSE, 0, square_vertices);

  if (camera->get_zoom() <= 32) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, line_ebo);
    for (BigInt i = bounds.top_left.x; i < bounds.bottom_right().x; ++i) {
      for (BigInt j = bounds.top_left.y; j < bounds.bottom_right().y; ++j) {
        modelMatrix.setToIdentity();
        modelMatrix.translate(i.get_si(), j.get_si(), 0);
        m_program->setUniformValue(m_modelUniform, modelMatrix);
        glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_INT, 0);
      }
    }
  }

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, square_ebo);
  for (BigInt i = bounds.top_left.x; i < bounds.bottom_right().x; ++i) {
    for (BigInt j = bounds.top_left.y; j < bounds.bottom_right().y; ++j) {
      modelMatrix.setToIdentity();
      modelMatrix.translate(i.get_si(), j.get_si(), 0);
      m_program->setUniformValue(m_modelUniform, modelMatrix);
      if (univ->get(Vec2(i, j))) {
        glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, 0);
      }
    }
  }
}

void RenderArea::render_hashlife(const QMatrix4x4 &viewMatrix) {
  Rect univ_bounds = (static_cast<HashUniverse *>(hashlife_universe))->bounds();

  size_t level =
      (static_cast<HashUniverse *>(hashlife_universe))->get_top_level();

  Rect bounds = camera->get_view_bounds(static_cast<float>(width()) /
                                            static_cast<float>(height()),
                                        hashlife_universe);

  BigInt nb_pixels(width() * height());
  BigInt nb_cells = bounds.size.x * bounds.size.y;
  mpf_class res_ratio(nb_cells);
  res_ratio /= nb_pixels;
  BigInt ratio_mpz(res_ratio);

  int min_rendered_level = mpz_sizeinbase(ratio_mpz.get_mpz_t(), 2) >> 1;

  min_rendered_level =
      (min_rendered_level >
       static_cast<HashUniverse *>(hashlife_universe)->get_top_level())
          ? static_cast<HashUniverse *>(hashlife_universe)->get_top_level()
          : min_rendered_level;

  min_rendered_level = (min_rendered_level < 1) ? 1 : min_rendered_level;

  float size_f = static_cast<float>((1 << (min_rendered_level - 1)));

  float square_vertices2[8] = {
      0.0f,   0.0f,   // Top-left
      size_f, 0.0f,   // Top-right
      size_f, size_f, // Bottom-right
      0.0f,   size_f, // Bottom-left
  };

  glVertexAttribPointer(m_posAttr, 2, GL_FLOAT, GL_FALSE, 0, square_vertices2);

  std::vector<Vec2> coords;

  static_cast<HashUniverse *>(hashlife_universe)
      ->get_cell_in_bounds(bounds, &coords, min_rendered_level);

  QMatrix4x4 modelMatrix;
  modelMatrix.setToIdentity();

  m_program->setUniformValue(m_viewUniform, viewMatrix);
  m_program->setUniformValue(m_projectionUniform, projectionMatrix);

  bounds.top_left.x = (bounds.top_left.x < univ_bounds.top_left.x)
                          ? univ_bounds.top_left.x
                          : bounds.top_left.x;
  bounds.top_left.y = (bounds.top_left.y < univ_bounds.top_left.y)
                          ? univ_bounds.top_left.y
                          : bounds.top_left.y;

  bounds.size.x = (bounds.bottom_right().x > univ_bounds.bottom_right().x - 1)
                      ? univ_bounds.bottom_right().x - bounds.top_left.x
                      : bounds.size.x;

  bounds.size.y = (bounds.bottom_right().y > univ_bounds.bottom_right().y - 1)
                      ? univ_bounds.bottom_right().y - bounds.top_left.y
                      : bounds.size.y;

  if (camera->get_zoom() <= 32) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, line_ebo);
    for (BigInt i = bounds.top_left.x; i < bounds.bottom_right().x; ++i) {
      for (BigInt j = bounds.top_left.y; j < bounds.bottom_right().y; ++j) {
        modelMatrix.setToIdentity();
        modelMatrix.translate(i.get_si(), j.get_si(), 0);
        m_program->setUniformValue(m_modelUniform, modelMatrix);
        glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_INT, 0);
      }
    }
  }

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, square_ebo);
  for (size_t i = 0; i < coords.size(); ++i) {
    modelMatrix.setToIdentity();
    modelMatrix.translate(coords[i].x.get_si(), coords[i].y.get_si(), 0);
    m_program->setUniformValue(m_modelUniform, modelMatrix);
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, 0);
  }

  if (selection->state == SelectionState::Second) {
    Rect r({selection->first.x, selection->first.y},
           {selection->second.x - selection->first.x,
            selection->second.y - selection->first.y});

    float width_f = static_cast<float>(r.width().get_si()) + 1;
    float height_f = static_cast<float>(r.height().get_si()) + 1;
    float selection_vertices[8] = {
        0.0f,    0.0f,     // Top-left
        width_f, 0.0f,     // Top-right
        width_f, height_f, // Bottom-right
        0.0f,    height_f, // Bottom-left
    };

    modelMatrix.setToIdentity();
    modelMatrix.translate(r.top_left.x.get_si(), r.top_left.y.get_si(), 0);
    m_program->setUniformValue(m_modelUniform, modelMatrix);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, line_ebo);
    glVertexAttribPointer(m_posAttr, 2,
      GL_FLOAT, GL_FALSE, 0, selection_vertices);
    glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_INT, 0);

    glVertexAttribPointer(m_posAttr, 2, GL_FLOAT, GL_FALSE, 0, square_vertices);
  }
}

void RenderArea::zoomin_event(QPoint origin) {
  Vec2 c = map_coords_from_mouse(origin);
  camera->set_zoom(camera->get_zoom() / 2);
  camera->look_at(c);
  update();
}
void RenderArea::zoomout_event(QPoint origin) {
  Vec2 c = map_coords_from_mouse(origin);
  camera->set_zoom(camera->get_zoom() * 2);
  camera->look_at(c);
  update();
}

void RenderArea::zoomin() { zoomin_event(QPoint(width() / 2, height() / 2)); }
void RenderArea::zoomout() { zoomout_event(QPoint(width() / 2, height() / 2)); }

Vec2 RenderArea::map_coords_from_mouse(QPoint mouseVec2s) {
  float aspect_ratio =
      static_cast<float>(width()) / static_cast<float>(height());
  float x_relative = static_cast<float>(mouseVec2s.x()) /
                     static_cast<float>(width()) * aspect_ratio;
  float y_relative =
      static_cast<float>(mouseVec2s.y()) / static_cast<float>(height());

  QMatrix4x4 viewinv = camera->get_view().inverted();
  Vec2 c;
  QPointF p = viewinv.map(QPointF(x_relative, y_relative));
  c.x = BigInt(std::floor(p.x()));
  c.y = BigInt(std::floor(p.y()));

  return c;
}

void RenderArea::down() { move_camera({0.0f, 0.05f}); }
void RenderArea::up() { move_camera({0.0f, -0.05f}); }
void RenderArea::right() { move_camera({0.05f, 0.0f}); }
void RenderArea::left() { move_camera({-0.05f, 0.0f}); }

void RenderArea::move_camera(QPointF vector) {
  camera->pos.setX(camera->pos.x() + vector.x());
  camera->pos.setY(camera->pos.y() + vector.y());
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

void RenderArea::fitPattern() {
  if (type == UniverseType::Hashlife) {
    std::pair<Rect, size_t> p =
        reinterpret_cast<HashUniverse *>(hashlife_universe)
            ->get_pattern_bounding_box();

    BigInt cx = p.first.top_left.x;
    cx += p.first.bottom_right().x;
    cx >>= mp_size_t(1);
    BigInt cy = p.first.top_left.y;
    cy += p.first.bottom_right().y;
    cy >>= mp_size_t(1);
    Vec2 center(cx, cy);
    camera->set_zoom(1 << (p.second));
    camera->look_at(center);
    update();
  }
}
