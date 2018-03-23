#include "acommand.h"

AAbstructCommand::AAbstructCommand(int maxCommandCount, ALiveArena *arena)
{
  Q_UNUSED(maxCommandCount);
  m_arena = arena;
//  create(maxCommandCount,arena);
}

AAbstructCommand::~AAbstructCommand()
{

}

void AAbstructCommand::setArena(ALiveArena *arena)
{
  m_arena = arena;
}

IfJump::IfJump(int maxCommandCount, ALiveArena *arena) : AAbstructIfCommand(maxCommandCount,arena)
{
  create(maxCommandCount,arena);
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

void IfJump::create(int maxCommandCount, ALiveArena *arena)
{
  m_arena = arena;
  m_cource = (ECource)(qrand() % (course_end - 1));
  m_object = (EObject)(qrand() % (object_end - 1));
  if (maxCommandCount != 0)
    m_commandNum = qrand() % maxCommandCount;
}

unsigned int IfJump::exec(AAbstructLiveObject *iAm)
{
  switch (m_cource) {
    case course_down:
      if (m_arena->typeObject(iAm->x(),iAm->y() + 1) == m_object)
        return m_commandNum;
      else
        return -1;
      break;

    case course_down_left:
      if (m_arena->typeObject(iAm->x() - 1,iAm->y() + 1) == m_object)
        return m_commandNum;
      else
        return -1;
      break;

    case course_down_right:
      if (m_arena->typeObject(iAm->x() + 1,iAm->y() + 1) == m_object)
        return m_commandNum;
      else
        return -1;
      break;

    case course_left:
      if (m_arena->typeObject(iAm->x() - 1,iAm->y()) == m_object)
        return m_commandNum;
      else
        return -1;
      break;

    case course_right:
      if (m_arena->typeObject(iAm->x() + 1,iAm->y()) == m_object)
        return m_commandNum;
      else
        return -1;
      break;

    case course_up:
      if (m_arena->typeObject(iAm->x(),iAm->y() - 1) == m_object)
        return m_commandNum;
      else
        return -1;
      break;

    case course_up_left:
      if (m_arena->typeObject(iAm->x() - 1,iAm->y() - 1) == m_object)
        return m_commandNum;
      else
        return -1;
      break;

    case course_up_right:
      if (m_arena->typeObject(iAm->x() + 1,iAm->y() - 1) == m_object)
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

AAbstructCommand *IfJump::createChanceCommand(int maxCommandCount, ALiveArena *arena)
{
  return new IfJump(maxCommandCount,arena);
}

AAbstructCommand *IfJump::clone()
{
  IfJump *ifJump = new IfJump();
  *ifJump = *this;
  return ifJump;
}

QString IfJump::textDescription(const QHash<int, QString> &adressNames)
{
  QString description = QObject::tr("если ");
  //направление
  if (m_cource == course_down)
    description.append(QObject::tr("внизу "));
  if (m_cource == course_down_left)
    description.append(QObject::tr("внизу слева "));
  if (m_cource == course_down_right)
    description.append(QObject::tr("внизу справа "));
  if (m_cource == course_left)
    description.append(QObject::tr("слева "));
  if (m_cource == course_right)
    description.append(QObject::tr("справа "));
  if (m_cource == course_up)
    description.append(QObject::tr("сверху "));
  if (m_cource == course_up_left)
    description.append(QObject::tr("вверху слева "));
  if (m_cource == course_up_right)
    description.append(QObject::tr("вверху справа "));

  //объект
  if (m_object == object_food)
    description.append(QObject::tr("еда "));
  if (m_object == object_liveObject)
    description.append(QObject::tr("живой объект "));
  if (m_object == object_none)
    description.append(QObject::tr("пустая клетка "));
  if (m_object == object_toxin)
    description.append(QObject::tr("яд "));
  if (m_object == object_wall)
    description.append(QObject::tr("стена "));

  description.append(QObject::tr("то перейти к ") + adressNames[m_commandNum]);
  return description;
}

void IfJump::toStream(QDataStream &stream)
{
  stream << (quint8)m_cource << (quint8)m_object << m_commandNum;
}

void IfJump::fromStream(QDataStream &stream)
{
  //  quint8 cource,object;
  stream >> (quint8&)m_cource >> (quint8&)m_object >> m_commandNum;
  //  m_cource = (ECource)cource;
  //  m_object = (EObject)object;
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

void Action::create(int maxCommandCount, ALiveArena *arena)
{
  Q_UNUSED(maxCommandCount);
  m_arena = arena;
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
  if(aim.x() < 0) aim.setX(m_arena->colCount() - 1);
  if(aim.x() > m_arena->colCount() - 1) aim.setX(0);
  if(aim.y() < 0) aim.setY(m_arena->rowCount() - 1);
  if(aim.y() > m_arena->rowCount() - 1) aim.setY(0);

  // получаем получаем тип цели и саму цель если это живой объект
  EObject objectType = m_arena->typeObject(aim.x(),aim.y());
  AAbstructLiveObject *object = m_arena->object(aim.x(),aim.y());

  // если действие
  switch (m_action) {
    case action_eat: // есть
      switch (objectType) {
        case object_food:
          iAm->eat(FOOD_COUNT);
          m_arena->setObject(aim.x(),aim.y(),object_none,NULL);
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
          m_arena->setObject(iAm->x(),iAm->y(),object_none,NULL);
          iAm->setPos(aim.x(),aim.y());
          m_arena->setObject(iAm->x(),iAm->y(),object_liveObject,iAm);
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

AAbstructCommand *Action::createChanceCommand(int maxCommandCount, ALiveArena *arena)
{
  return new Action(maxCommandCount,arena);
}

AAbstructCommand *Action::clone()
{
  Action *action = new Action();
  *action =*this;
  return action;
}

QString Action::textDescription(const QHash<int, QString> &adressNames)
{
  Q_UNUSED(adressNames);

  QString description;
  //название комманды
  if (m_action == action_eat)
    description.append(QObject::tr("съесть "));
  if (m_action == action_hit)
    description.append(QObject::tr("ударить "));
  if (m_action == action_move)
    description.append(QObject::tr("двигаться "));
  if (m_action == action_shot)
    description.append(QObject::tr("выстрелить "));
  if (m_action == action_stop)
    description.append(QObject::tr("стоять "));

  //направление
  if (m_action != action_stop)
    {
      if (m_cource == course_down)
        description.append(QObject::tr("вниз "));
      if (m_cource == course_down_left)
        description.append(QObject::tr("вниз слева "));
      if (m_cource == course_down_right)
        description.append(QObject::tr("вниз справа "));
      if (m_cource == course_left)
        description.append(QObject::tr("влево "));
      if (m_cource == course_right)
        description.append(QObject::tr("вправо "));
      if (m_cource == course_up)
        description.append(QObject::tr("вверх "));
      if (m_cource == course_up_left)
        description.append(QObject::tr("вверх слева "));
      if (m_cource == course_up_right)
        description.append(QObject::tr("вверх справа "));
    }
  return description;
}

void Action::toStream(QDataStream &stream)
{
  stream << (quint8)m_cource << (quint8)m_action;
}

void Action::fromStream(QDataStream &stream)
{

  stream >> (quint8&)m_cource >> (quint8&)m_action;
}

Action::Action(int maxCommandCount, ALiveArena *arena) : AAbstructActionCommand(maxCommandCount,arena)
{
  create(maxCommandCount,arena);
}

EAction Action::action()
{
  return m_action;
}

ECource Action::course()
{
  return m_cource;
}

//ACommand::ACommand(int maxCommandCount, AObjectCommonSettings *commonSettings)
//{
//  m_commonSettings = commonSettings;
//  evolution(true, maxCommandCount);
//}

//void ACommand::evolution(bool create, int maxCommandCount, bool onlyIf)
//{
//  if (m_commonSettings == nullptr)
//    return;

//  //сначала определяем превратится ли команда в другую комманду
//  const qreal CHANCE= 0.25;
//  qreal randNum = drand48();

//  //если да то создаём комманда
//  if ((randNum <= CHANCE) || create || onlyIf)
//    {
//      // выбираем чем будет комманда
//      if (onlyIf)
//        m_commandName = commandName_ifJump;
//      else
//        m_commandName = (ECommandName)(qrand() % commandName_end);

//      // если такой комманды не должно быть
//      if (!m_commonSettings->ifLengthActive && m_commandName == commandName_ifLengthJump)
//        m_commandName = commandName_ifJump;

//      switch (m_commandName)
//        {
//        case commandName_ifJump:
//          m_command.ifjump = IfJump();
//          m_command.ifjump.create(maxCommandCount,m_commonSettings);
//          break;

//        case commandName_Action:
//          m_command.action = Action();
//          m_command.action.create(maxCommandCount,m_commonSettings);
//          break;

//        case commandName_Jump:
//          m_command.jump = Jump();
//          m_command.jump.create(maxCommandCount,m_commonSettings);
//          break;

//        case commandName_ifLengthJump:
//          m_command.ifLengthJump = IfLengthJump();
//          m_command.ifLengthJump.create(maxCommandCount,m_commonSettings);
//          break;

//        case commandName_end:
//          break;
//        default:
//          break;
//        }
//    }
//  else //иначе изменяем
//    {
//      switch (m_commandName)
//        {
//        case commandName_ifJump:
//          m_command.ifjump.evolution(maxCommandCount);
//          break;

//        case commandName_Action:
//          m_command.action.evolution(maxCommandCount);
//          break;

//        case commandName_Jump:
//          m_command.jump.evolution(maxCommandCount);
//          break;

//        case commandName_ifLengthJump:
//          m_command.ifLengthJump.evolution(maxCommandCount);
//          break;

//        case commandName_end:
//          break;
//        default:
//          break;
//        }
//    }

//}

//int ACommand::exec(AAbstructLiveObject *iAm)
//{
//  if (m_commonSettings == nullptr)
//    return 1;
//  switch (m_commandName)
//    {
//    case commandName_ifJump:
//      return m_command.ifjump.exec(iAm);
//      break;

//    case commandName_Action:
//      return m_command.action.exec(iAm);
//      break;

//    case commandName_Jump:
//      return m_command.jump.exec(iAm);
//      break;

//    case commandName_ifLengthJump:
//      return m_command.ifLengthJump.exec(iAm);
//      break;

//    case commandName_end:
//      return 1;
//      break;

//    default:
//      return 1;
//      break;
//    }
//  //  ((AAbstructCommand*)&m_command)->exec();
//}

//bool ACommand::isAction()
//{
//  switch (m_commandName)
//    {
//    case commandName_ifJump:
//      return m_command.ifjump.isAction();
//      break;

//    case commandName_Action:
//      return m_command.action.isAction();
//      break;

//    case commandName_Jump:
//      return m_command.jump.isAction();
//      break;

//    case commandName_ifLengthJump:
//      return m_command.ifLengthJump.isAction();
//      break;

//    case commandName_end:
//      return 1;
//      break;

//    default:
//      return 1;
//      break;
//    }
//}

//AObjectCommonSettings *ACommand::commonSettings() const
//{
//    return m_commonSettings;
//}

//void ACommand::setCommonSettings(AObjectCommonSettings *commonSettings)
//{
//    m_commonSettings = commonSettings;
//}

Jump::Jump(int maxCommandCount, ALiveArena *arena) : AAbstructIfCommand(maxCommandCount,arena)
{
  create(maxCommandCount,arena);
}

void Jump::evolution(int maxCommandCount)
{
  m_commandNum = qrand() % maxCommandCount;
}

void Jump::create(int maxCommandCount, ALiveArena *arena)
{
  m_arena = arena;
  if (maxCommandCount != 0)
    m_commandNum = qrand() % maxCommandCount;
}

unsigned int Jump::exec(AAbstructLiveObject *iAm)
{
  Q_UNUSED(iAm);
  return m_commandNum;
}

AAbstructCommand *Jump::createChanceCommand(int maxCommandCount, ALiveArena *arena)
{
  return new Jump(maxCommandCount,arena);
}

AAbstructCommand *Jump::clone()
{
  Jump *jump = new Jump();
  *jump = *this;
  return jump;
}

QString Jump::textDescription(const QHash<int, QString> &adressNames)
{
  return (QObject::tr("перейти к ") + adressNames[m_commandNum]);
}

void Jump::toStream(QDataStream &stream)
{
  stream << m_commandNum;
}

void Jump::fromStream(QDataStream &stream)
{
  stream >> m_commandNum;
}

IfLengthJump::IfLengthJump(int maxCommandCount, ALiveArena *arena) : AAbstructIfCommand(maxCommandCount,arena)
{
  create(maxCommandCount,arena);
}

void IfLengthJump::evolution(int maxCommandCount)
{
  //сначала случайно определим что будем менять
  int z = qrand() % 4;

  //теперь меняем
  if (z == 0) m_cource = (ECource)(qrand() % (course_end - 1));
  if (z == 1) m_object = (EObject)(qrand() % (object_end - 1));
  if (z == 2) m_length = (EObject)(qrand() % qMin(m_arena->colCount(),m_arena->rowCount()));
  if (z == 3) m_commandNum = qrand() % maxCommandCount;
}

void IfLengthJump::create(int maxCommandCount, ALiveArena *arena)
{
  m_arena = arena;
  if ( (maxCommandCount == 0) || (m_arena == nullptr))
    return;
  m_cource = (ECource)(qrand() % (course_end - 1));
  m_object = (EObject)(qrand() % (object_end - 1));
  m_commandNum = qrand() % maxCommandCount;
  m_length = qrand() % qMin(m_arena->colCount(),m_arena->rowCount());
}

unsigned int IfLengthJump::exec(AAbstructLiveObject *iAm)
{
  switch (m_cource)
    {
    case course_down:
      for (int i = 0;i < m_length;i++)
        {
          if (m_arena->typeObject(iAm->x(),iAm->y() + i) != object_none)
            {
              if (m_arena->typeObject(iAm->x(),iAm->y() + i) == m_object)
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
          if (m_arena->typeObject(iAm->x() - i,iAm->y() + i) != object_none)
            {
              if (m_arena->typeObject(iAm->x() - i,iAm->y() + i) == m_object)
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
          if (m_arena->typeObject(iAm->x() + i,iAm->y() + i) != object_none)
            {
              if (m_arena->typeObject(iAm->x() + i,iAm->y() + i) == m_object)
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
          if (m_arena->typeObject(iAm->x() - i,iAm->y()) != object_none)
            {
              if (m_arena->typeObject(iAm->x() - i,iAm->y()) == m_object)
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
          if (m_arena->typeObject(iAm->x() + i,iAm->y()) != object_none)
            {
              if (m_arena->typeObject(iAm->x() + i,iAm->y()) == m_object)
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
          if (m_arena->typeObject(iAm->x(),iAm->y() - i) != object_none)
            {
              if (m_arena->typeObject(iAm->x(),iAm->y() - i) == m_object)
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
          if (m_arena->typeObject(iAm->x() - i,iAm->y() - i) != object_none)
            {
              if (m_arena->typeObject(iAm->x() - i,iAm->y() - i) == m_object)
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
          if (m_arena->typeObject(iAm->x() + i,iAm->y() - i) != object_none)
            {
              if (m_arena->typeObject(iAm->x() + i,iAm->y() - i) == m_object)
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

AAbstructCommand *IfLengthJump::createChanceCommand(int maxCommandCount, ALiveArena *arena)
{
  return new IfLengthJump(maxCommandCount,arena);
}

AAbstructCommand *IfLengthJump::clone()
{
  IfLengthJump *ifLengthJump = new IfLengthJump();
  *ifLengthJump = *this;
  return ifLengthJump;
}

QString IfLengthJump::textDescription(const QHash<int, QString> &adressNames)
{
  QString description = QObject::tr("если ");

  //направление
  if (m_cource == course_down)
    description.append(QObject::tr("внизу "));
  if (m_cource == course_down_left)
    description.append(QObject::tr("внизу слева "));
  if (m_cource == course_down_right)
    description.append(QObject::tr("внизу справа "));
  if (m_cource == course_left)
    description.append(QObject::tr("слева "));
  if (m_cource == course_right)
    description.append(QObject::tr("справа "));
  if (m_cource == course_up)
    description.append(QObject::tr("сверху "));
  if (m_cource == course_up_left)
    description.append(QObject::tr("вверху слева "));
  if (m_cource == course_up_right)
    description.append(QObject::tr("вверху справа "));

  //расстояние
  description.append(QObject::tr("на расстоянии менее ") + QString::number(m_length) + QObject::tr(" клеток ") );

  //объект
  if (m_object == object_food)
    description.append(QObject::tr("еда "));
  if (m_object == object_liveObject)
    description.append(QObject::tr("живой объект "));
  if (m_object == object_none)
    description.append(QObject::tr("пустая клетка "));
  if (m_object == object_toxin)
    description.append(QObject::tr("яд "));
  if (m_object == object_wall)
    description.append(QObject::tr("стена "));

  description.append(QObject::tr("то перейти к ") + adressNames[m_commandNum]);
  return description;
}

void IfLengthJump::toStream(QDataStream &stream)
{
  stream << (quint8)m_cource << (quint8)m_object << m_length << m_commandNum;
}

void IfLengthJump::fromStream(QDataStream &stream)
{
  stream >> (quint8&)m_cource >> (quint8&)m_object >> m_length >> m_commandNum;
}

AAbstructIfCommand::AAbstructIfCommand(int maxCommandCount, ALiveArena *arena) : AAbstructCommand(maxCommandCount,arena)
{
}

bool AAbstructIfCommand::isAction()
{
  return false;
}

quint16 AAbstructIfCommand::commandNum() const
{
  return m_commandNum;
}

void AAbstructIfCommand::setCommandNum(quint16 commandNum)
{
  m_commandNum = commandNum;
}

AAbstructActionCommand::AAbstructActionCommand(int maxCommandCount, ALiveArena *arena) : AAbstructCommand(maxCommandCount,arena)
{
}

bool AAbstructActionCommand::isAction()
{
  return true;
}

ACommandGenerator::ACommandGenerator(int maxCommandCount, ALiveArena *arena)
{
  addCommand(new IfJump(maxCommandCount,arena),"ifJump",commandName_ifJump,false,0);
  addCommand(new Jump(maxCommandCount,arena),"jump",commandName_Jump,false,0);
  addCommand(new IfLengthJump(maxCommandCount,arena),"ifLengthJump",commandName_ifLengthJump,false,0);
  addCommand(new Action(maxCommandCount,arena),"action",commandName_Action,false,0);
  m_sumChance = 0;
}

ACommandGenerator::~ACommandGenerator()
{
  for (int i = 0;i < m_commands.count();i++)
    {
      delete m_commands[i].command;
    }
}

//void ACommandGenerator::append(AAbstructCommand *command, int chance)
//{
//  AChanceThisCommand z;
//  z.command = command;
//  z.chance = chance;
//  append(z);
//}

//void ACommandGenerator::append(const AChanceThisCommand &t)
//{
//  m_sumChance += t.chance;
//  QList<AChanceThisCommand>::append(t);
//}

AAbstructCommand *ACommandGenerator::generate(int maxCommandCount, ALiveArena *arena)
{
  if (m_sumChance == 0) return nullptr;
  int chance = qrand() % m_sumChance;
  int min = 0;
  AAbstructCommand *command = nullptr;
  for (int i = 0; i < m_commands.count();i++)
    {
      ACommandDescription chanceThisCommand = m_commands[i];
      if (chanceThisCommand.exist)
        {
          int max = min + chanceThisCommand.chance;
          if ((chance >= min) && (chance < max))
            {
              command = chanceThisCommand.command;
              break;
            }
          min = max;
        }
    }
  if (command != nullptr)
    return command->createChanceCommand(maxCommandCount,arena);
  else
    return nullptr;
}

bool ACommandGenerator::setCommandOptions(QString name, bool exist, int chance)
{
  bool result = false;
  for (int i = 0;i < m_commands.count();i++)
    {
      ACommandDescription commandDescr = m_commands[i];
      if (commandDescr.name == name)
        {
          commandDescr.exist = exist;
          commandDescr.chance = chance;
          m_commands[i] = commandDescr;
          result = true;
          break;
        }
    }
  update();
  return result;
}

bool ACommandGenerator::setCommandOptions(ECommandName type, bool exist, int chance)
{
  bool result = false;
  for (int i = 0;i < m_commands.count();i++)
    {
      ACommandDescription commandDescr = m_commands[i];
      if (commandDescr.type == type)
        {
          commandDescr.exist = exist;
          commandDescr.chance = chance;
          m_commands[i] = commandDescr;
          result = true;
          break;
        }
    }
  update();
  return result;
}

void ACommandGenerator::update()
{
  m_sumChance = 0;
  for (int i = 0;i < m_commands.count();i++)
    {
      ACommandDescription commandDescr = m_commands[i];
      if (commandDescr.exist)
        m_sumChance += commandDescr.chance;
    }
}

void ACommandGenerator::addCommand(AAbstructCommand *command, QString name, ECommandName type, bool exist, int chance)
{
  ACommandDescription commandChance;
  commandChance.command = command;
  commandChance.name = name;
  commandChance.type = type;
  commandChance.exist = exist;
  commandChance.chance = chance;
  m_commands << commandChance;
  m_commandNames << name;
}

QStringList ACommandGenerator::commandNames() const
{
  return m_commandNames;
}

AAbstructCommand *ACommandGenerator::commandByType(ECommandName name, int maxCommandCount, ALiveArena *arena)
{
  switch (name)
    {
    case commandName_Action:
      return new Action(maxCommandCount,arena);
      break;
    case commandName_ifJump:
      return new IfJump(maxCommandCount,arena);
      break;
    case commandName_Jump:
      return new Jump(maxCommandCount,arena);
      break;
    case commandName_ifLengthJump:
      return new IfLengthJump(maxCommandCount,arena);
      break;
    default:
      return nullptr;
      break;
    }
}

bool ACommandGenerator::isEmpty()
{
  if (m_sumChance == 0)
    return true;
  else
    return false;
}
