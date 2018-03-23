#ifndef AABSTRACTSERIESOPTIMISER_H
#define AABSTRACTSERIESOPTIMISER_H

#include <QObject>
#include <QPointF>
#include <QRect>
#include <QRectF>

#include "AChart/achartpolyline.h"

class AAbstractSeriesOptimiser : public QObject
{
  Q_OBJECT
public:
  explicit AAbstractSeriesOptimiser(QObject *parent = 0);
  virtual void addPoint(QPointF point) = 0;
  virtual AChartPolyline optimisePolyline(QRectF realView, QRect screenView) = 0;
  virtual bool optimisePolylineExist() = 0;

signals:

public slots:
  virtual void setViews(QRectF realView, QRect screenView) = 0;

};

#endif // AABSTRACTSERIESOPTIMISER_H
