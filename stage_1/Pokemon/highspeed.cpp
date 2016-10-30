#include "highspeed.h"

HighSpeed::HighSpeed(uint qualification,SKILL skillIndex)
{
    this->name="HIGH_Speed";
    this->kind=HIGH_SPEED;
    this->skill=skillIndex;

    srand((unsigned)time(NULL));
    switch (qualification)//资质越高，属性越高
    {
        case R:
            this->setValues(qrand()%BASE+BASE,qrand()%BASE+BASE,qrand()%BASE+BASE,qrand()%BASE+MAINR);
            break;
        case SR:
            this->setValues(qrand()%MINORBASE+MINORSR,qrand()%MINORBASE+MINORSR,qrand()%MINORBASE+MINORSR,qrand()%BASE+MAINSR);
            break;
        case SSR:
            this->setValues(qrand()%MINORBASE+MINORSSR,qrand()%MINORBASE+MINORSSR,qrand()%MINORBASE+MINORSSR,qrand()%BASE+MAINSSR);
            break;
        default:
            break;
    }
}

uint HighSpeed::Attack()
{
    qsrand (QTime::currentTime ().msec ());
    uint probability=qrand ()%10;
    if(probability<8)//普通攻击
        return NormalAttack;
    else             //20%几率使用技能
        return this->skill;
}

void HighSpeed::LevelUP()
{
    this->level++;

    this->attack+=50;
    this->blood+=50;
    this->currentBlood=this->blood;//升级后当前的血量回复满血
    this->defense+=50;
    this->speed+=100;
}
