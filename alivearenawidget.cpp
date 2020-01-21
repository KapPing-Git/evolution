#include "alivearenawidget.h"

ALiveArenaWidget::ALiveArenaWidget(QWidget *parent, ALiveArena *arena) : QWidget(parent)
{
  m_timerId = 0;
  m_drawStop = false;
  startView();
  m_arena = arena;
}

void ALiveArenaWidget::setArena(ALiveArena *arena)
{
  m_arena = arena;
}

void ALiveArenaWidget::startView()
{
  m_timerId = startTimer(75);
  m_drawStop = false;
}

void ALiveArenaWidget::stopView()
{
  killTimer(m_timerId);
  m_drawStop = true;
}

void ALiveArenaWidget::timerEvent(QTimerEvent *event)
{
  Q_UNUSED(event);
  update();
}

void ALiveArenaWidget::paintEvent(QPaintEvent *event)
{
  Q_UNUSED(event);
  if (m_arena == NULL)
    return;
  if (m_drawStop == true)
    return;

  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing);
  // рисуем сетку
  int cellWidth = width() / m_arena->colCount();
  int cellHeight = height() / m_arena->rowCount();
  if (cellWidth < cellHeight)
    cellHeight = cellWidth;
  else
    cellWidth = cellHeight;
  int tableWidth = cellWidth * m_arena->colCount();
  int tableHeight = cellHeight * m_arena->rowCount();

  for (int i = 0; i < m_arena->colCount() + 1;i++)
    painter.drawLine(i*cellWidth,0,i*cellWidth,tableHeight);
  for (int i = 0; i < m_arena->rowCount() + 1; i++)
    painter.drawLine(0,i*cellHeight,tableWidth,i*cellHeight);

  // рисуем объекты сетки
  for (int numRow = 0;numRow < m_arena->rowCount();numRow++)
    {
      for (int numCol = 0; numCol < m_arena->colCount(); numCol++)
        {
          //определяем координаты начала ячейки
          int x = numCol * cellWidth;
          int y = numRow * cellHeight;
          QRect objectRect(x,y,cellWidth,cellHeight);

          ALiveObject *liveObject;
          AAbstructCommand *currCommand;
          QColor liveColor;
          int transparencyLevel = 255;
          switch (m_arena->typeObject(numCol,numRow))
            {
            case object_food:
              painter.setBrush(QBrush(Qt::yellow));
              painter.drawEllipse(objectRect);
              break;

            case object_liveObject:
              liveObject = (ALiveObject *)m_arena->object(numCol,numRow);
              if (liveObject->programm().isEmpty())
                break;
              currCommand = liveObject->lastCommand();

              //определяем насыщенность красному по уровню жизней объекта
              transparencyLevel = 255 *(liveObject->energy()/100);
              if (transparencyLevel > 255)
                transparencyLevel = 255;
              if (!liveObject->isNoob())
                liveColor.setRgb(255,0,0,transparencyLevel);
              else
                liveColor.setRgb(0,0,255,transparencyLevel);

              painter.setBrush(QBrush(liveColor));
              painter.drawEllipse(objectRect);
              if (currCommand->name() == commandName_Action)
                {
                  Action *action = (Action*)currCommand;
                  switch (action->action())
                    {
                    case action_eat:
                      drawEat(&painter,action->course(),objectRect);
                      break;

                    case action_hit:
                      drawHit(&painter,action->course(),objectRect);
                      break;

                    case action_move:
                      drawMove(&painter,action->course(),objectRect);
                      break;

                    case action_shot:
                      drawStop(&painter,objectRect);
                      break;

                    case action_stop:
                      drawStop(&painter,objectRect);
                      break;

                    default:
                      break;
                    }
                }
              break;

            case object_toxin:
              painter.setBrush(QBrush(Qt::green));
              painter.drawEllipse(objectRect);
              break;

            case object_wall:
              painter.setBrush(QBrush(Qt::black));
              painter.drawRect(objectRect);
              break;

            case object_none:
              break;

            case object_end:
              break;

            default:
              break;
            }
        }
    }
}

