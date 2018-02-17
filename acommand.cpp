#include "acommand.h"

AAbstructCommand::AAbstructCommand()
{

}

void IfJump::evolution(int maxCommandCount)
{
  //сначала случайно определим что будем менять
  int z = qrand() % 3;

  //теперь меняем
  if (z == 0) m_cource = (ECource)(qrand() % (course_end - 1));
  if (z == 1) m_object = (EObject)(qrand() % (object_end - 1));
  if (z == 2)
    {
      m_commandNum = qrand() % maxCommandCount;
    }
}

void IfJump::create(int maxCommandCount, AObjectCommonSettings *commonSettings)
{
  m_commonSettings = commonSettings;
  m_cource = (ECource)(qrand() % (course_end - 1));
  m_object = (EObject)(qrand() % (object_end - 1));
  m_commandNum = qrand() % maxCommandCount;
  //  if (commandNum < 2) // прыгать на 1 шаг безсмысленно, а при 0 программа вообще остановится
  //    commandNum = 2;
}

unsigned int IfJump::exec(AAbstructLiveObject *iAm)
{
  switch (m_cource) {
    case course_down:
      if (m_commonSettings->arena->typeObject(iAm->x(),iAm->y() + 1) == m_object)
        return m_commandNum;
      else
        return -1;
      break;

    case course_down_left:
      if (m_commonSettings->arena->typeObject(iAm->x() - 1,iAm->y() + 1) == m_object)
        return m_commandNum;
      else
        return -1;
      break;

    case course_down_right:
      if (m_commonSettings->arena->typeObject(iAm->x() + 1,iAm->y() + 1) == m_object)
        return m_commandNum;
      else
        return -1;
      break;

    case course_left:
      if (m_commonSettings->arena->typeObject(iAm->x() - 1,iAm->y()) == m_object)
        return m_commandNum;
      else
        return -1;
      break;

    case course_right:
      if (m_commonSettings->arena->typeObject(iAm->x() + 1,iAm->y()) == m_object)
        return m_commandNum;
      else
        return -1;
      break;

    case course_up:
      if (m_commonSettings->arena->typeObject(iAm->x(),iAm->y() - 1) == m_object)
        return m_commandNum;
      else
        return -1;
      break;

    case course_up_left:
      if (m_commonSettings->arena->typeObject(iAm->x() - 1,iAm->y() - 1) == m_object)
        return m_commandNum;
      else
        return -1;
      break;

    case course_up_right:
      if (m_commonSettings->arena->typeObject(iAm->x() + 1,iAm->y() - 1) == m_object)
        return m_commandNum;
      else
        return -1;
      break;

    case course_end:
      return -1;
      break;

    default:
      return -1;
      break;
    }
}

bool IfJump::isAction()
{
  return false;
}

void Action::evolution(int maxCommandCount)
{
  Q_UNUSED(maxCommandCount);
  //сначала случайно определим что будем менять
  int z = qrand() % 3;

  //теперь меняем
  if (z == 0) m_cource = (ECource)(qrand() % (course_end));
  if (z == 1) m_action = (EAction)(qrand() % (action_end));
}

void Action::create(int maxCommandCount, AObjectCommonSettings *commonSettings)
{
  Q_UNUSED(maxCommandCount);
  m_commonSettings = commonSettings;
  m_cource = (ECource)(qrand() % (course_end));
  m_action = (EAction)(qrand() % (action_end));
}

