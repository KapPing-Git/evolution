#include "alivearena.h"

ALiveArena::ALiveArena()
{
  clear();
}

void ALiveArena::setObject(int x, int y, EObject typeObject, AAbstructLiveObject *object)
{
  m_cells[translateY(y)][translateX(x)].object = object;
  m_cells[translateY(y)][translateX(x)].typeObject = typeObject;
}

EObject ALiveArena::typeObject(int x, int y)
{
//  if (x >= COL_COUNT)
//    x = 0;
//  if (x < 0)
//    x = COL_COUNT - 1;
//  if (y >= ROW_COUNT)
//    y = 0;
//  if (y < 0)
//    y = ROW_COUNT - 1;
//  int trY = translateY(y);
//  int trX = translateY(x);
  return m_cells[translateY(y)][translateX(x)].typeObject;
}

AAbstructLiveObject *ALiveArena::object(int x, int y)
{
//  if (x >= COL_COUNT) x = 0;
//  if (x < 0) x = COL_COUNT - 1;
//  if (y >= ROW_COUNT) y = 0;
//  if (y < 0) y = ROW_COUNT - 1;
//  int trY = translateY(y);
//  int trX = translateY(x);
  return m_cells[translateY(y)][translateX(x)].object;
}

void ALiveArena::clear()
{
  for (int col = 0; col < COL_COUNT;col++)
    {
      for (int row = 0; row < ROW_COUNT;row++)
        {
          m_cells[row][col].object = NULL;
          m_cells[row][col].typeObject = object_none;
        }
    }
}

int ALiveArena::translateX(int x)
{
  int xTr = x % colCount();
  if (xTr < 0)
    xTr = colCount() + xTr;
  return xTr;
}

int ALiveArena::translateY(int y)
{
  int yTr = y % rowCount();
  if (yTr < 0)
    yTr = rowCount() + yTr;
  return yTr;
}
