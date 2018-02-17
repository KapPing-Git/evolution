#include "aliveobject.h"

ALiveObject::ALiveObject(AObjectCommonSettings *commonSettings, QObject *parent) : AAbstructLiveObject(parent)
{
  m_commandNumber = 0;
  m_lastExecutedCommandNumber = m_commandNumber;
//  m_arena = commonSettings.arena;
  m_commonSettings = commonSettings;
  m_x = 0;
  m_y = 0;
  m_energy = BEGIN_ENERGY;
  m_armor = BEGIN_ARMOR;
  m_hitPower = HIT_POWER;
  m_startCommandCount = 100;
  m_commandCountChangeable = false;
}

void ALiveObject::createChanceProgramm()
{
  for (int i = 0;i < m_startCommandCount;i++)
    {
      ACommand command(m_startCommandCount,m_commonSettings);
      m_programm << command;
    }
  m_iNuub = true;
  m_henotipStabilityTime = 0;
  m_name = hashByProgramm();// получаем имя как хеш от программы
}

// случайным образом изменяем одну команду
void ALiveObject::evolution()
{
  if (m_commandCountChangeable)
    {
      int changeShance = qrand() % (m_programm.size() + 1);
      if (changeShance == m_programm.size())
        {
          //добавляем или удаляем
          int increaseDecrease = qrand() % 2;
          if (increaseDecrease == 0)
            {
              //добавляем случайную команду
              int numCommand = qrand() % m_programm.size();
              ACommand command(m_programm.size(),m_commonSettings);
              m_programm.insert(numCommand,command);

              //правим все переходы если комманда добавлена не в конец
              if (numCommand != (m_programm.size() - 1))
                {
                  for (int i = 0;i < m_programm.size();i++)
                    {
                      ACommand command = m_programm[i];
                      if(!command.isAction())
                        {
                          if (command.m_commandName == commandName_ifJump)
                            {
                              quint16 address = command.m_command.ifjump.m_commandNum;
                              if ( (address >= numCommand))
                                command.m_command.ifjump.m_commandNum++;
                            }
                          if (command.m_commandName == commandName_Jump)
                            {
                              quint16 address = command.m_command.jump.m_commandNum;
                              if ( (address >= numCommand))
                                command.m_command.jump.m_commandNum++;
                            }
                          if (command.m_commandName == commandName_ifLengthJump)
                            {
                              quint16 address = command.m_command.ifLengthJump.m_commandNum;
                              if ( (address >= numCommand))
                                command.m_command.ifLengthJump.m_commandNum++;
                            }
                        }
                      m_programm[i] = command;
                    }
                }
            }
          else
            {
              //удаляем случайную команду
              int numCommand = qrand() % m_programm.size();
              m_programm.removeAt(numCommand);

              //правим все переходы
              for (int i = 0;i < m_programm.size();i++)
                {
                  ACommand command = m_programm[i];
                  if(!command.isAction())
                    {
                      if (command.m_commandName == commandName_ifJump)
                        {
                          quint16 address = command.m_command.ifjump.m_commandNum;
                          if ( (address > numCommand) || (address > m_programm.size() - 1))
                            command.m_command.ifjump.m_commandNum--;
                        }
                      if (command.m_commandName == commandName_Jump)
                        {
                          quint16 address = command.m_command.jump.m_commandNum;
                          if ( (address > numCommand) || (address > m_programm.size() - 1))
                            command.m_command.jump.m_commandNum--;
                        }
                      if (command.m_commandName == commandName_ifLengthJump)
                        {
                          quint16 address = command.m_command.ifLengthJump.m_commandNum;
                          if ( (address > numCommand) || (address > m_programm.size() - 1))
                            command.m_command.ifLengthJump.m_commandNum--;
                        }
                    }
                  m_programm[i] = command;
                }
            }
        }
      else
        {
          int numCommand = qrand() % m_programm.size();
          m_programm[numCommand].evolution(false,m_programm.count());
        }
    }
  else
    {
      int numCommand = qrand() % m_programm.size();
      m_programm[numCommand].evolution(false,m_programm.count());
    }
  m_henotipStabilityTime = 0;
  m_name = hashByProgramm();// получаем имя как хеш от программы
}

