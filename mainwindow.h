#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDir>
#include <QFile>
#include <QDataStream>
#include <QFileDialog>

#include "AChart/achart.h"
#include "AChart/achartseries.h"
#include "aliveobject.h"
#include "alivemanager.h"
#include "ASettings/amysettings.h"
#include "ASettings/amysettingsform.h"

namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

private slots:
  void on_playButton_clicked();

  void on_startAutoButton_clicked();

  void on_stopAutoButton_clicked();

  void on_nextGeneration();

  void on_saveLiveObjectsAction_triggered();

  void on_loadLiveObjectsAction_triggered();

  void on_setBorderAction_triggered();

  void on_unsetBorderAction_triggered();

  void on_settingsChangeAction_triggered();

  void onSettingsChanged();

private:
  Ui::MainWindow *ui;
  ALiveArena m_arena;
  ALiveManager *m_liveManager;
  AChartSeries *m_liveTimeSeries;
  AMySettings *m_mySettings;

  void applySettings();
  void saveBests(QString fileName, ALiveManager *manager);
  void loadBests(QString fileName, ALiveManager *manager);
  inline QString settingsPath();
  inline QString settingsFileName();
};

#endif // MAINWINDOW_H
