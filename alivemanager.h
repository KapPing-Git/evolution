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
  bool liveObjectsIfLengthActive();
  void setLiveObjectsIfLengthActive(bool active);

  AObjectCommonSettings *liveObjectsCommonSettings();

signals:
  void nextGeneration();

public slots:

private:
  const int LIVE_OBJECT_START_COUNT = 200;
  const int LIVE_OBJECT_MIN_COUNT = 8;
  const int FOOD_COUNT = 100;
  const int TOXIN_COUNT = 10;
  const int WALL_COUNT = 10;
  ALiveArena *m_arena;
  QList<ALiveObject *> m_liveObjects;
  QList<ALiveObject *> m_lastSurvived;
  int m_timerId;

  int m_numGeneration;
  int m_numPlayedGeneration;
  int m_colPlayedGeneration;
  bool m_played;
  int m_maxLiveTime;
  int m_generationLiveTime;
  int m_lastGeneratiolLiveTime;

  AObjectCommonSettings m_liveObjectsCommonSettings;
  bool m_needStop;
  bool m_border; //края карты непрохродимы
  bool m_constFoodGeneration;// Необходимо ли постоянно генерировать еду
  float m_foodGenerationChance; // шанс генерации еды на каждом шаге
  bool m_realTimeReproduction;// размножение в реальном времени
  int m_liveObjectProgrammSize; // размер программы живого объекта
  bool m_liveObjectChangeableProgrammSize; //размер программы живого объекта может увеличиваться

  void toPlaceBorder();
  void toPlaceNoLiveObject(EObject typeObject,int objectCount,bool searchfreeCell = true);
  void setLiveObjectOnArena(ALiveObject * object);

  // QObject interface
protected:
  void timerEvent(QTimerEvent *event);
};

#endif // ALIVEMANAGER_H
