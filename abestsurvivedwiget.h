#ifndef ABESTSURVIVEDWIGET_H
#define ABESTSURVIVEDWIGET_H

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QHash>
#include <QList>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>

#include "aliveobject.h"
#include "asourcecodeform.h"

class ASourceButton : public QPushButton
{
public:
  explicit ASourceButton(int numObject = 0, QString text = "source", QWidget *parent = nullptr);
  int numObject() const;
  void setNumObject(int numObject);

private:
  int m_numObject;
};

class ABestSurvivedWiget : public QWidget
{
  Q_OBJECT
public:
  explicit ABestSurvivedWiget(QWidget *parent = nullptr);
  void addSurveved(QList<ALiveObject *> surviveds);

signals:

public slots:

private slots:
  void onSourceCodeButtonPressed();

//protected:
//  // QWidget interface
//  void resizeEvent(QResizeEvent *event);

private:
  void createComponents(int count);
  struct Survived
  {
    ALiveObject *object;
    friend bool operator<(const Survived &first, const Survived &second)
    {
      if (first.object->stabilityTime() < second.object->stabilityTime())
        return true;
      else
        return false;
    }
  };

  QVBoxLayout *m_mainMainLayout;
  QWidget *m_mainWidget;
  QScrollArea *m_scroolArea;
//  QVBoxLayout *m_scroolAreaLayout;
  QVBoxLayout *m_mainVBoxLayout;
  QList<QLabel*> m_names;
  QList<QLabel*> m_generationCounts;
  QList<ASourceButton*> m_sourceButtons;
  QList<Survived> m_bests;


};

#endif // ABESTSURVIVEDWIGET_H
