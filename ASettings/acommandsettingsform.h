#ifndef ACOMMANDSETTINGSFORM_H
#define ACOMMANDSETTINGSFORM_H

#include <QWidget>
#include <QIntValidator>

namespace Ui {
  class ACommandSettingsForm;
}

class ACommandSettingsForm : public QWidget
{
  Q_OBJECT

public:
  explicit ACommandSettingsForm(QString name = "",bool exist = false,int chance = 0, QWidget *parent = 0);
  ~ACommandSettingsForm();
  bool exist();
  void setExist(bool exist);
  int chance();
  void setChance(int chance);
  QString name();
  void setName(QString name);
  void setCommandDescription(QString name,bool exist,int chance);

private:
  Ui::ACommandSettingsForm *ui;
};

#endif // ACOMMANDSETTINGSFORM_H
