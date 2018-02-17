#ifndef AMYSETTINGS_H
#define AMYSETTINGS_H

#include <QObject>
#include <QSettings>
#include <QDir>
#include <QFileInfo>

class AMySettings : public QObject
{
  Q_OBJECT
public:
  explicit AMySettings(QString fileName, QObject *parent = nullptr);
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

  bool liveObjectIfLengthActive() const;
  void setLiveObjectIfLengthActive(bool liveObjectIfLengthActive);

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
  bool m_liveObjectIfLengthActive;
};

#endif // AMYSETTINGS_H
