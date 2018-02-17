#ifndef PUBLIC_H
#define PUBLIC_H

#include <QObject>

enum EObject
{
  object_none,
  object_liveObject,
  object_food,
  object_toxin,
  object_wall,
  object_end
};

enum EAction
{
  action_move,
  action_stop,
  action_hit,
  action_shot,
  action_eat,
  action_end
};

enum ECource
{
  course_up,
  course_down,
  course_left,
  course_right,
  course_up_left,
  course_up_right,
  course_down_left,
  course_down_right,
  course_end
};

enum ECommandName
{
  commandName_ifJump,
  commandName_Action,
  commandName_Jump,
  commandName_ifLengthJump,
  commandName_end
};

const qreal FOOD_COUNT = 50;

inline double dRandom(double min,double max)
{
  return min + (max - min) * (qrand() / RAND_MAX);
}

#endif // PUBLIC_H
