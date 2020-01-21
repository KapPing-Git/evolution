#ifndef ALIVEMANAGER_H
#define ALIVEMANAGER_H

#include <QObject>
#include <QList>
#include <QApplication>

#include "public.h"
#include "aabstructliveobject.h"
#include "aliveobject.h"
#include "alivearena.h"
#include "acommand.h"


class ALiveManager : public QObject
{
  Q_OBJECT
public:
  explicit ALiveManager(ALiveArena *arena, QObject *parent = nullptr);
  void createFirstGeneration();
  void createNextGeneration();
  bool exec(); //возвращает false если осталось слишком мало живых
  void play(int generationCount);
  void stop();
  void autoPlay(int mSek);
  void stopAuto();
  int numGeneration();
  int numPlayedGeneration();
  int colPlayedGeneration();
  bool played();
  int lastGenerationLiveTime();
  int maxLiveTime();
  int minLiveObjectsCount();
  QList<ALiveObject *> lastSurvived();
  void setBeginObjects(QList<ALiveObject *> objects);
  bool border() const;
  void setBorder(bool border);


  bool constFoodGeneration() const;
  void setPermanentFoodGeneration(bool constFoodGeneration);
  float foodGenerationChance() const;
  void setFoodGenerationChance(float foodGenerationChance);
  bool realTimeReproduction() const;
  void setRealTimeReproduction(bool realTimeReproduction);
  int liveObjectProgrammSize() const;
  void setLiveObjectProgrammSize(int liveObjectProgrammSize);
  bool liveObjectChangeableProgrammSize() const;
  void setLiveObjectChangeableProgrammSize(bool liveObjectChangeableProgrammSize);
  ACommandGenerator *commandGenerator();
  AObjectCommonSettings *liveObjectsCommonSettings();
  int liveObjectStartCount() const;
  void setLiveObjectStartCount(int liveObjectStartCount);
  int liveObjectMinCount() const;
  void setLiveObjectMinCount(int liveObjectMinCount);
  int foodCount() const;
  void setFoodCount(int foodCount);
  int toxinCount() const;
  void setToxinCount(int toxinCount);
  int wallCount() const;
  void setWallCount(int wallCount);
  qreal liveObjectChangeProgrammSizeChance() const;
  void setLiveObjectChangeProgrammSizeChance(const qreal &liveObjectChangeProgrammSizeChance);
  int colChildrenForFirstSurvived() const;
  void setColChildrenForFirstSurvived(int colChildrenForFirstSurvived);
  qreal nextSurvivedChildrenDecrease() const;
  void setNextSurvivedChildrenDecrease(const qreal &nextSurvivedChildrenDecrease);

signals:
  void nextGeneration();

public slots:

private:
  int m_liveObjectStartCount;
  int m_liveObjectMinCount;
  int m_colChildrenForLastSurviveds;
  int m_foodCount;
  int m_toxinCount;
  int m_wallCount;
  qreal m_nextSurvivedChildrenDecrease;// во сколько раз каждый следующий выживший будет давать меньше потомства
//  EDefinitionMethodOfSurvived m_definitionMethodOfSurvived = definitionMethodOfSurvived_lastN;
  ALiveArena *m_arena;
  QList<ALiveObject *> m_liveObjects;
  QList<ALiveObject *> m_diedObjects;
  QList<ALiveObject *> m_prevDiedObjects;
  int m_timerId;

  int m_numGeneration;
  int m_numPlayedGeneration;
  int m_colPlayedGeneration;
  bool m_played;
  int m_maxLiveTime;
  int m_generationLiveTime;
  int m_lastGeneratiolLiveTime;

  AObjectCommonSettings m_liveObjectsCommonSettings;
  ACommandGenerator m_commandGenerator;
  bool m_needStop;
  bool m_border; //края карты непрохродимы
  bool m_constFoodGeneration;// Необходимо ли постоянно генерировать еду
  float m_foodGenerationChance; // шанс генерации еды на каждом шаге
  bool m_realTimeReproduction;// размножение в реальном времени
  int m_liveObjectProgrammSize; // размер программы живого объекта
  bool m_liveObjectChangeableProgrammSize; //размер программы живого объекта может увеличиваться
//  qreal m_liveObjectChangeProgrammSizeChance;

  void toPlaceBorder();
  void toPlaceNoLiveObject(EObject typeObject,int objectCount,bool searchfreeCell = true);
  void setLiveObjectOnArena(ALiveObject * object);
  void addNewBornOrCreateObject(ALiveObject *object,int chanceNewObject);

  // QObject interface
protected:
  void timerEvent(QTimerEvent *event);
};

#endif // ALIVEMANAGER_H
