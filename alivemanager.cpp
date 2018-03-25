#include "alivemanager.h"

ALiveManager::ALiveManager(ALiveArena *arena, QObject *parent) : QObject(parent)
{
  m_arena = arena;
  m_liveObjectsCommonSettings.arena = m_arena;
  m_liveObjectsCommonSettings.generator = &m_commandGenerator;
  m_timerId = 0;
  m_numGeneration = 0;
  m_numPlayedGeneration = 0;
  m_colPlayedGeneration = 0;
  m_played = false;
  m_maxLiveTime = 0;
  m_generationLiveTime = 0;
  m_lastGeneratiolLiveTime = 0;
  m_needStop = false;
  m_border = false;
  m_constFoodGeneration = false;
  m_foodGenerationChance = 0;
  m_realTimeReproduction = false;
  m_liveObjectProgrammSize = 100;
  m_liveObjectChangeableProgrammSize = false;
  m_liveObjectStartCount = 200;
  m_liveObjectMinCount = 8;
  m_colChildrenForLastSurviveds = 25;
  m_foodCount = 1000;
  m_toxinCount = 100;
  m_wallCount = 100;
  m_nextSurvivedChildrenDecrease = 1;
}

void ALiveManager::createFirstGeneration()
{
  qsrand(QTime::currentTime().msecsSinceStartOfDay());// далее будут использоваться случайные числа
  m_arena->clear();
  if(m_border)
    toPlaceBorder();

  // размещаем живые объекты
  for (int i = 0; i < m_liveObjectStartCount;i++)
    {
      ALiveObject *liveObject = new ALiveObject(&m_liveObjectsCommonSettings,this);
      liveObject->setStartCommandCount(m_liveObjectProgrammSize);
      liveObject->setCommandCountChangeable(m_liveObjectChangeableProgrammSize);
      liveObject->createChanceProgramm();
      m_liveObjects << liveObject;
      setLiveObjectOnArena(liveObject);
    }
  // размещаем неживые объекты
  toPlaceNoLiveObject(object_food,m_foodCount);
  toPlaceNoLiveObject(object_toxin,m_toxinCount);
  toPlaceNoLiveObject(object_wall,m_wallCount);

}

//ПЕРЕДЕЛАТЬ кол-во копий объекта должно быть в соответствии с занимаемым местом, при смерти если осталось меньше 8 заполнять выживших
void ALiveManager::createNextGeneration()
{
  //если не было создано первое поколение значит ошибка прерываем функцию
  if (m_diedObjects.isEmpty())
    {
      createFirstGeneration();
      return;
    }

  m_arena->clear();
  if(m_border)
    toPlaceBorder();
//  QList<ALiveObject*> cloneObjects;
  // удаляем старые выжившие объекты
  for (int i = 0;i < m_prevDiedObjects.count();i++)
    {
      delete m_prevDiedObjects[i];
    }
  m_prevDiedObjects.clear();

//  //сохраняем выживших
//  m_prevDiedObjects = m_liveObjects;
//  for (int i = 0; i < m_liveObjects.count();i++)
//    cloneObjects << m_liveObjects[i];
//  m_liveObjects.clear();

  // клонируем объекты
  ////клонируем обьекты с постоянным количеством потомков
  int colCreatedObjects = 0;
  for (int numObject = 0;numObject < m_liveObjectMinCount;numObject++)
    {
      int parentIndex = (m_diedObjects.count() - 1) - numObject;
      ALiveObject *liveObject = m_diedObjects[parentIndex];
      for (int numChild = 0;numChild < m_colChildrenForLastSurviveds;numChild++)
        {
          addNewBornOrCreateObject(liveObject,20);
          colCreatedObjects++;
          if (colCreatedObjects >= m_liveObjectStartCount)
            break;
        }
      if (colCreatedObjects >= m_liveObjectStartCount)
        break;
    }

  ////клонируем обьекты с постоянно уменьшающимся количеством потомков
  int colChildrenForCurrObject = (m_liveObjectStartCount - colCreatedObjects) / m_nextSurvivedChildrenDecrease;
  int parentIndex = m_diedObjects.count() - colCreatedObjects - 1;
  while (colChildrenForCurrObject > 0)
    {
      if (parentIndex < 0)
        break;
      ALiveObject *liveObject = m_diedObjects[parentIndex];
      for (int i = 0;i < colChildrenForCurrObject;i++)
        {
          addNewBornOrCreateObject(liveObject,20);
          colCreatedObjects++;
          if (colCreatedObjects >= m_liveObjectStartCount)
            break;
        }
      if (colCreatedObjects >= m_liveObjectStartCount)
        break;
      colChildrenForCurrObject /= m_nextSurvivedChildrenDecrease;
      parentIndex--;
    }

  m_prevDiedObjects = m_diedObjects;
  m_diedObjects.clear();

  // размещаем неживые объекты
  toPlaceNoLiveObject(object_food,m_foodCount);
  toPlaceNoLiveObject(object_toxin,m_toxinCount);
  toPlaceNoLiveObject(object_wall,m_wallCount);
  m_numGeneration++;
  m_numPlayedGeneration++;

  if (m_generationLiveTime > m_maxLiveTime)
    m_maxLiveTime = m_generationLiveTime;
  emit nextGeneration();
  m_lastGeneratiolLiveTime = m_generationLiveTime;
  m_generationLiveTime = 0;
}

