#ifndef PRESERIESOPTIMISER_H
#define PRESERIESOPTIMISER_H

#include <QList>

#include "aabstractseriesoptimiser.h"


class preSeriesOptimiser : public AAbstractSeriesOptimiser
{
public:
  preSeriesOptimiser();

  // AAbstractSeriesOptimiser interface
public:
  void addPoint(QPointF point);
  AChartPolyline optimisePolyline(QRectF realView, QRect screenView);
  bool optimisePolylineExist();

public slots:
  void setViews(QRectF realView, QRect screenView);

private:
  QList<AChartPolyline> m_polylines;
};

#endif // PRESERIESOPTIMISER_H
