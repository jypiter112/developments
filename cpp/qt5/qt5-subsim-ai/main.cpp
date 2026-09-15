// main.cpp
// Single-file Qt5 app: a small triangle on a black screen that moves
// toward the last-clicked point, moving at `speed` (pixels/second)
// scaled by frame delta-time (dt).
//
// Build: see accompanying Makefile (uses moc + g++ + pkg-config, no qmake
// needed)

#include <QApplication>
#include <QElapsedTimer>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QPolygonF>
#include <QTimer>
#include <QWidget>
#include <cmath>

class TriangleWidget : public QWidget {
  Q_OBJECT

public:
  explicit TriangleWidget(QWidget *parent = nullptr)
      : QWidget(parent), m_pos(400, 300), m_target(400, 300),
        m_speed(250.0) // pixels per second
  {
    setWindowTitle("Click to move the triangle");
    resize(800, 600);

    // Dark background via palette (paintEvent also fills black each frame)
    QPalette pal = palette();
    pal.setColor(QPalette::Window, Qt::black);
    setPalette(pal);
    setAutoFillBackground(true);

    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &TriangleWidget::tick);
    m_elapsed.start();
    m_timer->start(16); // ~60 FPS
  }

protected:
  void paintEvent(QPaintEvent *) override {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    // Black background
    painter.fillRect(rect(), Qt::black);

    ui_draw(painter);

    if (fl_moving_drawline) {
      // Draw a line from the triangle to the clicked position
      painter.setPen(QPen(Qt::white, 1, Qt::DashLine));
      painter.drawLine(m_pos, m_target);
    }

    // Tiny triangle centered on m_pos, pointing "up"
    const qreal r = 10.0; // half-size of the triangle
    QPolygonF triangle;
    triangle << QPointF(m_pos.x(), m_pos.y() - r)
             << QPointF(m_pos.x() - r, m_pos.y() + r)
             << QPointF(m_pos.x() + r, m_pos.y() + r);

    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::white);
    painter.drawPolygon(triangle);
  }

  void mousePressEvent(QMouseEvent *event) override {
    m_target = event->pos();
    fl_moving_drawline = true;
  }
  void keyPressEvent(QKeyEvent *event) override {
    switch (event->key()) {
    case Qt::Key_W:
      m_speed_mode++;
      if (m_speed_mode > 5)
        m_speed_mode = 5;
      break;
    case Qt::Key_S:
      m_speed_mode--;
      if (m_speed_mode < 0)
        m_speed_mode = 0;
      break;
    default:
      QWidget::keyPressEvent(event);
    }
    update();
  }

private slots:
  void tick() {
    // Delta time in seconds since the last tick
    const qint64 ns = m_elapsed.restart();
    const double dt = ns / 1000.0; // restart() returns ms

    QPointF diff = m_target - m_pos;
    const double dist = std::sqrt(QPointF::dotProduct(diff, diff));

    if (dist > 0.5) {
      QPointF dir = diff / dist;  // unit vector toward target
      double step = m_speed * dt; // distance to move this frame
      if (step > dist)
        step = dist; // don't overshoot
      m_pos += dir * step;
      update();
    } else {
      fl_moving_drawline = false;
    }
  }

private:
  void ui_draw(QPainter &painter) {
    painter.setPen(Qt::white);
    painter.setFont(QFont("Monospace", 8));
    QString ui_text = QString("Position: (%1, %2)\nMoving to: (%3, %4)\nSpeed: "
                              "%5 px/s,\nMode: %6\n")
                          .arg(m_pos.x(), 0, 'f', 1)
                          .arg(m_pos.y(), 0, 'f', 1)
                          .arg(m_target.x(), 0, 'f', 1)
                          .arg(m_target.y(), 0, 'f', 1)
                          .arg(m_speed, 0, 'f', 1)
                          .arg(m_speed_mode);
    const int margin = 6;
    QFontMetrics fm(painter.font());
    int textHeight = fm.height();

    painter.drawText(margin, height() - margin, ui_text);
  }
  QPointF m_pos;    // current triangle position
  QPointF m_target; // last clicked position
  double m_speed;   // movement speed, pixels/second
  QTimer *m_timer;
  QElapsedTimer m_elapsed;
  bool fl_moving_drawline = false; // draws line from m_pos to clicked position
                                   //
                                   // Sub speeds, move to class later
  bool m_moving = false;
  int m_speed_mode =
      0; // 0=standard, 1=onethird, 2=twothirds, 3=sprint, 4=flank, 5=stop
};

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  TriangleWidget w;
  w.show();
  return app.exec();
}

#include "main.moc"