unsigned int Action::exec(AAbstructLiveObject *iAm)
{
  // получаем координаты цели
  QPoint aim;
  switch (m_cource) {
    case course_down:
      aim.setX(iAm->x());
      aim.setY(iAm->y() + 1);
      break;

    case course_down_left:
      aim.setX(iAm->x() - 1);
      aim.setY(iAm->y() + 1);
      break;

    case course_down_right:
      aim.setX(iAm->x() + 1);
      aim.setY(iAm->y() + 1);
      break;

    case course_left:
      aim.setX(iAm->x() - 1);
      aim.setY(iAm->y());
      break;

    case course_right:
      aim.setX(iAm->x() + 1);
      aim.setY(iAm->y());
      break;

    case course_up:
      aim.setX(iAm->x());
      aim.setY(iAm->y() - 1);
      break;

    case course_up_left:
      aim.setX(iAm->x() - 1);
      aim.setY(iAm->y() - 1);
      break;

    case course_up_right:
      aim.setX(iAm->x() + 1);
      aim.setY(iAm->y() - 1);;
      break;

    case course_end:
      break;

    default:
      break;
    }
  if(aim.x() < 0) aim.setX(m_commonSettings->arena->colCount() - 1);
  if(aim.x() > m_commonSettings->arena->colCount() - 1) aim.setX(0);
  if(aim.y() < 0) aim.setY(m_commonSettings->arena->rowCount() - 1);
  if(aim.y() > m_commonSettings->arena->rowCount() - 1) aim.setY(0);

  // получаем получаем тип цели и саму цель если это живой объект
  EObject objectType = m_commonSettings->arena->typeObject(aim.x(),aim.y());
  AAbstructLiveObject *object = m_commonSettings->arena->object(aim.x(),aim.y());

  // если действие
  switch (m_action) {
    case action_eat: // есть
      switch (objectType) {
        case object_food:
          iAm->eat(FOOD_COUNT);
          m_commonSettings->arena->setObject(aim.x(),aim.y(),object_none,NULL);
          break;

        case object_liveObject:
          iAm->eat(-3);
          break;

        case object_none:
          iAm->eat(-2);
          break;

        case object_toxin:
          iAm->die();
          break;

        case object_wall:
          iAm->damage(10);
          iAm->eat(-5);
          break;

        default:
          iAm->eat(-1);
          break;
        }
      break;

    case action_hit: //удар
      switch (objectType) {
        case object_food:
          iAm->eat(-1);
          break;

        case object_liveObject:
          object->damage(iAm->hitPower());
          break;

        case object_none:
          iAm->eat(-1);
          break;

        case object_toxin:
          iAm->die();
          break;

        case object_wall:
          iAm->damage(10);
          iAm->eat(-5);
          break;

        default:
          iAm->eat(-1);
          break;
        }
      break;

    case action_move: // движение
      if (objectType == object_none)
        {
          m_commonSettings->arena->setObject(iAm->x(),iAm->y(),object_none,NULL);
          iAm->setPos(aim.x(),aim.y());
          m_commonSettings->arena->setObject(iAm->x(),iAm->y(),object_liveObject,iAm);
          iAm->eat(-2);
        }
      else
        {
          if (objectType == object_toxin)
            iAm->die();
          else
            iAm->eat(-1);
        }
      break;

    case action_shot: // выстрел
      iAm->eat(-1); //пока не руализую это действие
      break;

    case action_stop: // стоять на месте
      iAm->eat(-1);
      break;

    case action_end:
      iAm->eat(-1);
      break;

    default:
      iAm->eat(-1);
      break;
    }
  return -1;
}

bool Action::isAction()
{
  return true;
}

EAction Action::action()
{
  return m_action;
}

ECource Action::course()
{
  return m_cource;
}

ACommand::ACommand(int maxCommandCount, AObjectCommonSettings *commonSettings)
{
  m_commonSettings = commonSettings;
  evolution(true, maxCommandCount);
}

