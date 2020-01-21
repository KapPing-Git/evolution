#include "acommandsettingsform.h"
#include "ui_acommandsettingsform.h"

//ACommandSettingsForm::ACommandSettingsForm(QWidget *parent) :
//  QWidget(parent),
//  ui(new Ui::ACommandSettingsForm)
//{
//  ui->setupUi(this);
//  ui->chanceEdit->setValidator(new QIntValidator(this));
//}

ACommandSettingsForm::ACommandSettingsForm(QString name, bool exist, int chance, QWidget *parent) :
  QWidget(parent),
  ui(new Ui::ACommandSettingsForm)
{
  ui->setupUi(this);
  setCommandDescription(name, exist, chance);
}

ACommandSettingsForm::~ACommandSettingsForm()
{
  delete ui;
}

bool ACommandSettingsForm::exist()
{
  return ui->existCheckBox->isChecked();
}

void ACommandSettingsForm::setExist(bool exist)
{
  ui->existCheckBox->setChecked(exist);
}

int ACommandSettingsForm::chance()
{
  return ui->chanceEdit->text().toInt();
}

void ACommandSettingsForm::setChance(int chance)
{
  ui->chanceEdit->setText(QString::number(chance));
}

QString ACommandSettingsForm::name()
{
  return ui->nameLabel->text();
}

void ACommandSettingsForm::setName(QString name)
{
  ui->nameLabel->setText(name);
}

void ACommandSettingsForm::setCommandDescription(QString name, bool exist, int chance)
{
  ui->nameLabel->setText(name);
  ui->existCheckBox->setChecked(exist);
  ui->chanceEdit->setText(QString::number(chance));
}
