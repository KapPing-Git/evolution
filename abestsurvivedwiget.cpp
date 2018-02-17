#include "abestsurvivedwiget.h"

ABestSurvivedWiget::ABestSurvivedWiget(QWidget *parent) : QWidget(parent)
{

  m_mainWidget = new QWidget();
  m_mainVBoxLayout = new QVBoxLayout(this);
  m_scroolArea = new QScrollArea();

  m_scroolArea->setWidget(m_mainWidget);

  m_scroolArea->setMinimumHeight(400);
  m_scroolArea->setMinimumWidth(270);
  m_scroolArea->setSizePolicy(QSizePolicy::Maximum,QSizePolicy::MinimumExpanding);
  m_mainVBoxLayout->addWidget(m_scroolArea);
}

void ABestSurvivedWiget::createComponents(int count)
{
  //удаляем старое
  delete m_mainWidget;
  m_names.clear();
  m_generationCounts.clear();
  m_sourceButtons.clear();

  //создаём новое содержание окна
  m_mainWidget = new QWidget();
  m_mainVBoxLayout->addWidget(m_mainWidget);
  QVBoxLayout *mainWidgetLayout = new QVBoxLayout(m_mainWidget);
  for (int i = 0;i < count;i++)
    {
      QHBoxLayout *hLayout = new QHBoxLayout();
      mainWidgetLayout->addLayout(hLayout);
      QLabel *name = new QLabel();
      QLabel *genCount = new QLabel();
      ASourceButton *sourceButton = new ASourceButton(i,tr("Исходный код"));
      connect(sourceButton,&QPushButton::pressed,this,&ABestSurvivedWiget::onSourceCodeButtonPressed);
      m_names << name;
      m_generationCounts << genCount;
      m_sourceButtons << sourceButton;
      hLayout->addWidget(name);
      hLayout->addWidget(genCount);
      hLayout->addWidget(sourceButton);
      name->setText("aaa");
      genCount->setText("bbb");
    }
  m_mainWidget->setMinimumHeight(200);
  m_mainWidget->setMinimumWidth(250);
  m_scroolArea->setWidget(m_mainWidget);

}

void ABestSurvivedWiget::addSurveved(QList<ALiveObject *> surviveds)
{
  m_bests.clear();
  for (int i = 0;i < surviveds.count();i++)
    {
      Survived sur;
      sur.object = surviveds[i];
      m_bests << sur;
    }
  qSort(m_bests);
  createComponents(surviveds.count());

  for (int i = m_bests.count() - 1; i >= 0 ;i--)
    {
      int num = (m_bests.count() - 1) - i;
      Survived sur = m_bests[i];
      m_names[num]->setText(sur.object->name());
      m_generationCounts[num]->setText(QString::number(sur.object->stabilityTime()));
    }
}

void ABestSurvivedWiget::onSourceCodeButtonPressed()
{
  ASourceButton *codeButton = (ASourceButton*)sender();
  int numObject = codeButton->numObject();
  ASourceCodeForm *codeForm = new ASourceCodeForm(this);
  codeForm->setWindowFlags(Qt::Window);
  codeForm->setProgramm(m_bests[numObject].object->programm());
  codeForm->show();
}

//void ABestSurvivedWiget::resizeEvent(QResizeEvent *event)
//{
//  Q_UNUSED(event);
////  m_scroolArea->setMaximumHeight(height() -15);
////  m_scroolArea->setMinimumHeight(height() -15);
//}

ASourceButton::ASourceButton(int numObject, QString text, QWidget *parent) : QPushButton(text,parent)
{
  m_numObject = numObject;
}

int ASourceButton::numObject() const
{
  return m_numObject;
}

void ASourceButton::setNumObject(int numObject)
{
  m_numObject = numObject;
}
