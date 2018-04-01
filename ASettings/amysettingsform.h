#ifndef AMYSETTINGSFORM_H
#define AMYSETTINGSFORM_H

#include <QWidget>
#include <QFileDialog>
#include "ASettings/amysettings.h"
#include "acommandssettingswidget.h"

namespace Ui {
  class AMySettingsForm;
}

class AMySettingsForm : public QWidget
{
  Q_OBJECT

public:
  explicit AMySettingsForm(AMySettings *settings, QWidget *parent = 0);
  ~AMySettingsForm();

private slots:
  void on_applyButton_clicked();

  void on_pushButton_clicked();

private:
  Ui::AMySettingsForm *ui;
  AMySettings *m_settings;
};

#endif // AMYSETTINGSFORM_H