void ALiveArenaWidget::rotatePainter(QPainter *painter, ECource course)
{
  switch (course)
    {
    case course_down:
      painter->rotate(180);
      break;

    case course_down_left:
      painter->rotate(180+45);
      break;

    case course_down_right:
      painter->rotate(180-45);
      break;

    case course_left:
      painter->rotate(180+90);
      break;

    case course_right:
      painter->rotate(90);
      break;

    case course_up:
      painter->rotate(0);
      break;

    case course_up_left:
      painter->rotate(360-45);
      break;

    case course_up_right:
      painter->rotate(45);
      break;

    default:
      break;
    }
}

void ALiveArenaWidget::drawStop(QPainter *painter, QRect cellRect)
{
  painter->save();

  int subX = cellRect.width() /4;
  int subY = cellRect.height() / 4;
  cellRect.adjust(subX,subY,-subX,-subY);
  painter->setBrush(QBrush(Qt::green));
  painter->drawRect(cellRect);

  painter->restore();
}

void ALiveArenaWidget::drawMove(QPainter *painter, ECource course, QRect cellRect)
{
  painter->save();

  QPen pen = painter->pen();
  pen.setColor(Qt::green);
  painter->setPen(pen);
  painter->translate(cellRect.center());
  rotatePainter(painter,course);
  QRect rect = cellRect;
  rect.moveTo(-cellRect.width()/2,-cellRect.height()/2);

  int xMid = 0;
  int y1 = rect.bottom();
  int y2 = rect.top();
  painter->drawLine(xMid,y1,xMid,y2);
  int yMid = 0;
  int xLeft = rect.left() / 2;
  int xRight = rect.right() / 2;
  painter->drawLine(xLeft,yMid,xMid,y2);
  painter->drawLine(xRight,yMid,xMid,y2);

  //  if (course == course_right)
  //    {
  //      int x1 = cellRect.left();
  //      int x2 = cellRect.right();
  //      int xMid = x1 + (x2 - x1) / 2;
  //      int yMid = cellRect.bottom() - cellRect.height()/2;
  //      painter->drawLine(x1,yMid,x2,yMid);
  //      painter->drawLine(xMid,cellRect.top(),x2,yMid);
  //      painter->drawLine(xMid,cellRect.bottom(),x2,yMid);
  //    }
  //  if (course == course_left)
  //    {
  //      int x1 = cellRect.left();
  //      int x2 = cellRect.right();
  //      int xMid = x1 + (x2 - x1) / 2;
  //      int yMid = cellRect.bottom() - cellRect.height()/2;
  //      painter->drawLine(x1,yMid,x2,yMid);
  //      painter->drawLine(xMid,cellRect.top(),x1,yMid);
  //      painter->drawLine(xMid,cellRect.bottom(),x1,yMid);
  //    }
  //  if (course == course_up)
  //    {
  //      int xMid = cellRect.center().x();
  //      int y1 = cellRect.bottom();
  //      int y2 = cellRect.top();
  //      painter->drawLine(xMid,y1,xMid,y2);
  //      int yMid = cellRect.center().y();
  //      int xLeft = cellRect.left() + cellRect.width() / 4;
  //      int xRight = cellRect.right() - cellRect.width() / 4;
  //      painter->drawLine(xLeft,yMid,xMid,y2);
  //      painter->drawLine(xRight,yMid,xMid,y2);
  //    }
  //  if (course == course_down)
  //    {
  //      int xMid = cellRect.center().x();
  //      int y1 = cellRect.bottom();
  //      int y2 = cellRect.top();
  //      painter->drawLine(xMid,y1,xMid,y2);
  //      int yMid = cellRect.center().y();
  //      int xLeft = cellRect.left() + cellRect.width() / 4;
  //      int xRight = cellRect.right() - cellRect.width() / 4;
  //      painter->drawLine(xLeft,yMid,xMid,y1);
  //      painter->drawLine(xRight,yMid,xMid,y1);
  //    }
  //  if (course == course_down_left)
  //    {
  //      painter->drawLine(cellRect.topRight(),cellRect.bottomLeft());
  //      int yMid = cellRect.center().y();
  //      int xMid = cellRect.center().x();
  //      int xLeft = cellRect.left();
  //      painter->drawLine(xLeft,yMid,xLeft,cellRect.bottom());
  //      painter->drawLine(xLeft,cellRect.bottom(),xMid,cellRect.bottom());
  //    }
  //  if (course == course_up_right)
  //    {
  //      painter->drawLine(cellRect.topRight(),cellRect.bottomLeft());
  //      int yMid = cellRect.center().y();
  //      int xMid = cellRect.center().x();
  //      int xRight= cellRect.right();
  //      painter->drawLine(xMid,cellRect.top(),xRight,cellRect.top());
  //      painter->drawLine(xRight,cellRect.top(),xRight,yMid);
  //    }
  //  if (course == course_up_left)
  //    {
  //      painter->drawLine(cellRect.bottomRight(),cellRect.topLeft());
  //      int yMid = cellRect.center().y();
  //      int xMid = cellRect.center().x();
  //      int xLeft = cellRect.left();
  //      painter->drawLine(xLeft,cellRect.top(),xLeft,yMid);
  //      painter->drawLine(xLeft,cellRect.top(),xMid,cellRect.top());
  //    }
  //  if (course == course_down_right)
  //    {
  //      painter->drawLine(cellRect.bottomRight(),cellRect.topLeft());
  //      int yMid = cellRect.center().y();
  //      int xMid = cellRect.center().x();
  //      int xRight= cellRect.right();
  //      painter->drawLine(xMid,cellRect.bottom(),xRight,cellRect.bottom());
  //      painter->drawLine(xRight,yMid,xRight,cellRect.bottom());
  //    }

  painter->restore();

}

