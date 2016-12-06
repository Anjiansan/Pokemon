#ifndef POKEMON_H
#define POKEMON_H

#include <QMainWindow>
#include <QObject>
#include <QWidget>

//#include <iostream>
//#include <string>
//#include <stdlib.h>
#include <time.h>
#include <QTime>
#include <QDataStream>

#define MAX_LEVEL 15

enum KIND//四种类型
{
    HIGH_ATTACK,HIGH_BLOOD,HIGH_DEFENSE,HIGH_SPEED
};

enum SKILL//技能
{
    HydroCannon,//加农水炮
    Cut,//居合斩
    LeechLife,//吸血
    FocusEnergy,//聚气
    Barrier,//屏障
    LightScreen,//光墙
    Counter,//双倍奉还
    TriAttack,//三重攻击
    NormalAttack,//普通攻击
};

const QList<QString> ATTACKKIND={
    "HydroCannon", "Cut", "LeechLife", "FocusEnergy","Barrier", "LightScreen",
    "Counter","TriAttack","Attack"} ;//技能名称

const QList<QString> ATTRKIND={
    "S","SS","SSS"//属性名称
};

const QList<QString> KINDNAME={
    "HIGH_ATTACK","HIGH_BLOOD","HIGH_DEFENSE","HIGH_SPEED"//类型名称
};

enum QUALIFICATION
{
    S,SS,SSS
};

struct UDPPkm
{
    QString name;     //名字
    uint level;       //等级
    uint experience;  //经验
    uint attack;      //攻击属性
    uint blood;       //生命属性
    uint currentBlood;//实时血量，对决时使用
    uint defense;     //防御属性
    uint speed;       //敏捷属性
    uint kind;        //种类
    uint attr;        //稀有度
    uint skill;       //技能

    //友元声明，一对用于支持QDataStream 输入输出的函数
    friend QDataStream &operator<<(QDataStream &stream, const UDPPkm &pkm);
    friend QDataStream &operator>>(QDataStream &stream, UDPPkm &pkm);
};

const uint MAINR=100;
const uint MAINSR=150;
const uint MAINSSR=200;
const uint BASE=50;
const uint MINORBASE=25;
const uint MINORSR=100;
const uint MINORSSR=150;

class Pokemon : public QObject
{
    Q_OBJECT
public:
//    explicit Pokemon(QObject *parent = 0);
    Pokemon():level(1),experience(0){}
    virtual ~Pokemon(){}

    virtual uint Attack(){return 0;}//攻击函数
    void ExperienceUp(uint value);
    virtual void LevelUP(){}

    QString getName();
    uint getLevel();
    QString getInformation();
    UDPPkm* getAttr();
protected:
    QString name;     //名字
    uint level;       //等级
    uint experience;  //经验
    uint attack;      //攻击属性
    uint blood;       //生命属性
    uint currentBlood;//实时血量，对决时使用
    uint defense;     //防御属性
    uint speed;       //敏捷属性
    uint kind;        //种类
    uint attr;        //稀有度
    uint skill;       //技能

    void setValues(uint baseAttack, uint baseBlood, uint baseDefense, uint baseSpeed);

signals:

public slots:
};

void HydroCannonAttack(UDPPkm *self,UDPPkm *opponent);//加农水炮技能

void CutAttack(UDPPkm *self,UDPPkm *opponent);//居合斩

void LeechLifeAttack(UDPPkm *self,UDPPkm *opponent);//吸血

void FocusEnergyAttack(UDPPkm *self,UDPPkm *opponent);//聚气

void BarrierAttack(UDPPkm *self,UDPPkm *opponent);//屏障

void LightScreenAttack(UDPPkm *self,UDPPkm *opponent);//光墙

void CounterAttack(UDPPkm *self,UDPPkm *opponent);//双倍奉还

void TriAttackAttack(UDPPkm *self,UDPPkm *opponent);//三重攻击

void NmlAttack(UDPPkm *self,UDPPkm *opponent);//普通攻击

#endif // POKEMON_H
