#ifndef ASOURCECODEFORM_H
#define ASOURCECODEFORM_H

#include <QWidget>
#include <QList>
#include <QHash>

#include "acommand.h"

namespace Ui {
  class ASourceCodeForm;
}

class ASourceCodeForm : public QWidget
{
  Q_OBJECT

public:
  explicit ASourceCodeForm(QWidget *parent = 0);
  ~ASourceCodeForm();
  void setProgramm(QList<ACommand> programm);

private:
  Ui::ASourceCodeForm *ui;
  QString metkaByNum(int num);
};

#endif // ASOURCECODEFORM_H
