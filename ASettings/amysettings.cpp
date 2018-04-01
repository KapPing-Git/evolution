#include "amysettings.h"

AMySettings::AMySettings(QString fileName, ACommandGenerator *commandGenerator, QObject *parent) : QObject(parent)
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
  m_liveObjectChangeProgrammSizeChance = m_settings->value("liveObjectChangeProgrammSizeChance",0.1).toFloat();
  m_liveObjectStartCount = m_settings->value("liveObjectStartCount",200).toInt();
  m_liveObjectMinCount = m_settings->value("liveObjectMinCount",8).toInt();
  m_foodCount = m_settings->value("foodCount",1000).toInt();
  m_toxinCount = m_settings->value("toxinCount",100).toInt();
  m_wallCount = m_settings->value("wallCount",100).toInt();
  m_colChildrenForLastSurviveds = m_settings->value("colChildrenForLastSurviveds",25).toInt();
  m_nextSurvivedChildrenDecrease = m_settings->value("nextSurvivedChildrenDecrease",1).toDouble();
  m_autosaveBestGenerations = m_settings->value("autosaveBestGenerations",false).toBool();
  m_autoSavePath = m_settings->value("autoSavePath",QDir::homePath() + QDir::separator() + "evolution" + QDir::separator() + "autosave").toString();
  ////загружаем данные о списке комманд
  QStringList commandsNames = commandGenerator->commandNames();
  m_settings->beginGroup("commandList");
  for (int i = 0;i < commandsNames.count();i++)
    {
      ACommandDescription descr;
      descr.name = commandsNames[i];
      m_settings->beginGroup(commandsNames[i]);
      descr.exist = m_settings->value("exist").toBool();
      descr.chance = m_settings->value("chance").toInt();
      m_settings->endGroup();
      m_commandsDescription << descr;
    }
  m_settings->endGroup();
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

int AMySettings::commandDescrCount()
{
  return m_commandsDescription.count();
}

QString AMySettings::commandName(int num)
{
  return m_commandsDescription[num].name;
}

bool AMySettings::commandExist(int num)
{
  return m_commandsDescription[num].exist;
}

int AMySettings::commandChance(int num)
{
  return m_commandsDescription[num].chance;
}

void AMySettings::setCommandDescr(int num, QString name, bool exist, int chance)
{
  m_commandsDescription[num].name = name;
  m_commandsDescription[num].exist = exist;
  m_commandsDescription[num].chance = chance;
  m_settings->beginGroup("commandList");
  m_settings->beginGroup(name);
  m_settings->setValue("exist",exist);
  m_settings->setValue("chance",chance);
  m_settings->endGroup();
  m_settings->endGroup();
}

int AMySettings::liveObjectStartCount() const
{
  return m_liveObjectStartCount;
}

void AMySettings::setLiveObjectStartCount(int liveObjectStartCount)
{
  m_liveObjectStartCount = liveObjectStartCount;
  m_settings->setValue("liveObjectStartCount",liveObjectStartCount);
  if (!m_transaction) emit changed();
}

int AMySettings::liveObjectMinCount() const
{
  return m_liveObjectMinCount;
}

void AMySettings::setLiveObjectMinCount(int liveObjectMinCount)
{
  m_liveObjectMinCount = liveObjectMinCount;
  m_settings->setValue("liveObjectMinCount",liveObjectMinCount);
  if (!m_transaction) emit changed();
}

int AMySettings::foodCount() const
{
  return m_foodCount;
}

void AMySettings::setFoodCount(int foodCount)
{
  m_foodCount = foodCount;
  m_settings->setValue("foodCount",foodCount);
  if (!m_transaction) emit changed();
}

int AMySettings::toxinCount() const
{
  return m_toxinCount;
}

void AMySettings::setToxinCount(int toxinCount)
{
  m_toxinCount = toxinCount;
  m_settings->setValue("toxinCount",toxinCount);
  if (!m_transaction) emit changed();
}

int AMySettings::wallCount() const
{
  return m_wallCount;
}

void AMySettings::setWallCount(int wallCount)
{
  m_wallCount = wallCount;
  m_settings->setValue("wallCount",wallCount);
  if (!m_transaction) emit changed();
}

float AMySettings::liveObjectChangeProgrammSizeChance() const
{
  return m_liveObjectChangeProgrammSizeChance;
}

void AMySettings::setLiveObjectChangeProgrammSizeChance(float liveObjectChangeProgrammSizeChance)
{
  m_liveObjectChangeProgrammSizeChance = liveObjectChangeProgrammSizeChance;
  m_settings->setValue("liveObjectChangeProgrammSizeChance",liveObjectChangeProgrammSizeChance);
  if (!m_transaction) emit changed();
}

int AMySettings::colChildrenForLastSurviveds() const
{
  return m_colChildrenForLastSurviveds;
}

void AMySettings::setColChildrenForLastSurviveds(int colChildrenForLastSurviveds)
{
  m_colChildrenForLastSurviveds = colChildrenForLastSurviveds;
  m_settings->setValue("colChildrenForLastSurviveds",colChildrenForLastSurviveds);
  if (!m_transaction) emit changed();
}

int AMySettings::nextSurvivedChildrenDecrease() const
{
  return m_nextSurvivedChildrenDecrease;
}

void AMySettings::setNextSurvivedChildrenDecrease(qreal nextSurvivedChildrenDecrease)
{
  m_nextSurvivedChildrenDecrease = nextSurvivedChildrenDecrease;
  m_settings->setValue("nextSurvivedChildrenDecrease",nextSurvivedChildrenDecrease);
  if (!m_transaction) emit changed();
}

bool AMySettings::autosaveBestGenerations() const
{
  return m_autosaveBestGenerations;
}

void AMySettings::setAutosaveBestGenerations(bool autosaveBestGenerations)
{
  m_autosaveBestGenerations = autosaveBestGenerations;
  m_settings->setValue("autosaveBestGenerations",autosaveBestGenerations);
  if (!m_transaction) emit changed();
}

QString AMySettings::autoSavePath() const
{
  return m_autoSavePath;
}

void AMySettings::setAutoSavePath(const QString &autoSavePath)
{
  m_autoSavePath = autoSavePath;
  m_settings->setValue("autoSavePath",autoSavePath);
  if (!m_transaction) emit changed();
}