bool ALiveManager::exec()
{
  //если надо генерировать еду
  if (m_constFoodGeneration)
    {
      qreal randNum = drand48();
      if (m_foodGenerationChance > randNum)
        toPlaceNoLiveObject(object_food,3,false);
    }

  // все объекты выполняют по 1 действию
  for (int i = 0;i < m_liveObjects.count();i++)
    {
      m_liveObjects[i]->exec();
    }

//  //проверяем не умерли ли все одновременно
//  bool survivedExists = false;
//  for (int i = 0;i < m_liveObjects.count();i++)
//    {
//      ALiveObject *liveObject = m_liveObjects[i];
//      if (!liveObject->isDie())
//        {
//          survivedExists = true;
//          break;
//        }
//    }

  // удаляем умершие обьекты
  //  if (survivedExists)
  //    {
  for (int i = 0;i < m_liveObjects.count();i++)
    {
      ALiveObject *liveObject = m_liveObjects[i];
      if (liveObject->isDie())
        {
          m_arena->setObject(liveObject->x(),liveObject->y(),object_food,NULL);
          m_diedObjects << liveObject;
          m_liveObjects.removeAt(i);
          i--;
        }
    }
  //    }

  //если надо размножать объекты
  if (m_realTimeReproduction)
    {
      for (int i = 0;i < m_liveObjects.count();i++)
        {
          ALiveObject *liveObject = m_liveObjects[i];
          //если надо пробуем размножить объект
          if (liveObject->canReproduction())
            {
              //ищем место для нового объекта
              int posX = liveObject->posX();
              int posY = liveObject->posY();
              for (int x = posX - 1; x < (posX + 1);x++)
                {
                  for (int y = posY - 1; y < (posY + 1);y++)
                    {
                      // если точка внутри арены на арене
                      if ( (x >= 0) && (x <= m_arena->colCount()) &&
                           (y >= 0) && (y <= m_arena->rowCount()))
                        {
                          if (m_arena->typeObject(x,y) == object_none)
                            {
                              ALiveObject *newLiveObject = liveObject->born(this);
                              newLiveObject->setPos(x,y);
                              m_arena->setObject(x,y,object_liveObject,newLiveObject);
                              m_liveObjects << newLiveObject;
                              liveObject->eat(-100);
                              break;
                            }
                        }
                    }
                }
            }
        }
    }

  m_generationLiveTime++;
  //если количество объектов меньше минимального, то позвращаем false как признак невозможности дальнейшей жизнедеятельности
//  if ((m_liveObjects.count() <= m_liveObjectMinCount) or !survivedExists)
  if (m_liveObjects.count() <= m_liveObjectMinCount)
    {
      for (int i = 0;i < m_liveObjects.count();i++)
        m_diedObjects << m_liveObjects[i];
      m_liveObjects.clear();
      return false;
    }
  else
    return true;
  //    }
}

