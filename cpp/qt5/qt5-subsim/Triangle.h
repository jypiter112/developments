#pragma once

#include <QPainter>
#include <QPointF>

// Triangle
//
// A small, self-contained "game object" representing the white triangle.
// It owns its own state (size, rotation) and knows how to:
//   - update(dt): advance its state given a delta-time in seconds
//   - draw(...):  render itself with a QPainter
//
// Keeping this logic inside its own class (rather than inline in the
// widget's paintEvent) is what makes the design object-oriented: the
// RenderWidget doesn't need to know *how* the triangle animates or draws
// itself, only that it can be updated and drawn.
class Triangle {
public:
  explicit Triangle(float size = 10.f);

  // Advance the triangle's internal state. dt is the elapsed time,
  // in seconds, since the previous update.
  void update(double dt);

  // Draw the triangle centered at `center` using the given painter.
  void draw(QPainter &painter) const;

  // Configuration
  void setSize(float size);
  void setRotationSpeed(double degreesPerSecond);
  void setColor(const QColor &color);
  void moveTo(const QPointF &position);

private:
  float m_size;
  double m_rotationDeg;
  double m_rotationSpeedDegPerSec;
  QColor m_color;
  QPointF m_position;
};