void ALiveArenaWidget::drawEat(QPainter *painter,ECource course, QRect cellRect)
{
  painter->save();
  QRect rect = cellRect;
  rect.moveTo(-cellRect.width()/2,-cellRect.height()/2);
  painter->translate(cellRect.center());
  rotatePainter(painter,course);
  //  switch (course)
  //    {
  //    case course_down:
  //      painter->rotate(180);
  //      break;

  //    case course_down_left:
  //      painter->rotate(180-45);
  //      break;

  //    case course_down_right:
  //      painter->rotate(180+45);
  //      break;

  //    case course_left:
  //      painter->rotate(90);
  //      break;

  //    case course_right:
  //      painter->rotate(180+90);
  //      break;

  //    case course_up:
  //      painter->rotate(0);
  //      break;

  //    case course_up_left:
  //      painter->rotate(45);
  //      break;

  //    case course_up_right:
  //      painter->rotate(360-45);
  //      break;

  //    default:
  //      break;
  //    }
  painter->setBrush(QBrush(Qt::green));
  painter->drawPie(rect,(120)*16,(300)*16);
  painter->restore();
}

void ALiveArenaWidget::drawHit(QPainter *painter, ECource course, QRect cellRect)
{
  painter->save();
  QRect rect = cellRect;
  rect.moveTo(-cellRect.width()/2,-cellRect.height()/2);
  painter->translate(cellRect.center());
  rotatePainter(painter,course);
  //  switch (course)
  //    {
  //    case course_down:
  //      painter->rotate(180);
  //      break;

  //    case course_down_left:
  //      painter->rotate(180-45);
  //      break;

  //    case course_down_right:
  //      painter->rotate(180+45);
  //      break;

  //    case course_left:
  //      painter->rotate(90);
  //      break;

  //    case course_right:
  //      painter->rotate(180+90);
  //      break;

  //    case course_up:
  //      painter->rotate(0);
  //      break;

  //    case course_up_left:
  //      painter->rotate(45);
  //      break;

  //    case course_up_right:
  //      painter->rotate(360-45);
  //      break;

  //    default:
  //      break;
  //    }
  QPoint point[3];
  point[0].setX(-rect.width()/4);
  point[0].setY(0);
  point[1].setX(rect.width()/4);
  point[1].setY(0);
  point[2].setX(0);
  point[2].setY(-rect.height());

  painter->setBrush(QBrush(Qt::green));
  painter->drawPolygon(point,3);
  painter->restore();
}