void ALiveManager::play(int generationCount)
{
  m_played = true;
  m_numPlayedGeneration = 0;
  m_colPlayedGeneration = generationCount;
  m_needStop = false;
  for ( int m_numPlayedGeneration = 0;m_numPlayedGeneration < generationCount;m_numPlayedGeneration++)
    {
      if(m_needStop)
        break;
      int execNum = 0;
      while(exec())
        {
          // контроллируем
          if (execNum > 4000)
            {
              execNum = 0;
              QApplication::processEvents();
            }
          if(m_needStop)
            break;
          execNum++;
        }
      createNextGeneration();
      QApplication::processEvents();
    }
  m_needStop = false;
  m_played = false;
}

void ALiveManager::stop()
{
  m_needStop = true;
}

void ALiveManager::autoPlay(int mSek)
{
  killTimer(m_timerId);
  m_timerId = startTimer(mSek);
}

void ALiveManager::stopAuto()
{
  killTimer(m_timerId);
}

int ALiveManager::numGeneration()
{
  return m_numGeneration;
}

int ALiveManager::numPlayedGeneration()
{
  return m_numPlayedGeneration;
}

int ALiveManager::colPlayedGeneration()
{
  return m_colPlayedGeneration;
}

bool ALiveManager::played()
{
  return m_played;
}

int ALiveManager::lastGenerationLiveTime()
{
  return m_lastGeneratiolLiveTime;
}

int ALiveManager::maxLiveTime()
{
  return m_maxLiveTime;
}

int ALiveManager::minLiveObjectsCount()
{
  return m_liveObjectMinCount;
}

QList<ALiveObject *> ALiveManager::lastSurvived()
{
  return m_prevDiedObjects;
}

void ALiveManager::setBeginObjects(QList<ALiveObject *> objects)
{
  //удаляем старые объекты если есть
  for (int i = 0;i < m_liveObjects.count();i++)
    {
      delete(m_liveObjects[i]);
    }

  //устанавливаем новые
  m_liveObjects = objects;
  createNextGeneration();
}

bool ALiveManager::border() const
{
  return m_border;
}

void ALiveManager::setBorder(bool border)
{
  if (m_border != border)
    {
      m_border = border;
      //      createNextGeneration();
    }
}

bool ALiveManager::constFoodGeneration() const
{
  return m_constFoodGeneration;
}

void ALiveManager::setPermanentFoodGeneration(bool constFoodGeneration)
{
  m_constFoodGeneration = constFoodGeneration;
}

float ALiveManager::foodGenerationChance() const
{
  return m_foodGenerationChance;
}

void ALiveManager::setFoodGenerationChance(float foodGenerationChance)
{
  m_foodGenerationChance = foodGenerationChance;
}

bool ALiveManager::realTimeReproduction() const
{
  return m_realTimeReproduction;
}

void ALiveManager::setRealTimeReproduction(bool realTimeReproduction)
{
  m_realTimeReproduction = realTimeReproduction;
}

int ALiveManager::liveObjectProgrammSize() const
{
  return m_liveObjectProgrammSize;
}

void ALiveManager::setLiveObjectProgrammSize(int liveObjectProgrammSize)
{
  m_liveObjectProgrammSize = liveObjectProgrammSize;
}

bool ALiveManager::liveObjectChangeableProgrammSize() const
{
  return m_liveObjectChangeableProgrammSize;
}

void ALiveManager::setLiveObjectChangeableProgrammSize(bool liveObjectChangeableProgrammSize)
{
  m_liveObjectChangeableProgrammSize = liveObjectChangeableProgrammSize;
}

ACommandGenerator *ALiveManager::commandGenerator()
{
  return &m_commandGenerator;
}

//bool ALiveManager::liveObjectsIfLengthActive()
//{
//  return m_liveObjectsCommonSettings.ifLengthActive;
//}

//void ALiveManager::setLiveObjectsIfLengthActive(bool active)
//{
//  m_liveObjectsCommonSettings.ifLengthActive = active;
//}

AObjectCommonSettings *ALiveManager::liveObjectsCommonSettings()
{
  return &m_liveObjectsCommonSettings;
}

int ALiveManager::liveObjectStartCount() const
{
  return m_liveObjectStartCount;
}

void ALiveManager::setLiveObjectStartCount(int liveObjectStartCount)
{
  m_liveObjectStartCount = liveObjectStartCount;
}

int ALiveManager::liveObjectMinCount() const
{
  return m_liveObjectMinCount;
}

void ALiveManager::setLiveObjectMinCount(int liveObjectMinCount)
{
  m_liveObjectMinCount = liveObjectMinCount;
}

