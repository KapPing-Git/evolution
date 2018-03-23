#ifndef ACHARTSERIESMARKER_H
#define ACHARTSERIESMARKER_H

#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>

#include "AChart/ACHartDevice/achartseriesdecorator.h"

class AChartSeriesMarkerMenu : public QWidget
{
  Q_OBJECT
public:
  explicit AChartSeriesMarkerMenu(QString text, QWidget *parent = 0);
  QString text();
  void textSelect();
signals:
  void changed(AChartSeriesMarkerMenu *menu);
  void userSendDelete(AChartSeriesMarkerMenu *menu);
  void closed(AChartSeriesMarkerMenu *menu);

protected slots:
  void closeEvent(QCloseEvent *event);

private slots:
  void on_TextEditFinished();
  void on_PushButtonClicked();

private:
  QLineEdit *m_lineEdit;
  QPushButton *m_pushButton;
  QVBoxLayout *m_vboxLayout;
};
//---------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------
class AChartSeriesMarker : public AChartSeriesDecorator
{
  Q_OBJECT
public:
  explicit AChartSeriesMarker(AChart *parent,AChartSeries *series, QString text = "", qreal x = 0, qreal y = 0);
  explicit AChartSeriesMarker(AChart *parent,int numSeries, QString text = "", qreal x = 0, qreal y = 0);
  explicit AChartSeriesMarker(AChart *parent,QString nameSeries, QString text = "", qreal x = 0, qreal y = 0);

  virtual QString typeName();
  virtual void decoratorUpdate();

  virtual QByteArray data();
  virtual void setData(QByteArray *byteArray);
  virtual AChartDecorator *cloneMyType(AChart *chart);

  QColor textColor() const;
  void setTextColor(const QColor &textColor);
  QColor color() const;
  void setColor(const QColor &color);

  QString text() const;
  void setText(const QString &text);

  bool zaprositPriUstanovke() const;
  void setZaprositPriUstanovke(bool zaprositPriUstanovke);

  qreal x() const;
  void setX(const qreal &x);
  qreal y() const;
  void setY(const qreal &y);

signals:
  void textChanged(QString text);

public slots:

protected:
  virtual void mousePress(QMouseEvent *event);
  virtual void mouseRelease(QMouseEvent *event);
  virtual void mouseMove(QMouseEvent *event);
  virtual void wheel(QWheelEvent *event);
  virtual bool isMousePressTransperency();
  virtual bool isMouseReleaseTransperency();
  virtual bool isMouseMoveTransperency();
  virtual bool isWheelTransperency();

  virtual void draw(QPaintDevice *device,QRect chartRect);
  virtual bool containsScreenPoint(QPoint point);
  virtual bool isCanDraw();

private slots:
  void onMenuTextChanged(AChartSeriesMarkerMenu *menu);
  void onMenuDelete(AChartSeriesMarkerMenu *menu);
  void onMenuClose(AChartSeriesMarkerMenu *menu);

private:
  void initialase(qreal x = 0, qreal y = 0, QString text = "");
  QRect textRect(int x,int y,QString text);
  void menuShow(QPoint pos,bool textSelect = false);
  qreal m_x;
  qreal m_y;
  QString m_text;
  QColor m_textColor;
  QColor m_color;

  AChartAxis *m_axis;
  qreal m_leftIndent;
  qreal m_topIndent;
  QPoint m_startMenuPoint;
  AChartSeriesMarkerMenu *m_menu;
  bool m_zaprositPriUstanovke;
  bool m_firstMenuShowed;

  static const quint16 M_CURR_VERSION = 0;
};

#endif // ACHARTSERIESMARKER_H
