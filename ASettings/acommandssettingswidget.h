#ifndef ACOMMANDSSETTINGSWIDGET_H
#define ACOMMANDSSETTINGSWIDGET_H

#include <QWidget>
#include <QList>
#include <QVBoxLayout>

#include "acommandsettingsform.h"

class ACommandsSettingsWidget : public QWidget
{
  Q_OBJECT
public:
  explicit ACommandsSettingsWidget(QWidget *parent = nullptr);
  void addCommandDescription(QString name, bool exist, int chance);
  QString name(int num);
  bool exist(int num);
  int chance(int num);
  int count();

signals:

public slots:

private:
  QList<ACommandSettingsForm*> m_commands;
  QVBoxLayout m_layout;

};

#endif // ACOMMANDSSETTINGSWIDGET_H
