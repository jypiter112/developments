#include "Triangle.h"

Triangle::Triangle(float size)
    : m_size(size),
      m_rotationDeg(0.0),
      m_rotationSpeedDegPerSec(45.0), // slow spin, purely to make dt visible
      m_color(Qt::white)
{
}

void Triangle::update(double dt)
{
    m_rotationDeg += m_rotationSpeedDegPerSec * dt;
    if (m_rotationDeg >= 360.0) {
        m_rotationDeg -= 360.0;
    } else if (m_rotationDeg < 0.0) {
        m_rotationDeg += 360.0;
    }
}

void Triangle::draw(QPainter &painter, const QPointF &center) const
{
    painter.save();

    painter.translate(center);
    painter.rotate(m_rotationDeg);

    // An upward-pointing, roughly equilateral triangle centered on origin.
    const float h = m_size;
    QPolygonF poly;
    poly << QPointF(0.0f, -h * 2.0f / 3.0f)
         << QPointF(-h * 0.6f, h * 1.0f / 3.0f)
         << QPointF(h * 0.6f, h * 1.0f / 3.0f);

    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(m_color));
    painter.drawPolygon(poly);

    painter.restore();
}

void Triangle::setSize(float size)
{
    m_size = size;
}

void Triangle::setRotationSpeed(double degreesPerSecond)
{
    m_rotationSpeedDegPerSec = degreesPerSecond;
}

void Triangle::setColor(const QColor &color)
{
    m_color = color;
}
