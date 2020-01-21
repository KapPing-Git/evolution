#ifndef ACOMMAND_H
#define ACOMMAND_H

#include<QObject>
#include<QPoint>
#include<QDataStream>
#include<QHash>

#include "public.h"
#include "alivearena.h"
#include "aabstructliveobject.h"

class AAbstructLiveObject;

class AAbstructCommand
{
public:
  AAbstructCommand(int maxCommandCount = 0,ALiveArena *arena = nullptr);
  virtual ~AAbstructCommand();
  virtual void evolution(int maxCommandCount) = 0;
  virtual void create(int maxCommandCount,ALiveArena *arena) = 0;
  virtual unsigned int exec(AAbstructLiveObject *iAm) = 0; //возвращает номер команды на которую надо прыгнуть, или 1, если это действие и надо переместится на 1 команду.
  virtual bool isAction() = 0;
  virtual AAbstructCommand *createChanceCommand(int maxCommandCount, ALiveArena *arena) = 0;
  virtual AAbstructCommand *clone() = 0;
  virtual ECommandName name() = 0;
  virtual QString textDescription(const QHash<int,QString> &adressNames) = 0;

  friend QDataStream &operator<<(QDataStream &stream, AAbstructCommand &command)
  {command.toStream(stream);return stream;}
  friend QDataStream &operator>>(QDataStream &stream, AAbstructCommand &command)
  {command.fromStream(stream);return stream;}

  void setArena(ALiveArena *arena);

protected:
  virtual void toStream(QDataStream &stream) = 0;
  virtual void fromStream(QDataStream &stream) = 0;
  ALiveArena *m_arena;
};

struct ACommandDescription
{
  AAbstructCommand *command;
  QString name;
  ECommandName type;
  bool exist;
  int chance;
};

class ACommandGenerator
{
public:
  ACommandGenerator(int maxCommandCount = 0, ALiveArena *arena = nullptr);
  ~ACommandGenerator();
  AAbstructCommand *generate(int maxCommandCount, ALiveArena *arena);
  bool setCommandOptions(QString name, bool exist, int chance);
  bool setCommandOptions(ECommandName type, bool exist, int chance);
  QStringList commandNames() const;
  static AAbstructCommand *commandByType(ECommandName name,int maxCommandCount, ALiveArena *arena);
  bool isEmpty();

private:
  void addCommand(AAbstructCommand *command, QString name, ECommandName type, bool exist, int chance);
  void update();
  QList<ACommandDescription> m_commands;
  int m_sumChance;
  QStringList m_commandNames;
};

class AAbstructIfCommand : public AAbstructCommand
{
public:
  AAbstructIfCommand(int maxCommandCount = 0, ALiveArena *arena = nullptr);
  // AAbstructCommand interface
  bool isAction();

  quint16 commandNum() const;
  void setCommandNum(quint16 commandNum);

protected:
  quint16 m_commandNum;
};

class AAbstructActionCommand :  public AAbstructCommand
{
public:
  AAbstructActionCommand(int maxCommandCount = 0, ALiveArena *arena = nullptr);
  // AAbstructCommand interface
  bool isAction();
};

// если что то находится рядом то прыгаем на N шагов
class IfJump: public AAbstructIfCommand
{
public:
  IfJump(int maxCommandCount = 0, ALiveArena *arena = nullptr);

  ECource m_cource;
  EObject m_object;
//  quint16 m_commandNum;

  // AAbstructCommand interface
  void evolution(int maxCommandCount);
  void create(int maxCommandCount,ALiveArena *arena);
  unsigned int exec(AAbstructLiveObject *iAm);
  AAbstructCommand *createChanceCommand(int maxCommandCount, ALiveArena *arena);
  AAbstructCommand *clone();
  ECommandName name() {return commandName_ifJump;}
  QString textDescription(const QHash<int, QString> &adressNames);

protected:
  void toStream(QDataStream &stream);
  void fromStream(QDataStream &stream);
};

// прыгаем на N шагов
class Jump: public AAbstructIfCommand
{
public:
  Jump(int maxCommandCount = 0,ALiveArena *arena = nullptr);

//  quint16 m_commandNum;

