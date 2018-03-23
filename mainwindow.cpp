#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "alivearena.h"

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  // создаём менеждер живых обьектов и связываем его и визуализатор арены с собственно ареной
  m_liveManager = new ALiveManager(&m_arena,this);
  //  m_liveManager->createFirstGeneration();
  connect(m_liveManager,&ALiveManager::nextGeneration,this,&MainWindow::on_nextGeneration);
  ui->liveArenaWidget->setArena(&m_arena);
  m_liveTimeSeries = new AChartSeries(this,"gr");
  ui->mainChart->addSeries(m_liveTimeSeries);

  //загружаем настройки из файлов настройки

  m_mySettings = new AMySettings(settingsFileName(),m_liveManager->commandGenerator(),this);
  connect(m_mySettings,&AMySettings::changed,this,&MainWindow::onSettingsChanged);
  applySettings();

  m_liveManager->createFirstGeneration();
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::on_playButton_clicked()
{
  ui->playButton->setEnabled(false);
  ui->liveArenaWidget->stopView();
  m_liveManager->play(ui->generationCountEdit->text().toInt());
  ui->liveArenaWidget->startView();
  ui->generationCountEdit->setText(QString::number(m_liveManager->colPlayedGeneration()));
  ui->playButton->setEnabled(true);
}

void MainWindow::on_startAutoButton_clicked()
{
  m_liveManager->stop();
  m_liveManager->autoPlay(ui->timeActionEdit->text().toInt());
}

void MainWindow::on_stopAutoButton_clicked()
{
  m_liveManager->stopAuto();
}

void MainWindow::on_nextGeneration()
{
  //если пропускаются поколения выводим сколько чего
  if (m_liveManager->played())
    {
      QString num = QString::number(m_liveManager->numPlayedGeneration());
      QString count = QString::number(m_liveManager->colPlayedGeneration());
      ui->generationCountEdit->setText(num + tr(" из ") + count);
    }
  QString numGeneration = QString::number(m_liveManager->numGeneration());
  QString liveTime = QString::number(m_liveManager->lastGenerationLiveTime());
  QString maxLiveTime = QString::number(m_liveManager->maxLiveTime());
  ui->numGenerationLabel->setText(tr("Поколение № ") + numGeneration + tr(" время жизни=") + liveTime + tr(" макс. время жизни=") + maxLiveTime);
  ui->bestSurvivedWidget->addSurveved(m_liveManager->lastSurvived());
  m_liveTimeSeries->addPoint(m_liveManager->numGeneration(),m_liveManager->lastGenerationLiveTime());
  //  saveBests("/home/kap/liveObjects/отладка/" + QString::number(m_liveManager->numGeneration()) + "_" +
  //            QString::number(m_liveManager->lastGenerationLiveTime()) + ".live",m_liveManager);
}

void MainWindow::saveBests(QString fileName, ALiveManager *manager)
{
  QList<ALiveObject*> survived = manager->lastSurvived();
  QFile file(fileName);
  if (file.open(QIODevice::WriteOnly))
    {
      QDataStream out(&file);
      out << survived.count();
      for (int i = 0;i < survived.count();i++)
        {
          out << *survived[i];
        }
    }
}

void MainWindow::loadBests(QString fileName, ALiveManager *manager)
{
  QList<ALiveObject*> liveObjects;
  QFile file(fileName);
  if (file.open(QIODevice::ReadOnly))
    {
      QDataStream in(&file);
      int count;
      in >> count;
      for (int i = 0;i < count;i++)
        {
          ALiveObject *liveObject = new ALiveObject(manager->liveObjectsCommonSettings(),manager);
          in >> *liveObject;
          liveObjects << liveObject;
        }
      m_liveManager->setBeginObjects(liveObjects);
    }
}

QString MainWindow::settingsPath()
{
  QString dirName;
#ifdef linux
  dirName = ".evolution";
#endif
#ifndef linux
  dirName = "evolution";
#endif
  return QDir::homePath() + QDir::separator() + dirName;
}

QString MainWindow::settingsFileName()
{
  return settingsPath() + QDir::separator() + "settings.ini";
}

void MainWindow::on_saveLiveObjectsAction_triggered()
{
  QFileDialog dialog(this,tr("Open live object"),QDir::homePath(), tr("live objects (*.live)"));
  dialog.setAcceptMode(QFileDialog::AcceptSave);
  if (dialog.exec())
    {
      QString fileName = dialog.selectedFiles().at(0);
      if (!fileName.contains(".live"))
        fileName += ".live";
      saveBests(fileName,m_liveManager);
    }
}

void MainWindow::on_loadLiveObjectsAction_triggered()
{
  QFileDialog dialog(this,tr("Open live object"), QDir::homePath() /*+ "/liveObjects/отладка/"*/, tr("live objects (*.live)"));
  dialog.setAcceptMode(QFileDialog::AcceptOpen);
  if (dialog.exec())
    {
      QString fileName = dialog.selectedFiles().at(0);
      loadBests(fileName,m_liveManager);
    }
}

void MainWindow::on_setBorderAction_triggered()
{
  m_liveManager->setBorder(true);
  m_liveManager->createNextGeneration();
}

void MainWindow::on_unsetBorderAction_triggered()
{
  m_liveManager->setBorder(false);
  m_liveManager->createNextGeneration();
}

void MainWindow::on_settingsChangeAction_triggered()
{
  AMySettingsForm *form = new AMySettingsForm(m_mySettings);
  form->show();
}

void MainWindow::onSettingsChanged()
{
  applySettings();
}

void MainWindow::applySettings()
{
  m_liveManager->setPermanentFoodGeneration(m_mySettings->permanentFoodGeneration());
  m_liveManager->setFoodGenerationChance(m_mySettings->foodGenerationChance());
  m_liveManager->setRealTimeReproduction(m_mySettings->realTimeReproduction());
  m_liveManager->setBorder(m_mySettings->arenaBorderExist());
  m_liveManager->setLiveObjectProgrammSize(m_mySettings->liveObjectProgrammSize());
  m_liveManager->setLiveObjectChangeableProgrammSize(m_mySettings->liveObjectChangeableProgrammSize());
  m_liveManager->setLiveObjectChangeProgrammSizeChance(m_mySettings->liveObjectChangeProgrammSizeChance());
  m_liveManager->setLiveObjectStartCount(m_mySettings->liveObjectStartCount());
  m_liveManager->setLiveObjectMinCount(m_mySettings->liveObjectMinCount());
  m_liveManager->setFoodCount(m_mySettings->foodCount());
  m_liveManager->setToxinCount(m_mySettings->toxinCount());
  m_liveManager->setWallCount(m_mySettings->wallCount());
  for ( int i = 0; i < m_mySettings->commandDescrCount();i++)
    m_liveManager->commandGenerator()->setCommandOptions(m_mySettings->commandName(i),m_mySettings->commandExist(i),m_mySettings->commandChance(i));
}
