#ifndef ACOMMAND_H
#define ACOMMAND_H

#include<QObject>
#include<QPoint>
#include<QDataStream>

#include "public.h"
#include "alivearena.h"
#include "aabstructliveobject.h"

struct AObjectCommonSettings
{
  ALiveArena *arena;
  bool ifLengthActive;
};

class AAbstructCommand
{
public:
  AAbstructCommand();
  virtual void evolution(int maxCommandCount) = 0;
  virtual void create(int maxCommandCount,AObjectCommonSettings *commonSettings) = 0; //Create а не конструктор, потому что иначе в union из классов такого типа может быть вызов всех конструкторов
  virtual unsigned int exec(AAbstructLiveObject *iAm) = 0; //возвращает количество команд на которое надо прыгнуть, или 1, если это действие и надо переместится только на 1 команду.
  virtual bool isAction() = 0;

protected:
  AObjectCommonSettings *m_commonSettings;
};

// если что то находится рядом то прыгаем на N шагов
class IfJump: public AAbstructCommand
{
public:
  ECource m_cource;
  EObject m_object;
  quint16 m_commandNum;
//  ALiveArena *m_arena;

  // ACommand interface
  void evolution(int maxCommandCount);
  void create(int maxCommandCount,AObjectCommonSettings *commonSettings);
  unsigned int exec(AAbstructLiveObject *iAm);
  virtual bool isAction();
};

// прыгаем на N шагов
class Jump: public AAbstructCommand
{
public:
  quint16 m_commandNum;

  // ACommand interface
  void evolution(int maxCommandCount);
  void create(int maxCommandCount,AObjectCommonSettings *commonSettings);
  unsigned int exec(AAbstructLiveObject *iAm);
  virtual bool isAction();
};

// Если что то находится на определённом расстоянии по любому из восми осей движения то прыгаем на N шагов
class IfLengthJump: public AAbstructCommand
{
public:
//  ALiveArena *m_arena;
  ECource m_cource;
  EObject m_object;
  int m_length;
  quint16 m_commandNum;

  // ACommand interface
  void evolution(int maxCommandCount);
  void create(int maxCommandCount,AObjectCommonSettings *commonSettings);
  unsigned int exec(AAbstructLiveObject *iAm);
  virtual bool isAction();
};

// делаем что то с рядом находящимся обьектом
class Action: public AAbstructCommand
{
public:
  ECource m_cource;
  EAction m_action;
//  ALiveArena *m_arena;

  // ACommand interface
  void evolution(int maxCommandCount);
  void create(int maxCommandCount,AObjectCommonSettings *commonSettings);
  unsigned int exec(AAbstructLiveObject *iAm);
  virtual bool isAction();
  EAction action();
  ECource course();
};

union AllCommand
{
  AllCommand () {}
  AllCommand (const AllCommand &command)
  {
    memcpy(this,&command,sizeof(AllCommand));
  }
  AllCommand* operator= (const AllCommand &command)
  {
    memcpy(this,&command,sizeof(AllCommand));
    return this;
  }

  friend QDataStream &operator<<(QDataStream &stream, const AllCommand &command)
  {
    stream.writeRawData((char*)&command,sizeof(AllCommand));
    return stream;
  }
  friend QDataStream &operator>>(QDataStream &stream, AllCommand &command)
  {
    stream.readRawData((char*)&command,sizeof(AllCommand));
    return stream;
  }

  IfJump ifjump;
  Jump jump;
  IfLengthJump ifLengthJump;
  Action action;
};

class ACommand
{
public:
  explicit ACommand(int maxCommandCount = 20, AObjectCommonSettings *commonSettings = nullptr);// = 20 ,nullptr
  ECommandName m_commandName;
  AllCommand m_command;
  void evolution(bool create, int maxCommandCount,bool onlyIf = false);
  int exec(AAbstructLiveObject *iAm);
  bool isAction();

  AObjectCommonSettings *commonSettings() const;
  void setCommonSettings(AObjectCommonSettings *commonSettings);

  friend QDataStream &operator<<(QDataStream &stream, const ACommand &command)
  {
    stream << (int)command.m_commandName << command.m_command;
    return stream;
  }
  friend QDataStream &operator>>(QDataStream &stream, ACommand &command)
  {
    int z;
    stream >> z >> command.m_command;
    command.m_commandName = (ECommandName)z;
    return stream;
  }



private:
  AObjectCommonSettings *m_commonSettings;

};

#endif // ACOMMAND_H
