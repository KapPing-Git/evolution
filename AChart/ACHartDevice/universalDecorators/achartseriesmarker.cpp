#include "achartseriesmarker.h"

AChartSeriesMarker::AChartSeriesMarker(AChart *parent, AChartSeries *series, QString text, qreal x, qreal y)
  :AChartSeriesDecorator(parent,series,x,0)
{
  initialase(x,y,text);
}

AChartSeriesMarker::AChartSeriesMarker(AChart *parent, int numSeries, QString text, qreal x, qreal y)
  :AChartSeriesDecorator(parent,numSeries,x,0)
{
  initialase(x,y,text);
}

AChartSeriesMarker::AChartSeriesMarker(AChart *parent, QString nameSeries, QString text, qreal x, qreal y)
  :AChartSeriesDecorator(parent,nameSeries,x,0)
{
  initialase(x,y,text);
}

QString AChartSeriesMarker::typeName()
{
  return "seriesMarker";
}

void AChartSeriesMarker::decoratorUpdate()
{

}

QByteArray AChartSeriesMarker::data()
{
  QByteArray byteArray;
  QDataStream out(&byteArray,QIODevice::WriteOnly);
  out << M_CURR_VERSION << m_axis->name() << m_series->name() << m_x << m_y << m_text << m_color << m_textColor << m_zaprositPriUstanovke;
  return byteArray;
}

void AChartSeriesMarker::setData(QByteArray *byteArray)
{
  QString axisName = "";
  QString seriesName = "";
  quint16 version;
  QDataStream in(byteArray,QIODevice::ReadOnly);
  in >> version;
  if (version == M_CURR_VERSION)
    {
      in >> axisName >> seriesName >> m_x >> m_y >> m_text >> m_color >> m_textColor >> m_zaprositPriUstanovke;
      m_axis = m_parentChart->axisContainer()->axis(axisName);
      m_series = m_parentChart->series(seriesName,axisName);
      m_complete = true;
      m_firstMenuShowed = true;
    }
  else
    {

    }
}

AChartDecorator *AChartSeriesMarker::cloneMyType(AChart *chart)
{
  return new AChartSeriesMarker(chart,0);
}

void AChartSeriesMarker::mousePress(QMouseEvent *event)
{
  if (!m_series->visible()) return;
  event->accept();
  if (m_complete)
    {
      if (event->button() == Qt::LeftButton)
        {
          m_complete = false;
          QPointF mouseRealPoint = m_parentChart->screenToPhysicalCoords(event->pos());
          m_leftIndent = mouseRealPoint.x() - m_x;
          m_topIndent = mouseRealPoint.y() - m_y;
        }
      if (event->button() == Qt::RightButton)
        {
          m_startMenuPoint = event->pos();
        }
    }
}

void AChartSeriesMarker::mouseRelease(QMouseEvent *event)
{
  if (!m_series->visible()) return;
  event->accept();
  if (!m_complete)
    {
      if (event->button() == Qt::LeftButton)
        m_complete = true;
      //При установке маркера сразу выводим меню если надо
      if (!m_firstMenuShowed)
        {
          if (m_zaprositPriUstanovke)
            {
              menuShow(event->globalPos(),true);
              m_firstMenuShowed = true;
            }
        }
    }
  else
    {
      if (event->button() == Qt::RightButton)
        {
          QLineF line(m_startMenuPoint,event->pos());
          if (line.length() < 4)
            menuShow(event->globalPos());
        }
    }
}

void AChartSeriesMarker::mouseMove(QMouseEvent *event)
{
  if (!m_series->visible()) return;
  event->accept();
  if (m_complete)
    {

    }
  else
    {
      QPointF realPoint = m_parentChart->screenToPhysicalCoords(event->pos(),m_axis);
      m_x = realPoint.x() - m_leftIndent;
      m_y = realPoint.y() - m_topIndent;
    }
}

void AChartSeriesMarker::wheel(QWheelEvent *event)
{
  event->ignore();
}

bool AChartSeriesMarker::isMousePressTransperency()
{
  return false;
}

bool AChartSeriesMarker::isMouseReleaseTransperency()
{
  return true;
}

bool AChartSeriesMarker::isMouseMoveTransperency()
{
  return true;
}

bool AChartSeriesMarker::isWheelTransperency()
{
  return true;
}

void AChartSeriesMarker::draw(QPaintDevice *device, QRect chartRect)
{
  if (m_series->visible())
    {
      //задаём настройки рисования
      QPainter painter(device);
      painter.setClipRect(chartRect,Qt::ReplaceClip);
      painter.setRenderHint(QPainter::Antialiasing);
      painter.setPen(m_textColor);
      //печатаем текст и определяем область занимаемую текстом
      int x = m_parentChart->physicalToScreenX(m_x,m_axis);
      int y = m_parentChart->physicalToScreenY(m_y,m_axis);
      painter.drawText(x,y,m_text);
      QRect rect = textRect(x,y,m_text);
      //определяем градиент для данной области
      QLinearGradient gradient(rect.topLeft(),rect.bottomRight());
      int r = m_color.red();
      int g = m_color.green();
      int b = m_color.blue();
      float z = 0.5;
      int alfa = 50;
      gradient.setColorAt(0.0, QColor(r * z,g * z,b * z,alfa));
      gradient.setColorAt(0.5, QColor(r,g,b,alfa));
      gradient.setColorAt(1.0, QColor(r * z,g * z,b * z,alfa));
      QBrush brush = QBrush(gradient);
      //рисуем градиент
      painter.fillRect(rect,brush);

    }
}

