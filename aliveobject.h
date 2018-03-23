#ifndef ALIVEOBJECT_H
#define ALIVEOBJECT_H

#include <QObject>
#include <QTime>
#include <QHash>

#include "aabstructliveobject.h"
#include "alivearena.h"
#include "public.h"
#include "acommand.h"

struct AObjectCommonSettings
{
  ALiveArena *arena;
  ACommandGenerator *generator;
  double chanceEvolution = 0.3;
  qreal changeProgrammSizeChance;
  AAbstructCommand *generateCommand(int maxCommandCount)
  {return generator->generate(maxCommandCount,arena);}
};

class AAbstructCommand;

class ALiveObject : public AAbstructLiveObject
{
  Q_OBJECT
public:
  explicit ALiveObject(AObjectCommonSettings *commonSettings, QObject *parent = 0);
  ~ALiveObject();
  void createChanceProgramm();
  void evolution();
  void exec();
  ALiveObject *born(QObject *parent = 0);
  int x();
  void setX(int x);
  int y();
  void setY(int y);
  virtual void setPos(int x, int y);
  virtual QPoint pos();
  virtual qreal posX();
  virtual qreal posY();
  virtual void eat(qreal energyCount);
  virtual void die();
  virtual void damage(qreal energyCount);
  virtual qreal hitPower();
  virtual bool isDie();
  AAbstructCommand* currCommand();
  AAbstructCommand* lastCommand();
  qreal energy();
  bool isNoob();
  QString name();
  int stabilityTime();
  QString hashByProgramm();
  QList<AAbstructCommand*> programm();
  void setProgramm(QList<AAbstructCommand*> programm);
  bool canReproduction();
  int startCommandCount() const;
  void setStartCommandCount(int startCommandCount);
  bool commandCountChangeable() const;
  void setCommandCountChangeable(bool commandCountChangeable);

  friend QDataStream &operator<<(QDataStream &stream, const ALiveObject &object)
  {
    stream << object.m_startCommandCount << object.m_commandCountChangeable << object.m_commandNumber << object.m_energy << object.m_armor << object.m_hitPower <<
              object.m_x << object.m_y << object.m_iNuub << object.m_henotipStabilityTime << object.m_name << object.m_programm.count();
    for (int i = 0;i < object.m_programm.count();i++)
      {
        stream << (quint8)object.m_programm[i]->name() << *object.m_programm[i];
      }
    return stream;
  }
  friend QDataStream &operator>>(QDataStream &stream, ALiveObject &object)
  {
    int programmSize = 0;
    stream >> object.m_startCommandCount >> object.m_commandCountChangeable >> object.m_commandNumber >> object.m_energy >> object.m_armor >> object.m_hitPower >>
        object.m_x >> object.m_y >> object.m_iNuub >> object.m_henotipStabilityTime >> object.m_name >> programmSize;
    for (int i = 0; i < programmSize;i++)
      {
        quint8 type;
        stream >> type;
        AAbstructCommand *command = ACommandGenerator::commandByType((ECommandName)type,programmSize,object.m_commonSettings->arena);
        stream >> *command;
        object.m_programm << command;
      }
//    object.updateCommonSettings();
    return stream;
  }

  AObjectCommonSettings *commonSettings() const;
  void setCommonSettings(AObjectCommonSettings *commonSettings);
//  void updateCommonSettings();

signals:

public slots:

private:
  void updateCommonSettings();
//  AAbstructCommand *createCommand();

  bool m_commandCountChangeable;
  //  bool m_ifLengthActive;
  int m_startCommandCount = 100;
  const qreal BEGIN_ENERGY = 100;
  const qreal BEGIN_ARMOR = 100;
  const qreal HIT_POWER = 100;
  QList<AAbstructCommand*> m_programm;
  int m_commandNumber;
  int m_lastExecutedCommandNumber;

//  ALiveArena *m_arena;
  AObjectCommonSettings *m_commonSettings;
  qreal m_energy;
  qreal m_armor;
  qreal m_hitPower;
  int m_x;
  int m_y;

  bool m_iNuub;
  int m_henotipStabilityTime;
  QString m_name;
};

#endif // ALIVEOBJECT_H