  // AAbstructCommand interface
  void evolution(int maxCommandCount);
  void create(int maxCommandCount,ALiveArena *arena);
  unsigned int exec(AAbstructLiveObject *iAm);
  AAbstructCommand *createChanceCommand(int maxCommandCount, ALiveArena *arena);
  AAbstructCommand *clone();
  ECommandName name() {return commandName_Jump;}
  QString textDescription(const QHash<int, QString> &adressNames);

protected:
  void toStream(QDataStream &stream);
  void fromStream(QDataStream &stream);
};

// Если что то находится на определённом расстоянии по любому из восми осей движения то прыгаем на N шагов
class IfLengthJump: public AAbstructIfCommand
{
public:
  IfLengthJump(int maxCommandCount = 0, ALiveArena *arena = nullptr);

  ECource m_cource;
  EObject m_object;
  int m_length;
//  quint16 m_commandNum;

  // AAbstructCommand interface
  void evolution(int maxCommandCount);
  void create(int maxCommandCount,ALiveArena *arena);
  unsigned int exec(AAbstructLiveObject *iAm);
  AAbstructCommand *createChanceCommand(int maxCommandCount, ALiveArena *arena);
  AAbstructCommand *clone();
  ECommandName name() {return commandName_ifLengthJump;}
  QString textDescription(const QHash<int, QString> &adressNames);

  // AAbstructCommand interface
protected:
  void toStream(QDataStream &stream);
  void fromStream(QDataStream &stream);
};

// делаем что то с рядом находящимся обьектом
class Action: public AAbstructActionCommand
{
public:
  Action(int maxCommandCount = 0,ALiveArena *arena = nullptr);

  ECource m_cource;
  EAction m_action;

  EAction action();
  ECource course();

  // AAbstructCommand interface
  void evolution(int maxCommandCount);
  void create(int maxCommandCount, ALiveArena *arena);
  unsigned int exec(AAbstructLiveObject *iAm);
  AAbstructCommand *createChanceCommand(int maxCommandCount, ALiveArena *arena);
  AAbstructCommand *clone();
  ECommandName name() {return commandName_Action;}
  QString textDescription(const QHash<int, QString> &adressNames);

  // AAbstructCommand interface
protected:
  void toStream(QDataStream &stream);
  void fromStream(QDataStream &stream);
};

//union AllCommand
//{
//  AllCommand () {}
//  AllCommand (const AllCommand &command)
//  {
//    memcpy(this,&command,sizeof(AllCommand));
//  }
//  AllCommand* operator= (const AllCommand &command)
//  {
//    memcpy(this,&command,sizeof(AllCommand));
//    return this;
//  }

//  friend QDataStream &operator<<(QDataStream &stream, const AllCommand &command)
//  {
//    stream.writeRawData((char*)&command,sizeof(AllCommand));
//    return stream;
//  }
//  friend QDataStream &operator>>(QDataStream &stream, AllCommand &command)
//  {
//    stream.readRawData((char*)&command,sizeof(AllCommand));
//    return stream;
//  }

//  IfJump ifjump;
//  Jump jump;
//  IfLengthJump ifLengthJump;
//  Action action;
//};

//class ACommand
//{
//public:
//  explicit ACommand(int maxCommandCount = 20, AObjectCommonSettings *commonSettings = nullptr);// = 20 ,nullptr
//  ECommandName m_commandName;
//  AllCommand m_command;
//  void evolution(bool create, int maxCommandCount,bool onlyIf = false);
//  int exec(AAbstructLiveObject *iAm);
//  bool isAction();

//  AObjectCommonSettings *commonSettings() const;
//  void setCommonSettings(AObjectCommonSettings *commonSettings);

//  friend QDataStream &operator<<(QDataStream &stream, const ACommand &command)
//  {
//    stream << (int)command.m_commandName << command.m_command;
//    return stream;
//  }
//  friend QDataStream &operator>>(QDataStream &stream, ACommand &command)
//  {
//    int z;
//    stream >> z >> command.m_command;
//    command.m_commandName = (ECommandName)z;
//    return stream;
//  }



//private:
//  AObjectCommonSettings *m_commonSettings;

//};

#endif // ACOMMAND_H
