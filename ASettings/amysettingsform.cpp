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
  ui->liveObjectsIfLengthCheckBox->setChecked(m_settings->liveObjectIfLengthActive());
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
  m_settings->setLiveObjectIfLengthActive(ui->liveObjectsIfLengthCheckBox->isChecked());

  m_settings->stopTransaction();
}
