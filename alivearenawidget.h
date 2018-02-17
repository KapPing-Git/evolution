#ifndef ALIVEARENAWIDGET_H
#define ALIVEARENAWIDGET_H

#include <QWidget>
#include <QPainter>

#include "alivearena.h"
#include "aliveobject.h"
#include "acommand.h"

class ALiveArenaWidget : public QWidget
{
  Q_OBJECT
public:
  explicit ALiveArenaWidget(QWidget *parent = nullptr, ALiveArena *arena = NULL);
  void setArena(ALiveArena *arena);
  void startView();
  void stopView();

signals:

public slots:

  // QObject interface
protected:
  void timerEvent(QTimerEvent *event);

  // QWidget interface
protected:
  void paintEvent(QPaintEvent *event);

private:
  ALiveArena *m_arena;
  int m_timerId;
  bool m_drawStop;
  void rotatePainter(QPainter *painter,ECource course);//вращаем ось на нужный угол
  void drawStop(QPainter *painter,QRect cellRect);
  void drawMove(QPainter *painter,ECource course,QRect cellRect);
  void drawEat(QPainter *painter,ECource course,QRect cellRect);
  void drawHit(QPainter *painter,ECource course,QRect cellRect);
};

#endif // ALIVEARENAWIDGET_H