void ALiveObject::exec()
{
  if (m_programm.isEmpty())
    return;

  //исполняем программу до первого действия
  const int MAX_JUMP_COUNT = 500;
  int colJump = 0;
  int jumpCount = 1;
  ACommand command;
  do
    {
      command = m_programm[m_commandNumber];
      jumpCount = command.exec(this);
      m_lastExecutedCommandNumber = m_commandNumber;
      if(jumpCount == -1)
        m_commandNumber = (m_commandNumber + 1) % m_programm.count();
      else
        m_commandNumber = jumpCount;
      colJump++;
      if (colJump > MAX_JUMP_COUNT)//чтобы не зависнуть если вся программа будет состоять только из условий
        {
          die(); //убиваем себя
          break;
        }
    }
  while(!command.isAction());
}

ALiveObject *ALiveObject::clone(QObject *parent)
{
  //копируем данные
  const double CHANCE_EVOLUSHION = 0.3;
  ALiveObject *object = new ALiveObject(m_commonSettings, parent);
  object->m_programm = m_programm;
  object->m_startCommandCount = m_startCommandCount;
  object->m_commandCountChangeable = m_commandCountChangeable;
  //  object->m_energy = m_energy;
  object->m_iNuub = false;
  object->m_henotipStabilityTime = m_henotipStabilityTime + 1;
  object->m_name = m_name;

  //если надо эволюционируем
  double randVal = drand48();
  if (randVal < CHANCE_EVOLUSHION)
    {
      object->evolution();
    }

  return object;
}

int ALiveObject::x()
{
  return m_x;
}

void ALiveObject::setX(int x)
{
  m_x = x;
}

int ALiveObject::y()
{
  return m_y;
}

void ALiveObject::setY(int y)
{
  m_y = y;
}

void ALiveObject::setPos(int x, int y)
{
  m_x = x;
  m_y = y;
}

QPoint ALiveObject::pos()
{
  return QPoint(m_x,m_y);
}

qreal ALiveObject::posX()
{
  return m_x;
}

qreal ALiveObject::posY()
{
  return m_y;
}

void ALiveObject::eat(qreal energyCount)
{
  m_energy += energyCount;
}

void ALiveObject::die()
{
  m_energy = -1;
}

void ALiveObject::damage(qreal energyCount)
{
  m_armor -= energyCount;
}

qreal ALiveObject::hitPower()
{
  return m_hitPower;
}

bool ALiveObject::isDie()
{
  if (m_energy <= 0 || m_armor <= 0)
    return true;
  else
    return false;
}

ACommand ALiveObject::currCommand()
{
  return m_programm[m_commandNumber];
}

ACommand ALiveObject::lastCommand()
{
  return m_programm[m_lastExecutedCommandNumber];
}

qreal ALiveObject::energy()
{
  return m_energy;
}

bool ALiveObject::isNoob()
{
  return m_iNuub;
}

QString ALiveObject::name()
{
  return m_name;
}

int ALiveObject::stabilityTime()
{
  return m_henotipStabilityTime;
}

QString ALiveObject::hashByProgramm()
{
  QByteArray data;
  for (int i = 0;i < m_programm.count();i++)
    {
      data.append((char*)&m_programm[i],sizeof(ACommand));
    }
  return QString::number(qHash(data));
}

QList<ACommand> ALiveObject::programm()
{
  return m_programm;
}

void ALiveObject::setProgramm(QList<ACommand> programm)
{
  m_programm = programm;
}

bool ALiveObject::canReproduction()
{
  if (m_energy > 200)
    return true;
  else
    return false;
}

int ALiveObject::startCommandCount() const
{
  return m_startCommandCount;
}

void ALiveObject::setStartCommandCount(int startCommandCount)
{
  m_startCommandCount = startCommandCount;
}

bool ALiveObject::commandCountChangeable() const
{
  return m_commandCountChangeable;
}

void ALiveObject::setCommandCountChangeable(bool commandCountChangeable)
{
  m_commandCountChangeable = commandCountChangeable;
}

AObjectCommonSettings *ALiveObject::commonSettings() const
{
    return m_commonSettings;
}

void ALiveObject::setCommonSettings(AObjectCommonSettings *commonSettings)
{
  m_commonSettings = commonSettings;
  updateCommonSettings();
}

void ALiveObject::updateCommonSettings()
{
  for (int i = 0;i < m_programm.count();i++)
    {
      m_programm[i].setCommonSettings(m_commonSettings);
    }
}
