#include "RenderWidget.h"

#include <QPainter>
#include <QPalette>

RenderWidget::RenderWidget(QWidget *parent)
    : QWidget(parent),
      m_lastNs(0),
      m_triangle(120.0f)
{
    setWindowTitle(QStringLiteral("Qt Triangle Clock (fixed 60 FPS, dt-driven)"));
    resize(640, 480);

    // Solid black background.
    QPalette pal = palette();
    pal.setColor(QPalette::Window, Qt::black);
    setAutoFillBackground(true);
    setPalette(pal);

    // Start the high-resolution clock used to measure real dt between ticks.
    m_clock.start();
    m_lastNs = m_clock.nsecsElapsed();

    // Fixed-fps timer: fires onTick() roughly every 1000/60 ms.
    connect(&m_timer, &QTimer::timeout, this, &RenderWidget::onTick);
    m_timer.start(kFrameIntervalMs);
}

void RenderWidget::onTick()
{
    const qint64 nowNs = m_clock.nsecsElapsed();
    const double dt = static_cast<double>(nowNs - m_lastNs) / 1e9; // seconds
    m_lastNs = nowNs;

    // Advance simulation state using the real measured delta-time,
    // rather than assuming exactly 1/60s per tick.
    m_triangle.update(dt);

    // Schedule a repaint; Qt will call paintEvent() for us.
    update();
}

void RenderWidget::paintEvent(QPaintEvent * /*event*/)
{
    QPainter painter(this);

    // Black filled background.
    painter.fillRect(rect(), Qt::black);

    // Let the triangle draw itself, centered in the widget.
    const QPointF center(width() / 2.0, height() / 2.0);
    m_triangle.draw(painter, center);
}
