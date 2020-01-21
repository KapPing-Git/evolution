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

ALiveObject::~ALiveObject()
{
  for (int i = 0; i < m_programm.count();i++)
    {
      delete m_programm[i];
    }
}

void ALiveObject::createChanceProgramm()
{
  if (m_commonSettings->generator->isEmpty())
    return;
  for (int i = 0;i < m_startCommandCount;i++)
    {
      AAbstructCommand *command = m_commonSettings->generateCommand(m_startCommandCount);
      m_programm << command;
    }
  m_iNuub = true;
  m_henotipStabilityTime = 0;
  m_name = hashByProgramm();// получаем имя как хеш от программы
}

// случайным образом изменяем одну команду
void ALiveObject::evolution()
{
  if (m_commonSettings->generator->isEmpty())
    return;
  if (m_commandCountChangeable)
    {
      qreal changeShance = dRandom(0,1);
      if (changeShance < m_commonSettings->changeProgrammSizeChance)
        {
          //добавляем или удаляем
          int increaseDecrease = qrand() % 2;
          if (increaseDecrease == 0)
            {
              //добавляем случайную команду
              int numAddCommand = qrand() % m_programm.size();
              AAbstructCommand *command = m_commonSettings->generateCommand(m_programm.size());
              m_programm.insert(numAddCommand,command);

              //правим все переходы если комманда добавлена не в конец
              if (numAddCommand != (m_programm.size() - 1))
                {
                  for (int i = 0;i < m_programm.size();i++)
                    {
                      AAbstructCommand *command = m_programm[i];
                      if(!command->isAction())
                        {
                          AAbstructIfCommand *ifCommand = (AAbstructIfCommand*)command;
                          quint16 address = ifCommand->commandNum();
                          if ( (address >= numAddCommand))
                            ifCommand->setCommandNum(address + 1);
                        }
                    }
                }
            }
          else
            {
              //удаляем случайную команду
              int numDelCommand = qrand() % m_programm.size();
              delete m_programm[numDelCommand];
              m_programm.removeAt(numDelCommand);

              //правим все переходы
              for (int i = 0;i < m_programm.size();i++)
                {
                  AAbstructCommand *command = m_programm[i];
                  if(!command->isAction())
                    {
                      AAbstructIfCommand *ifCommand = (AAbstructIfCommand*)command;
                      quint16 address = ifCommand->commandNum();
                      if ( (address > numDelCommand) || (address > m_programm.size() - 1))
                        ifCommand->setCommandNum(address - 1);
                    }
                }
            }
        }
      else
        {
          int numCommand = qrand() % m_programm.size();
          m_programm[numCommand]->evolution(m_programm.count());
        }
    }
  else
    {
      int numCommand = qrand() % m_programm.size();
      m_programm[numCommand]->evolution(m_programm.count());
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
  AAbstructCommand *command;
  do
    {
      command = m_programm[m_commandNumber];
      jumpCount = command->exec(this);
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
  while(!command->isAction());
}

ALiveObject *ALiveObject::born(QObject *parent)
{
  //копируем данные
//  const double CHANCE_EVOLUSHION = 0.3;
  ALiveObject *object = new ALiveObject(m_commonSettings, parent);
  for (int i = 0;i < m_programm.count();i++)
    {
      object->m_programm << m_programm[i]->clone();
    }
//  object->m_programm = m_programm;
  object->m_startCommandCount = m_startCommandCount;
  object->m_commandCountChangeable = m_commandCountChangeable;
  //  object->m_energy = m_energy;
  object->m_iNuub = false;
  object->m_henotipStabilityTime = m_henotipStabilityTime + 1;
  object->m_name = m_name;

  //если надо эволюционируем
  double randVal = dRandom(0,1);
  if (randVal < m_commonSettings->chanceEvolution)
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

AAbstructCommand *ALiveObject::currCommand()
{
  return m_programm[m_commandNumber];
}

AAbstructCommand *ALiveObject::lastCommand()
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
  QDataStream in(&data,QIODevice::WriteOnly);
  for (int i = 0;i < m_programm.count();i++)
    {
      in << *m_programm[i];
//      data.append((char*)m_programm[i],sizeof(ACommand));
    }
  return QString::number(qHash(data));
}

QList<AAbstructCommand *> ALiveObject::programm()
{
  return m_programm;
}

void ALiveObject::setProgramm(QList<AAbstructCommand *> programm)
{
  //удаляем старые команды
  for (int i = 0;i < m_programm.count();i++)
    delete m_programm[i];
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
      m_programm[i]->setArena(m_commonSettings->arena);
    }
}

//AAbstructCommand *ALiveObject::createCommand()
//{

//}