void ACommand::evolution(bool create, int maxCommandCount, bool onlyIf)
{
  if (m_commonSettings == nullptr)
    return;

  //сначала определяем превратится ли команда в другую комманду
  const qreal CHANCE= 0.25;
  qreal randNum = drand48();

  //если да то создаём комманда
  if ((randNum <= CHANCE) || create || onlyIf)
    {
      // выбираем чем будет комманда
      if (onlyIf)
        m_commandName = commandName_ifJump;
      else
        m_commandName = (ECommandName)(qrand() % commandName_end);

      // если такой комманды не должно быть
      if (!m_commonSettings->ifLengthActive && m_commandName == commandName_ifLengthJump)
        m_commandName = commandName_ifJump;

      switch (m_commandName)
        {
        case commandName_ifJump:
          m_command.ifjump = IfJump();
          m_command.ifjump.create(maxCommandCount,m_commonSettings);
          break;

        case commandName_Action:
          m_command.action = Action();
          m_command.action.create(maxCommandCount,m_commonSettings);
          break;

        case commandName_Jump:
          m_command.jump = Jump();
          m_command.jump.create(maxCommandCount,m_commonSettings);
          break;

        case commandName_ifLengthJump:
          m_command.ifLengthJump = IfLengthJump();
          m_command.ifLengthJump.create(maxCommandCount,m_commonSettings);
          break;

        case commandName_end:
          break;
        default:
          break;
        }
    }
  else //иначе изменяем
    {
      switch (m_commandName)
        {
        case commandName_ifJump:
          m_command.ifjump.evolution(maxCommandCount);
          break;

        case commandName_Action:
          m_command.action.evolution(maxCommandCount);
          break;

        case commandName_Jump:
          m_command.jump.evolution(maxCommandCount);
          break;

        case commandName_ifLengthJump:
          m_command.ifLengthJump.evolution(maxCommandCount);
          break;

        case commandName_end:
          break;
        default:
          break;
        }
    }

}

int ACommand::exec(AAbstructLiveObject *iAm)
{
  if (m_commonSettings == nullptr)
    return 1;
  switch (m_commandName)
    {
    case commandName_ifJump:
      return m_command.ifjump.exec(iAm);
      break;

    case commandName_Action:
      return m_command.action.exec(iAm);
      break;

    case commandName_Jump:
      return m_command.jump.exec(iAm);
      break;

    case commandName_ifLengthJump:
      return m_command.ifLengthJump.exec(iAm);
      break;

    case commandName_end:
      return 1;
      break;

    default:
      return 1;
      break;
    }
  //  ((AAbstructCommand*)&m_command)->exec();
}

bool ACommand::isAction()
{
  switch (m_commandName)
    {
    case commandName_ifJump:
      return m_command.ifjump.isAction();
      break;

    case commandName_Action:
      return m_command.action.isAction();
      break;

    case commandName_Jump:
      return m_command.jump.isAction();
      break;

    case commandName_ifLengthJump:
      return m_command.ifLengthJump.isAction();
      break;

    case commandName_end:
      return 1;
      break;

    default:
      return 1;
      break;
    }
}

AObjectCommonSettings *ACommand::commonSettings() const
{
    return m_commonSettings;
}

void ACommand::setCommonSettings(AObjectCommonSettings *commonSettings)
{
    m_commonSettings = commonSettings;
}

void Jump::evolution(int maxCommandCount)
{
    m_commandNum = qrand() % maxCommandCount;
}

void Jump::create(int maxCommandCount, AObjectCommonSettings *commonSettings)
{
  m_commonSettings = commonSettings;
  m_commandNum = qrand() % maxCommandCount;
}

unsigned int Jump::exec(AAbstructLiveObject *iAm)
{
  Q_UNUSED(iAm);
  return m_commandNum;
}

bool Jump::isAction()
{
  return false;
}

void IfLengthJump::evolution(int maxCommandCount)
{
  //сначала случайно определим что будем менять
  int z = qrand() % 4;

  //теперь меняем
  if (z == 0) m_cource = (ECource)(qrand() % (course_end - 1));
  if (z == 1) m_object = (EObject)(qrand() % (object_end - 1));
  if (z == 2) m_length = (EObject)(qrand() % qMin(m_commonSettings->arena->colCount(),m_commonSettings->arena->rowCount()));
  if (z == 3) m_commandNum = qrand() % maxCommandCount;
}

