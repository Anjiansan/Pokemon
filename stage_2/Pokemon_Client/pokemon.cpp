#include "pokemon.h"

void Pokemon::ExperienceUp(uint value)
{
    this->experience+=value;

    uint levelUpValue=50*this->level*(this->level+1)/2;//未升级前的升级上限
    while(this->level < MAX_LEVEL && this->experience >= levelUpValue)//while循环是为了连升多级
    {
        this->LevelUP();
        levelUpValue+=50*this->level;
    }
}

QString Pokemon::getName()
{
    return this->name;
}

uint Pokemon::getLevel()
{
    return this->level;
}

QString Pokemon::getInformation()
{
    QString information,temp;
    information=information+"name:"+name;
    information=information+"\nkind:"+temp.setNum (kind);
    information=information+"\nlevel:"+temp.setNum (level);
    information=information+"\nexperience:"+temp.setNum (experience);
    information=information+"\nattack:"+temp.setNum (attack);
    information=information+"\nblood:"+temp.setNum (blood);
    information=information+"\ncurrentBlood:"+temp.setNum (currentBlood);
    information=information+"\ndefense:"+temp.setNum (defense);
    information=information+"\nspeed:"+temp.setNum (speed);
    information+='\n';

    return information;
}

UDPPkm* Pokemon::getAttr()
{
    UDPPkm *attr=new UDPPkm();
    attr->name=this->name;
    attr->level=this->level;
    attr->experience=this->experience;
    attr->attack=this->attack;
    attr->blood=this->blood;
    attr->defense=this->defense;
    attr->speed=this->speed;
    attr->kind=this->kind;
    attr->attr=this->attr;
    attr->skill=this->skill;

    return attr;
}

void Pokemon::setValues(uint baseAttack, uint baseBlood, uint baseDefense, uint baseSpeed)
{
    this->attack=baseAttack;
    this->blood=baseBlood;
    this->currentBlood=baseBlood;
    this->defense=baseDefense;
    this->speed=baseSpeed;
}

QDataStream &operator<<(QDataStream &stream, const UDPPkm &pkm)
{
    stream<<pkm.name;
    stream<<pkm.level;
    stream<<pkm.experience;
    stream<<pkm.attack;
    stream<<pkm.blood;
    stream<<pkm.currentBlood;
    stream<<pkm.defense;
    stream<<pkm.speed;
    stream<<pkm.kind;
    stream<<pkm.attr;
    stream<<pkm.skill;

    return stream;
}

QDataStream &operator>>(QDataStream &stream, UDPPkm &pkm)
{
    stream>>pkm.name;
    stream>>pkm.level;
    stream>>pkm.experience;
    stream>>pkm.attack;
    stream>>pkm.blood;
    stream>>pkm.currentBlood;
    stream>>pkm.defense;
    stream>>pkm.speed;
    stream>>pkm.kind;
    stream>>pkm.attr;
    stream>>pkm.skill;

    return stream;
}
