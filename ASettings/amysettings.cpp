#include "amysettings.h"

AMySettings::AMySettings(QString fileName, QObject *parent) : QObject(parent)
{
  m_transaction = false;

  //создаём путь к файлу
  QFileInfo info(fileName);
  QDir dir;
  dir.mkpath(info.absolutePath());

  //загружаем данные
  m_settings = new QSettings(fileName,QSettings::IniFormat,parent);
  m_permanentFoodGeneration = m_settings->value("permanentFoodGeneration").toBool();
  m_foodGenerationChance = m_settings->value("foodGenerationChance").toReal();
  m_realTimeReproduction = m_settings->value("realTimeReproduction").toBool();
  m_arenaBorderExist = m_settings->value("arenaBorderExist").toBool();
  m_liveObjectProgrammSize = m_settings->value("liveObjectProgrammSize").toDouble();
  m_liveObjectChangeableProgrammSize = m_settings->value("liveObjectChangeableProgrammSize").toBool();
  m_liveObjectIfLengthActive = m_settings->value("liveObjectIfLengthActive").toBool();
}

AMySettings::~AMySettings()
{
  if (parent() == nullptr)
    m_settings->deleteLater();
}

void AMySettings::startTransaction()
{
  m_transaction = true;
}

void AMySettings::stopTransaction()
{
  m_transaction = false;
  emit changed();
}

bool AMySettings::permanentFoodGeneration() const
{
  return m_permanentFoodGeneration;
}

void AMySettings::setPermanentFoodGeneration(bool permanentFoodGeneration)
{
  m_permanentFoodGeneration = permanentFoodGeneration;
  m_settings->setValue("permanentFoodGeneration",permanentFoodGeneration);
  if (!m_transaction) emit changed();
}

float AMySettings::foodGenerationChance() const
{
  return m_foodGenerationChance;
}

void AMySettings::setFoodGenerationChance(float foodGenerationChance)
{
  m_foodGenerationChance = foodGenerationChance;
  m_settings->setValue("foodGenerationChance",foodGenerationChance);
  if (!m_transaction) emit changed();
}

bool AMySettings::realTimeReproduction() const
{
  return m_realTimeReproduction;
}

void AMySettings::setRealTimeReproduction(bool realTimeReproduction)
{
  m_realTimeReproduction = realTimeReproduction;
  m_settings->setValue("realTimeReproduction",realTimeReproduction);
  if (!m_transaction) emit changed();
}

bool AMySettings::arenaBorderExist() const
{
  return m_arenaBorderExist;
}

void AMySettings::setArenaBorderExist(bool arenaBorderExist)
{
  m_arenaBorderExist = arenaBorderExist;
  m_settings->setValue("arenaBorderExist",arenaBorderExist);
  if (!m_transaction) emit changed();
}

int AMySettings::liveObjectProgrammSize() const
{
  return m_liveObjectProgrammSize;
}

void AMySettings::setLiveObjectProgrammSize(const int &liveObjectProgrammSize)
{
  m_liveObjectProgrammSize = liveObjectProgrammSize;
  m_settings->setValue("liveObjectProgrammSize",liveObjectProgrammSize);
  if (!m_transaction) emit changed();
}

bool AMySettings::liveObjectChangeableProgrammSize() const
{
  return m_liveObjectChangeableProgrammSize;
}

void AMySettings::setLiveObjectChangeableProgrammSize(bool liveObjectChangeableProgrammSize)
{
  m_liveObjectChangeableProgrammSize = liveObjectChangeableProgrammSize;
  m_settings->setValue("liveObjectChangeableProgrammSize",liveObjectChangeableProgrammSize);
  if (!m_transaction) emit changed();
}

bool AMySettings::liveObjectIfLengthActive() const
{
  return m_liveObjectIfLengthActive;
}

void AMySettings::setLiveObjectIfLengthActive(bool liveObjectIfLengthActive)
{
  m_liveObjectIfLengthActive = liveObjectIfLengthActive;
  m_settings->setValue("liveObjectIfLengthActive",liveObjectIfLengthActive);
  if (!m_transaction) emit changed();
}
