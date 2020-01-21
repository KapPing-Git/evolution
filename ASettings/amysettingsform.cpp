#include "amysettingsform.h"
#include "ui_amysettingsform.h"

AMySettingsForm::AMySettingsForm(AMySettings *settings, QWidget *parent) :
  QWidget(parent),
  ui(new Ui::AMySettingsForm)
{
  ui->setupUi(this);
  m_settings = settings;
  ui->permanentFoodGenerationCheckBox->setChecked(m_settings->permanentFoodGeneration());
  ui->foodGenerationChanceEdit->setText(QString::number(m_settings->foodGenerationChance()));
  ui->realTimeReproductionCheckBox->setChecked(m_settings->realTimeReproduction());
  if (m_settings->arenaBorderExist())
    ui->closeWordRadioButton->setChecked(true);
  else
    ui->openWorldRadioButton->setChecked(true);
  ui->liveObjectProgrammSizeEdit->setText(QString::number(m_settings->liveObjectProgrammSize()));
  ui->liveObjectChangeableProgrammSizeCheckBox->setChecked(m_settings->liveObjectChangeableProgrammSize());
  ui->liveObjectChangeProgrammSizeChanceEdit->setText(QString::number(m_settings->liveObjectChangeProgrammSizeChance()));
  for (int i = 0;i < m_settings->commandDescrCount();i++)
    {
      ui->commandsListWidget->addCommandDescription(m_settings->commandName(i),m_settings->commandExist(i),m_settings->commandChance(i));
    }
  ui->liveObjectStartCountEdit->setText(QString::number(m_settings->liveObjectStartCount()));
  ui->liveObjectMinCountEdit->setText(QString::number(m_settings->liveObjectMinCount()));
  ui->foodCountEdit->setText(QString::number(m_settings->foodCount()));
  ui->toxinCountEdit->setText(QString::number(m_settings->toxinCount()));
  ui->wallCountEdit->setText(QString::number(m_settings->wallCount()));
  ui->colChildrenForLastSurvivedsEdit->setText(QString::number(m_settings->colChildrenForLastSurviveds()));
  ui->nextSurvivedChildrenDecreaseEdit->setText(QString::number(m_settings->nextSurvivedChildrenDecrease()));
  ui->autosaveBestGenerationsCheckBox->setChecked(m_settings->autosaveBestGenerations());
  ui->autosavePathEdit->setText(m_settings->autoSavePath());
}


AMySettingsForm::~AMySettingsForm()
{
  delete ui;
}

void AMySettingsForm::on_applyButton_clicked()
{
  m_settings->startTransaction();

  m_settings->setPermanentFoodGeneration(ui->permanentFoodGenerationCheckBox->isChecked());
  m_settings->setFoodGenerationChance(ui->foodGenerationChanceEdit->text().toFloat());
  m_settings->setRealTimeReproduction(ui->realTimeReproductionCheckBox->isChecked());
  if (ui->closeWordRadioButton->isChecked())
    m_settings->setArenaBorderExist(true);
  else
    m_settings->setArenaBorderExist(false);
  m_settings->setLiveObjectProgrammSize(ui->liveObjectProgrammSizeEdit->text().toDouble());
  m_settings->setLiveObjectChangeableProgrammSize(ui->liveObjectChangeableProgrammSizeCheckBox->isChecked());
  m_settings->setLiveObjectChangeProgrammSizeChance(ui->liveObjectChangeProgrammSizeChanceEdit->text().toFloat());
  for (int i = 0;i < m_settings->commandDescrCount();i++)
    {
      m_settings->setCommandDescr(i,ui->commandsListWidget->name(i),ui->commandsListWidget->exist(i),ui->commandsListWidget->chance(i));
    }
  m_settings->setLiveObjectStartCount(ui->liveObjectStartCountEdit->text().toInt());
  m_settings->setLiveObjectMinCount(ui->liveObjectMinCountEdit->text().toInt());
  m_settings->setFoodCount(ui->foodCountEdit->text().toInt());
  m_settings->setToxinCount(ui->toxinCountEdit->text().toInt());
  m_settings->setWallCount(ui->wallCountEdit->text().toInt());
  m_settings->setColChildrenForLastSurviveds(ui->colChildrenForLastSurvivedsEdit->text().toInt());
  m_settings->setNextSurvivedChildrenDecrease(ui->nextSurvivedChildrenDecreaseEdit->text().toDouble());
  m_settings->setAutosaveBestGenerations(ui->autosaveBestGenerationsCheckBox->isChecked());
  m_settings->setAutoSavePath(ui->autosavePathEdit->text());

  m_settings->stopTransaction();
}

void AMySettingsForm::on_pushButton_clicked()
{
  QFileDialog dirSelectDialod;
  dirSelectDialod.setFileMode(QFileDialog::DirectoryOnly);
  dirSelectDialod.exec();
  QStringList dirs=dirSelectDialod.selectedFiles();
  if (dirs.count()==1)
    {
      ui->autosavePathEdit->setText(dirs.at(0));
    }
}
