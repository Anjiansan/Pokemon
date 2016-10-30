#include "highdefense.h"

HighDefense::HighDefense(uint qualification,uint skillIndex)
{
    this->name="HIGH_DEFENSE";
    this->kind=HIGH_DEFENSE;
    this->attr=qualification;
    this->skill=skillIndex;

    srand((unsigned)time(NULL));
    switch (qualification)//资质越高，属性越高
    {
        case S:
            this->setValues(qrand()%BASE+BASE,qrand()%BASE+BASE,qrand()%BASE+MAINR,qrand()%BASE+BASE);
            break;
        case SS:
            this->setValues(qrand()%MINORBASE+MINORSR,qrand()%MINORBASE+MINORSR,qrand()%BASE+MAINSR,qrand()%MINORBASE+MINORSR);
            break;
        case SSS:
            this->setValues(qrand()%MINORBASE+MINORSSR,qrand()%MINORBASE+MINORSSR,qrand()%BASE+MAINSSR,qrand()%MINORBASE+MINORSSR);
            break;
        default:
            break;
    }
}

uint HighDefense::Attack()
{
    qsrand (QTime::currentTime ().msec ());
    uint probability=qrand ()%10;
    if(probability<8)//普通攻击
        return NormalAttack;
    else             //20%几率使用技能
        return this->skill;
}

void HighDefense::LevelUP()
{
    this->level++;

    this->attack+=50;
    this->blood+=50;
    this->currentBlood=this->blood;//升级后当前的血量回复满血
    this->defense+=100;
    this->speed+=50;
}