void IfLengthJump::create(int maxCommandCount, AObjectCommonSettings *commonSettings)
{
  m_commonSettings = commonSettings;
  m_cource = (ECource)(qrand() % (course_end - 1));
  m_object = (EObject)(qrand() % (object_end - 1));
  m_commandNum = qrand() % maxCommandCount;
  m_length = qrand() % qMin(m_commonSettings->arena->colCount(),m_commonSettings->arena->rowCount());
}

unsigned int IfLengthJump::exec(AAbstructLiveObject *iAm)
{
  switch (m_cource)
    {
    case course_down:
      for (int i = 0;i < m_length;i++)
        {
          if (m_commonSettings->arena->typeObject(iAm->x(),iAm->y() + i) != object_none)
            {
              if (m_commonSettings->arena->typeObject(iAm->x(),iAm->y() + i) == m_object)
                return m_commandNum;
              else
                return -1;
            }
        }
      return -1;
      break;

    case course_down_left:
      for (int i = 0;i < m_length;i++)
        {
          if (m_commonSettings->arena->typeObject(iAm->x() - i,iAm->y() + i) != object_none)
            {
              if (m_commonSettings->arena->typeObject(iAm->x() - i,iAm->y() + i) == m_object)
                return m_commandNum;
              else
                return -1;
            }
        }
      return -1;
      break;

    case course_down_right:
      for (int i = 0;i < m_length;i++)
        {
          if (m_commonSettings->arena->typeObject(iAm->x() + i,iAm->y() + i) != object_none)
            {
              if (m_commonSettings->arena->typeObject(iAm->x() + i,iAm->y() + i) == m_object)
                return m_commandNum;
              else
                return -1;
            }
        }
      return -1;
      break;

    case course_left:
      for (int i = 0;i < m_length;i++)
        {
          if (m_commonSettings->arena->typeObject(iAm->x() - i,iAm->y()) != object_none)
            {
              if (m_commonSettings->arena->typeObject(iAm->x() - i,iAm->y()) == m_object)
                return m_commandNum;
              else
                return -1;
            }
        }
      return -1;
      break;

    case course_right:
      for (int i = 0;i < m_length;i++)
        {
          if (m_commonSettings->arena->typeObject(iAm->x() + i,iAm->y()) != object_none)
            {
              if (m_commonSettings->arena->typeObject(iAm->x() + i,iAm->y()) == m_object)
                return m_commandNum;
              else
                return -1;
            }
        }
      return -1;
      break;

    case course_up:
      for (int i = 0;i < m_length;i++)
        {
          if (m_commonSettings->arena->typeObject(iAm->x(),iAm->y() - i) != object_none)
            {
              if (m_commonSettings->arena->typeObject(iAm->x(),iAm->y() - i) == m_object)
                return m_commandNum;
              else
                return -1;
            }
        }
      return -1;
      break;

    case course_up_left:
      for (int i = 0;i < m_length;i++)
        {
          if (m_commonSettings->arena->typeObject(iAm->x() - i,iAm->y() - i) != object_none)
            {
              if (m_commonSettings->arena->typeObject(iAm->x() - i,iAm->y() - i) == m_object)
                return m_commandNum;
              else
                return -1;
            }
        }
      return -1;
      break;

    case course_up_right:
      for (int i = 0;i < m_length;i++)
        {
          if (m_commonSettings->arena->typeObject(iAm->x() + i,iAm->y() - i) != object_none)
            {
              if (m_commonSettings->arena->typeObject(iAm->x() + i,iAm->y() - i) == m_object)
                return m_commandNum;
              else
                return -1;
            }
        }
      return -1;
      break;

    case course_end:
      return -1;
      break;

    default:
      return -1;
      break;
    }
}

bool IfLengthJump::isAction()
{
  return false;
}
