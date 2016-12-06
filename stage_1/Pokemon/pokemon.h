#ifndef POKEMON_H
#define POKEMON_H

#include <QMainWindow>
#include <QObject>
#include <QWidget>

#include <iostream>
#include <string>
#include <stdlib.h>
#include <time.h>
#include<QTime>

#define MAX_LEVEL 15

enum KIND//四种类型
{
    HIGH_ATTACK,HIGH_BLOOD,HIGH_DEFENSE,HIGH_SPEED
};

const QList<QString> POKEMONNAME={
    "Feraligatr",//大力鳄
    "Charizard",//喷火龙
    "Noivern",//音波龙
    "Bagon",//宝贝龙
    "Glaceon",//冰伊布
    "Pikachu",//皮卡丘
    "Wynaut",//小果然
    "Doduo",//嘟嘟
};//pokemon名字

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

const QList<QString> KINDNAME={"HIGH ATTACK","HIGH BLOOD","HIGH DEFENSE","HIGH SPEED"};//类型名称

const QList<QString> ATTACKKIND={"HydroCannon", "Cut", "LeechLife", "FocusEnergy",
                                 "Barrier", "LightScreen", "Counter", "TriAttack",
                                 "Attack"} ;//技能名称

const QList<QString> ATTRKIND={"S","SS","SSS"};//属性名称

enum QUALIFICATION{
    S,SS,SSS
};

//const uint R=5;
//const uint SR=6;
//const uint SSR=7;
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

    virtual uint Attack(){return 0;}//精灵技能
    void ExperienceUp(uint value);
    virtual void LevelUP(){}

    QString getName();
    uint getLevel();
    QString getInformation();
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
    SKILL skill;       //技能

    void setValues(uint baseAttack, uint baseBlood, uint baseDefense, uint baseSpeed);

signals:

public slots:
};

#endif // POKEMON_H
