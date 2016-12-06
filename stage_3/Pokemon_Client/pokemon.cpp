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

void HydroCannonAttack(UDPPkm *self, UDPPkm *opponent)//攻击力提升为当前攻击力的level倍一回合
{
    uint damage;
    if((self->attack)/10*(self->level)<(opponent->defense)/150)
        damage=0;
    else
        damage=(self->attack)/10*(self->level)-(opponent->defense)/150;

    if(opponent->currentBlood<damage)
        opponent->currentBlood=0;
    else
        opponent->currentBlood-=damage;

//    return damage;
}

void CutAttack(UDPPkm *self, UDPPkm *opponent)//攻击力提升为当前攻击力的8倍一回合
{
    uint damage;
    if((self->attack)/10*8<(opponent->defense)/150)
        damage=0;
    else
        damage=(self->attack)/10*8-(opponent->defense)/150;

    if(opponent->currentBlood<damage)
        opponent->currentBlood=0;
    else
        opponent->currentBlood-=damage;
}

void LeechLifeAttack(UDPPkm *self, UDPPkm *opponent)//普通攻击，并将80%伤害转化为血量
{
    uint damage;
    if((self->attack)/10<(opponent->defense)/150)
        damage=0;
    else
        damage=(self->attack)/10-(opponent->defense)/150;

    self->currentBlood+=damage*0.8;
    if(self->currentBlood>self->blood)
        self->currentBlood=self->blood;
    if(opponent->currentBlood<damage)
        opponent->currentBlood=0;
    else
        opponent->currentBlood-=damage;
}

void FocusEnergyAttack(UDPPkm *self, UDPPkm *opponent)//普通攻击，并将(self->level)/10倍伤害转化为血量
{
    uint damage;
    if((self->attack)/10<(opponent->defense)/150)
        damage=0;
    else
        damage=(self->attack)/10-(opponent->defense)/150;

    self->currentBlood+=damage*(self->level)/10;
    if(self->currentBlood>self->blood)
        self->currentBlood=self->blood;
    if(opponent->currentBlood<damage)
        opponent->currentBlood=0;
    else
        opponent->currentBlood-=damage;
}

void BarrierAttack(UDPPkm *self, UDPPkm *opponent)//普通攻击，防御力提升为level倍
{
    uint damage;
    if((self->attack)/10<(opponent->defense)/150)
        damage=0;
    else
        damage=(self->attack)/10-(opponent->defense)/150;

    if(opponent->currentBlood<damage)
        opponent->currentBlood=0;
    else
        opponent->currentBlood-=damage;

    self->defense=(self->defense)*(self->level);
}

void LightScreenAttack(UDPPkm *self, UDPPkm *opponent)//普通攻击，防御力提升为8倍
{
    uint damage;
    if((self->attack)/10<(opponent->defense)/150)
        damage=0;
    else
        damage=(self->attack)/10-(opponent->defense)/150;

    if(opponent->currentBlood<damage)
        opponent->currentBlood=0;
    else
        opponent->currentBlood-=damage;

    self->defense=(self->defense)*8;
}

void CounterAttack(UDPPkm *self, UDPPkm *opponent)//快速造成两次100%的伤害
{
    uint damage;
    if((self->attack)/10<(opponent->defense)/150)
        damage=0;
    else
        damage=(self->attack)/10-(opponent->defense)/150;

    if(opponent->currentBlood<damage)
        opponent->currentBlood=0;
    else
        opponent->currentBlood-=damage;
    if(opponent->currentBlood<damage)
        opponent->currentBlood=0;
    else
        opponent->currentBlood-=damage;
}

void TriAttackAttack(UDPPkm *self, UDPPkm *opponent)//快速造成三次66%的伤害
{
    uint damage;
    if((self->attack)/10<(opponent->defense)/150)
        damage=0;
    else
        damage=((self->attack)/10-(opponent->defense)/150)*0.6;

    if(opponent->currentBlood<damage)
        opponent->currentBlood=0;
    else
        opponent->currentBlood-=damage;
    if(opponent->currentBlood<damage)
        opponent->currentBlood=0;
    else
        opponent->currentBlood-=damage;
    if(opponent->currentBlood<damage)
        opponent->currentBlood=0;
    else
        opponent->currentBlood-=damage;
}

void NmlAttack(UDPPkm *self, UDPPkm *opponent)//普通攻击
{
    uint damage;
    if((self->attack)/10<(opponent->defense)/150)
        damage=0;
    else
        damage=(self->attack)/10-(opponent->defense)/150;

    if(opponent->currentBlood<damage)
        opponent->currentBlood=0;
    else
        opponent->currentBlood-=damage;
}
