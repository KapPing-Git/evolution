#include "asourcecodeform.h"
#include "ui_asourcecodeform.h"

ASourceCodeForm::ASourceCodeForm(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::ASourceCodeForm)
{
  ui->setupUi(this);
}

ASourceCodeForm::~ASourceCodeForm()
{
  delete ui;
}

void ASourceCodeForm::setProgramm(QList<AAbstructCommand *> programm)
{
  //Находим все адреса в программе

  QList<int> adresses;
  for (int i = 0;i < programm.count();i++)
    {
      AAbstructCommand *command = programm[i];
      if (!command->isAction())
        {
          AAbstructIfCommand *ifCommand = (AAbstructIfCommand*)command;
          int adress = ifCommand->commandNum();
//          if (command.m_commandName == commandName_ifJump)
//            adress = command.m_command.ifjump.m_commandNum;
//          if (command.m_commandName == commandName_Jump)
//            adress = command.m_command.jump.m_commandNum;
//          if (command.m_commandName == commandName_ifLengthJump)
//            adress = command.m_command.ifLengthJump.m_commandNum;
          if (!adresses.contains(adress))
            {
              adresses << adress;
            }
        }
    }
  qSort(adresses);

  // создаём ассоциативный массив меток
  QHash<int,QString> adressNames;
  for (int i = 0;i < adresses.count();i++)
    {
      QString metka = metkaByNum(i);
      adressNames.insert(adresses[i],metka);
    }

  //Печатаем программу
  QTextEdit *textEdit = ui->textEdit;
  textEdit->clear();
  for (int i = 0;i < programm.count();i++)
    {
      //выводим метку если она есть
      if (adresses.contains(i))
        textEdit->append(adressNames[i] + "  ");
      else
        textEdit->append("         ");

      //выводим саму команду
      AAbstructCommand *command = programm[i];
      textEdit->insertPlainText(command->textDescription(adressNames));

//      if (command.m_commandName == commandName_ifJump)
//        {
//          //название комманды
//          textEdit->insertPlainText(tr("если "));

//          //направление
//          if (command.m_command.ifjump.m_cource == course_down)
//            textEdit->insertPlainText(tr("внизу "));
//          if (command.m_command.ifjump.m_cource == course_down_left)
//            textEdit->insertPlainText(tr("внизу слева "));
//          if (command.m_command.ifjump.m_cource == course_down_right)
//            textEdit->insertPlainText(tr("внизу справа "));
//          if (command.m_command.ifjump.m_cource == course_left)
//            textEdit->insertPlainText(tr("слева "));
//          if (command.m_command.ifjump.m_cource == course_right)
//            textEdit->insertPlainText(tr("справа "));
//          if (command.m_command.ifjump.m_cource == course_up)
//            textEdit->insertPlainText(tr("сверху "));
//          if (command.m_command.ifjump.m_cource == course_up_left)
//            textEdit->insertPlainText(tr("вверху слева "));
//          if (command.m_command.ifjump.m_cource == course_up_right)
//            textEdit->insertPlainText(tr("вверху справа "));

//          //объект
//          if (command.m_command.ifjump.m_object == object_food)
//            textEdit->insertPlainText(tr("еда "));
//          if (command.m_command.ifjump.m_object == object_liveObject)
//            textEdit->insertPlainText(tr("живой объект "));
//          if (command.m_command.ifjump.m_object == object_none)
//            textEdit->insertPlainText(tr("пустая клетка "));
//          if (command.m_command.ifjump.m_object == object_toxin)
//            textEdit->insertPlainText(tr("яд "));
//          if (command.m_command.ifjump.m_object == object_wall)
//            textEdit->insertPlainText(tr("стена "));

//          textEdit->insertPlainText(tr("то перейти к ") + adressNames[command.m_command.ifjump.m_commandNum]);
//        }

//      if (command.m_commandName == commandName_Jump)
//        {
//          //название комманды
//          textEdit->insertPlainText(tr("перейти к ") + adressNames[command.m_command.jump.m_commandNum]);
//        }

//      //выводим саму команду
//      if (command.m_commandName == commandName_ifLengthJump)
//        {
//          //название комманды
//          textEdit->insertPlainText(tr("если "));

//          //направление
//          if (command.m_command.ifLengthJump.m_cource == course_down)
//            textEdit->insertPlainText(tr("внизу "));
//          if (command.m_command.ifLengthJump.m_cource == course_down_left)
//            textEdit->insertPlainText(tr("внизу слева "));
//          if (command.m_command.ifLengthJump.m_cource == course_down_right)
//            textEdit->insertPlainText(tr("внизу справа "));
//          if (command.m_command.ifLengthJump.m_cource == course_left)
//            textEdit->insertPlainText(tr("слева "));
//          if (command.m_command.ifLengthJump.m_cource == course_right)
//            textEdit->insertPlainText(tr("справа "));
//          if (command.m_command.ifLengthJump.m_cource == course_up)
//            textEdit->insertPlainText(tr("сверху "));
//          if (command.m_command.ifLengthJump.m_cource == course_up_left)
//            textEdit->insertPlainText(tr("вверху слева "));
//          if (command.m_command.ifLengthJump.m_cource == course_up_right)
//            textEdit->insertPlainText(tr("вверху справа "));

//          //расстояние
//          textEdit->insertPlainText(tr("на расстоянии менее ") + QString::number(command.m_command.ifLengthJump.m_length) + tr(" клеток ") );

//          //объект
//          if (command.m_command.ifLengthJump.m_object == object_food)
//            textEdit->insertPlainText(tr("еда "));
//          if (command.m_command.ifLengthJump.m_object == object_liveObject)
//            textEdit->insertPlainText(tr("живой объект "));
//          if (command.m_command.ifLengthJump.m_object == object_none)
//            textEdit->insertPlainText(tr("пустая клетка "));
//          if (command.m_command.ifLengthJump.m_object == object_toxin)
//            textEdit->insertPlainText(tr("яд "));
//          if (command.m_command.ifLengthJump.m_object == object_wall)
//            textEdit->insertPlainText(tr("стена "));

//          textEdit->insertPlainText(tr("то перейти к ") + adressNames[command.m_command.ifLengthJump.m_commandNum]);
//        }


//      if (command.m_commandName == commandName_Action)
//        {
//          //название комманды
//          if (command.m_command.action.m_action == action_eat)
//            textEdit->insertPlainText(tr("съесть "));
//          if (command.m_command.action.m_action == action_hit)
//            textEdit->insertPlainText(tr("ударить "));
//          if (command.m_command.action.m_action == action_move)
//            textEdit->insertPlainText(tr("двигаться "));
//          if (command.m_command.action.m_action == action_shot)
//            textEdit->insertPlainText(tr("выстрелить "));
//          if (command.m_command.action.m_action == action_stop)
//            textEdit->insertPlainText(tr("стоять "));

//          //направление
//          if (command.m_command.action.m_action != action_stop)
//            {
//              if (command.m_command.ifjump.m_cource == course_down)
//                textEdit->insertPlainText(tr("вниз "));
//              if (command.m_command.ifjump.m_cource == course_down_left)
//                textEdit->insertPlainText(tr("вниз слева "));
//              if (command.m_command.ifjump.m_cource == course_down_right)
//                textEdit->insertPlainText(tr("вниз справа "));
//              if (command.m_command.ifjump.m_cource == course_left)
//                textEdit->insertPlainText(tr("влево "));
//              if (command.m_command.ifjump.m_cource == course_right)
//                textEdit->insertPlainText(tr("вправо "));
//              if (command.m_command.ifjump.m_cource == course_up)
//                textEdit->insertPlainText(tr("вверх "));
//              if (command.m_command.ifjump.m_cource == course_up_left)
//                textEdit->insertPlainText(tr("вверх слева "));
//              if (command.m_command.ifjump.m_cource == course_up_right)
//                textEdit->insertPlainText(tr("вверх справа "));
//            }
//        }
    }
}

QString ASourceCodeForm::metkaByNum(int num)
{
  return "m" + QString::number(num);
}
