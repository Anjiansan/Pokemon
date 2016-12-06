#include "highattack.h"

HighAttack::HighAttack(uint qualification,uint skillIndex)
{
//    this->name="HIGH_ATTRACK";
    this->name=POKEMONNAME[skillIndex];
    this->kind=HIGH_ATTACK;
    this->attr=qualification;
    this->skill=skillIndex;

    qsrand((unsigned)time(NULL));
    switch (qualification)//资质越高，属性越高
    {
        case S:
            this->setValues(qrand()%BASE+MAINR,qrand()%BASE+BASE,qrand()%BASE+BASE,qrand()%BASE+BASE);
            break;
        case SS:
            this->setValues(qrand()%BASE+MAINSR,qrand()%MINORBASE+MINORSR,qrand()%MINORBASE+MINORSR,qrand()%MINORBASE+MINORSR);
            break;
        case SSS:
            this->setValues(qrand()%BASE+MAINSSR,qrand()%MINORBASE+MINORSSR,qrand()%MINORBASE+MINORSSR,qrand()%MINORBASE+MINORSSR);
            break;
        default:
            break;
    }
}

HighAttack::HighAttack(uint l, uint e, uint a, uint b, uint d, uint s)
{
    this->level=l;
    this->experience=e;
    this->attack=a;
    this->blood=b;
    this->defense=d;
    this->speed=s;
}

uint HighAttack::Attack()
{
    qsrand (QTime::currentTime ().msec ());
    uint probability=qrand ()%10;
    if(probability<8)//普通攻击
        return NormalAttack;
    else             //20%几率使用技能
        return this->skill;
}

void HighAttack::LevelUP()
{
    this->level++;

    this->attack+=100;
    this->blood+=50;
    this->currentBlood=this->blood;//升级后当前的血量回复满血
    this->defense+=50;
    this->speed+=50;
}
