#pragma once

#include <QElapsedTimer>
#include <QMouseEvent>
#include <QTimer>
#include <QWidget>

#include "Triangle.h"

// RenderWidget
//
// Owns the "clock": a QTimer fixed to a 60 FPS interval, and a
// QElapsedTimer used to measure the real delta-time (dt) between ticks.
// Each tick, it computes dt and forwards it to the Triangle object so the
// triangle can update itself independently of frame rate hiccups.
//
// The widget itself is only responsible for:
//   - running the fixed-fps loop
//   - clearing the background to black
//   - asking the Triangle to draw itself
class RenderWidget : public QWidget {
  Q_OBJECT

public:
  explicit RenderWidget(QWidget *parent = nullptr);

protected:
  // hooks
  void paintEvent(QPaintEvent *event) override;
  void mousePressEvent(QMouseEvent *event) override;
private slots:
  // Called by the QTimer at a fixed 60 Hz interval.
  void onTick();

private:
  static constexpr int kTargetFps = 60;
  static constexpr int kFrameIntervalMs = 1000 / kTargetFps; // ~16 ms

  QTimer m_timer;        // fires onTick() at a fixed 60 fps interval
  QElapsedTimer m_clock; // used to measure real elapsed time (dt)
  qint64 m_lastNs;       // timestamp (ns) of the previous tick

  Triangle m_triangle; // the object being animated / rendered
};