int ALiveManager::foodCount() const
{
  return m_foodCount;
}

void ALiveManager::setFoodCount(int foodCount)
{
  m_foodCount = foodCount;
}

int ALiveManager::toxinCount() const
{
  return m_toxinCount;
}

void ALiveManager::setToxinCount(int toxinCount)
{
  m_toxinCount = toxinCount;
}

int ALiveManager::wallCount() const
{
  return m_wallCount;
}

void ALiveManager::setWallCount(int wallCount)
{
  m_wallCount = wallCount;
}

qreal ALiveManager::liveObjectChangeProgrammSizeChance() const
{
  return m_liveObjectsCommonSettings.changeProgrammSizeChance;
}

void ALiveManager::setLiveObjectChangeProgrammSizeChance(const qreal &liveObjectChangeProgrammSizeChance)
{
//  m_liveObjectChangeProgrammSizeChance = liveObjectChangeProgrammSizeChance;
  m_liveObjectsCommonSettings.changeProgrammSizeChance = liveObjectChangeProgrammSizeChance;
}

int ALiveManager::colChildrenForFirstSurvived() const
{
  return m_colChildrenForLastSurviveds;
}

void ALiveManager::setColChildrenForFirstSurvived(int colChildrenForFirstSurvived)
{
  m_colChildrenForLastSurviveds = colChildrenForFirstSurvived;
}

qreal ALiveManager::nextSurvivedChildrenDecrease() const
{
  return m_nextSurvivedChildrenDecrease;
}

void ALiveManager::setNextSurvivedChildrenDecrease(const qreal &nextSurvivedChildrenDecrease)
{
  m_nextSurvivedChildrenDecrease = nextSurvivedChildrenDecrease;
}

void ALiveManager::toPlaceBorder()
{
  for (int i = 0;i < m_arena->colCount();i++)
    {
      m_arena->setObject(i,0,object_wall,NULL);
      m_arena->setObject(i,m_arena->rowCount() - 1,object_wall,NULL);
    }
  for (int i = 0;i < m_arena->rowCount();i++)
    {
      m_arena->setObject(0,i,object_wall,NULL);
      m_arena->setObject(m_arena->colCount() - 1,i,object_wall,NULL);
    }
}

void ALiveManager::toPlaceNoLiveObject(EObject typeObject, int objectCount, bool searchfreeCell)
{
  for (int i = 0;i < objectCount;i++)
    {
      //пробуем разместить обьект на случайном месте, если оно занято, пробуем пока не найдём свободное
      bool placed = false;
      int tryingCount = 0;
      do
        {
          int x = rand() % m_arena->colCount();
          int y = rand() % m_arena->rowCount();
          if (m_arena->typeObject(x,y) == object_none)
            {
              m_arena->setObject(x,y,typeObject,NULL);
              placed = true;
            }
          if (tryingCount > 2)
            break;
          tryingCount++;
        }
      while(searchfreeCell && !placed);
    }
}

void ALiveManager::setLiveObjectOnArena(ALiveObject *object)
{
  int tryingCount = 0;
  while(true)
    {
      int x = rand() % (m_arena->colCount() - 1);
      int y = rand() % (m_arena->rowCount() - 1);
      if (m_arena->typeObject(x,y) == object_none)
        {
          object->setPos(x,y);
          m_arena->setObject(x,y,object_liveObject,object);
          break;
        }
      if (tryingCount > 10)
        break;
      tryingCount++;
    }
}

void ALiveManager::addNewBornOrCreateObject(ALiveObject *object, int chanceNewObject)
{
  //определённый процент от существ должен быть новым
  ALiveObject *newObject;
  int per = qrand() % 100;
  if (per >= chanceNewObject)
    newObject = object->born(this);
  else
    {
      newObject = new ALiveObject(&m_liveObjectsCommonSettings,this);
      newObject->setStartCommandCount(m_liveObjectProgrammSize);
      newObject->setCommandCountChangeable(m_liveObjectChangeableProgrammSize);
      newObject->createChanceProgramm();
    }
  m_liveObjects << newObject;
  setLiveObjectOnArena(newObject);
}

void ALiveManager::timerEvent(QTimerEvent *event)
{
  Q_UNUSED(event);
  if (!exec())
    createNextGeneration();
}