bool AChartSeriesMarker::containsScreenPoint(QPoint point)
{
  bool result = false;
  if (m_complete)
    {
      int x = m_parentChart->physicalToScreenX(m_x,m_axis);
      int y = m_parentChart->physicalToScreenY(m_y,m_axis);
      QRect rect = textRect(x,y,m_text);
      if (rect.contains(point))
        result = true;
      else
        result = false;
    }
  else
    result = true;
  return result;
}

bool AChartSeriesMarker::isCanDraw()
{
  if (m_complete)
    {
      if ( ((m_x > m_axis->viewMinX()) && (m_x < m_axis->viewMaxX())) &&
           ((m_y > m_axis->viewMinY()) && (m_y < m_axis->viewMaxY())) )
        return true;
      else
        return false;
    }
  else
    return true;
}

void AChartSeriesMarker::onMenuTextChanged(AChartSeriesMarkerMenu *menu)
{
  m_text = menu->text();
  menu->deleteLater();
  emit textChanged(m_text);
}

void AChartSeriesMarker::onMenuDelete(AChartSeriesMarkerMenu *menu)
{
  menu->deleteLater();
  deleteLater();
}

void AChartSeriesMarker::onMenuClose(AChartSeriesMarkerMenu *menu)
{
  menu->deleteLater();
}

void AChartSeriesMarker::initialase(qreal x, qreal y, QString text)
{
  m_leftIndent = 0;
  m_topIndent = 0;
  m_textColor = Qt::black;
  m_color = Qt::green;
  m_zaprositPriUstanovke = false;
  m_firstMenuShowed = false;
  m_x = x;
  m_y = y;
  m_text = text;
  if (m_parentChart != NULL)
    m_axis = m_parentChart->seriesAxis(m_series);
  if ( (m_x != 0) && (m_y != 0) && (m_text != ""))
    m_complete = true;
  else
    m_complete = false;
  m_menu = NULL;
}

QRect AChartSeriesMarker::textRect(int x, int y, QString text)
{
  QRect rect = m_parentChart->fontMetrics().boundingRect(text);// boundingRect - выдаёт область занимаемую текстом относительно 0,0
  rect.moveLeft(x);
  rect.moveBottom(y + 1);
  rect.adjust(-4,-2,6,2);
  return rect;
}

void AChartSeriesMarker::menuShow(QPoint pos, bool textSelect)
{
  m_menu = new AChartSeriesMarkerMenu(m_text,NULL);
  m_menu->setWindowFlags(Qt::Window);
  m_menu->move(pos);
  m_menu->show();
  connect(m_menu,&AChartSeriesMarkerMenu::changed,this,&AChartSeriesMarker::onMenuTextChanged);
  connect(m_menu,&AChartSeriesMarkerMenu::userSendDelete,this,&AChartSeriesMarker::onMenuDelete);
  connect(m_menu,&AChartSeriesMarkerMenu::closed,this,&AChartSeriesMarker::onMenuClose);
  if (textSelect) m_menu->textSelect();
}
qreal AChartSeriesMarker::y() const
{
    return m_y;
}

void AChartSeriesMarker::setY(const qreal &y)
{
    m_y = y;
}

qreal AChartSeriesMarker::x() const
{
    return m_x;
}

void AChartSeriesMarker::setX(const qreal &x)
{
    m_x = x;
}

bool AChartSeriesMarker::zaprositPriUstanovke() const
{
    return m_zaprositPriUstanovke;
}

void AChartSeriesMarker::setZaprositPriUstanovke(bool zaprositPriUstanovke)
{
  m_zaprositPriUstanovke = zaprositPriUstanovke;
}

QString AChartSeriesMarker::text() const
{
  return m_text;
}

void AChartSeriesMarker::setText(const QString &text)
{
  m_text = text;
  emit textChanged(m_text);
}

QColor AChartSeriesMarker::color() const
{
  return m_color;
}

void AChartSeriesMarker::setColor(const QColor &color)
{
  m_color = color;
}

QColor AChartSeriesMarker::textColor() const
{
  return m_textColor;
}

void AChartSeriesMarker::setTextColor(const QColor &textColor)
{
  m_textColor = textColor;
}

//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

AChartSeriesMarkerMenu::AChartSeriesMarkerMenu(QString text, QWidget *parent)
  :QWidget(parent)
{
  m_lineEdit = new QLineEdit(text,this);
  connect(m_lineEdit,&QLineEdit::returnPressed,this,&AChartSeriesMarkerMenu::on_TextEditFinished);
  m_pushButton = new QPushButton(tr("Удалить"),this);
  connect(m_pushButton,&QPushButton::clicked,this,&AChartSeriesMarkerMenu::on_PushButtonClicked);
  m_vboxLayout = new QVBoxLayout(this);
  m_vboxLayout->addWidget(m_lineEdit);
  m_vboxLayout->addWidget(m_pushButton);
  setLayout(m_vboxLayout);
}

QString AChartSeriesMarkerMenu::text()
{
  return m_lineEdit->text();
}

void AChartSeriesMarkerMenu::textSelect()
{
  m_lineEdit->selectAll();
}

void AChartSeriesMarkerMenu::closeEvent(QCloseEvent *event)
{
  event->accept();
  emit closed(this);
}

void AChartSeriesMarkerMenu::on_TextEditFinished()
{
  emit changed(this);
}

void AChartSeriesMarkerMenu::on_PushButtonClicked()
{
  emit userSendDelete(this);
}
