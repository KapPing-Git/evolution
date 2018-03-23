#ifndef AABSTRUCTLIVEOBJECT_H
#define AABSTRUCTLIVEOBJECT_H

#include <QObject>
#include <QPoint>

class AAbstructLiveObject : public QObject
{
  Q_OBJECT
public:
  explicit AAbstructLiveObject(QObject *parent = nullptr);
  virtual void createChanceProgramm() = 0;
  virtual void evolution() = 0;
  virtual void exec() = 0;
  virtual AAbstructLiveObject *born(QObject *parent = 0) = 0;
  virtual int x() = 0;
  virtual void setX(int x) = 0;
  virtual int y() = 0;
  virtual void setY(int y) = 0;
  virtual void setPos(int x, int y) = 0;
  virtual QPoint pos() = 0;
  virtual qreal posX() = 0;
  virtual qreal posY() = 0;

  virtual void eat(qreal energyCount) = 0;
  virtual void die() = 0;
  virtual void damage(qreal energyCount) = 0;
  virtual qreal hitPower() = 0;
  virtual bool isDie() = 0;

signals:

public slots:
};

#endif // AABSTRUCTLIVEOBJECT_H
