#ifndef ALIVEARENA_H
#define ALIVEARENA_H

#include <QObject>

#include "aabstructliveobject.h"
#include "public.h"

class ALiveArena
{
public:
  ALiveArena();
  int colCount() {return COL_COUNT;}
  int rowCount() {return ROW_COUNT;}
  void setObject(int x,int y,EObject typeObject,AAbstructLiveObject *object);
  EObject typeObject(int x,int y);
  AAbstructLiveObject *object(int x,int y);
  void clear();

private:
  struct Cell
  {
    EObject typeObject;
    AAbstructLiveObject *object;
  };

  static const int COL_COUNT = 120;
  static const int ROW_COUNT = 60;
  Cell m_cells[ROW_COUNT][COL_COUNT];

  int translateX(int x);
  int translateY(int y);
};

#endif // ALIVEARENA_H
