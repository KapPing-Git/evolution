#include "acommandssettingswidget.h"

ACommandsSettingsWidget::ACommandsSettingsWidget(QWidget *parent) : QWidget(parent)
{
  setLayout(&m_layout);
}

void ACommandsSettingsWidget::addCommandDescription(QString name, bool exist, int chance)
{
  ACommandSettingsForm *form = new ACommandSettingsForm(name,exist,chance,this);
  m_commands << form;
  m_layout.addWidget(form);
}

QString ACommandsSettingsWidget::name(int num)
{
  return m_commands[num]->name();
}

bool ACommandsSettingsWidget::exist(int num)
{
  return m_commands[num]->exist();
}

int ACommandsSettingsWidget::chance(int num)
{
  return m_commands[num]->chance();
}

int ACommandsSettingsWidget::count()
{
  return m_commands.count();
}
