#ifndef AMYSETTINGS_H
#define AMYSETTINGS_H

#include <QObject>
#include <QSettings>
#include <QDir>
#include <QFileInfo>

#include "acommand.h"

class AMySettings : public QObject
{
  Q_OBJECT
public:
  explicit AMySettings(QString fileName, ACommandGenerator *commandGenerator, QObject *parent = nullptr);
  ~AMySettings();

  void startTransaction();
  void stopTransaction();

  bool permanentFoodGeneration() const;
  void setPermanentFoodGeneration(bool permanentFoodGeneration);
  float foodGenerationChance() const;
  void setFoodGenerationChance(float foodGenerationChance);
  bool realTimeReproduction() const;
  void setRealTimeReproduction(bool realTimeReproduction);
  bool arenaBorderExist() const;
  void setArenaBorderExist(bool arenaBorderExist);
  int liveObjectProgrammSize() const;
  void setLiveObjectProgrammSize(const int &liveObjectProgrammSize);
  bool liveObjectChangeableProgrammSize() const;
  void setLiveObjectChangeableProgrammSize(bool liveObjectChangeableProgrammSize);
  int commandDescrCount();
  QString commandName(int num);
  bool commandExist(int num);
  int commandChance(int num);
  void setCommandDescr(int num, QString name,bool exist,int chance);

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

  float liveObjectChangeProgrammSizeChance() const;
  void setLiveObjectChangeProgrammSizeChance(float liveObjectChangeProgrammSizeChance);

signals:
  void changed();

public slots:

private:
  QSettings *m_settings;
  bool m_transaction;

  bool m_permanentFoodGeneration;
  float m_foodGenerationChance;
  bool m_realTimeReproduction;
  bool m_arenaBorderExist;
  int m_liveObjectProgrammSize;
  bool m_liveObjectChangeableProgrammSize;
  float m_liveObjectChangeProgrammSizeChance;
  QList<ACommandDescription> m_commandsDescription;
  int m_liveObjectStartCount;
  int m_liveObjectMinCount;

  int m_foodCount;
  int m_toxinCount;
  int m_wallCount;
};

#endif // AMYSETTINGS_H
