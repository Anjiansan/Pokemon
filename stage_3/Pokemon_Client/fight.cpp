#include "fight.h"
#include "ui_fight.h"
#include <QDebug>
#include <QMessageBox>

Fight::Fight(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Fight)
{
    ui->setupUi(this);

    this->selfTimer=new QTimer(this);
    this->oppoTimer=new QTimer(this);
    connect(selfTimer,SIGNAL(timeout()),this,SLOT(selfAttack()));
    connect(oppoTimer,SIGNAL(timeout()),this,SLOT(oppoAttack()));

    this->attackFunc[0]=HydroCannonAttack;//初始化技能函数指针数组
    this->attackFunc[1]=CutAttack;
    this->attackFunc[2]=LeechLifeAttack;
    this->attackFunc[3]=FocusEnergyAttack;
    this->attackFunc[4]=BarrierAttack;
    this->attackFunc[5]=LightScreenAttack;
    this->attackFunc[6]=CounterAttack;
    this->attackFunc[7]=TriAttackAttack;

    QPalette pa;//设置字体颜色
    pa.setColor(QPalette::WindowText,Qt::red);
    ui->labelSelfA->setPalette(pa);
    ui->labelOppoA->setPalette (pa);

    connect(this,SIGNAL(sendResult(bool)),this->parentWidget (),SLOT(recvResult(bool)));

    ui->labelUserPkm->setObjectName ("labelUserPkm");
    ui->labelAdmPkm->setObjectName ("labelAdmPkm");
}

Fight::~Fight()
{
    delete ui;
}

QString Fight::getInfo(UDPPkm *currentPkm)
{
    QString information,temp;
    information=information+"name:"+currentPkm->name;
    information=information+"\nkind:"+KINDNAME[currentPkm->kind];
    information=information+"\nattr:"+ATTRKIND[currentPkm->attr];
    information=information+"\nlevel:"+temp.setNum (currentPkm->level);
    information=information+"\nexperience:"+temp.setNum (currentPkm->experience);
    information=information+"\nattack:"+temp.setNum (currentPkm->attack);
    information=information+"\nblood:"+temp.setNum (currentPkm->blood);
    information=information+"\ncurrentBlood:"+temp.setNum (currentPkm->currentBlood);
    information=information+"\ndefense:"+temp.setNum (currentPkm->defense);
    information=information+"\nspeed:"+temp.setNum (currentPkm->speed);
    information=information+"\nskill:"+ATTACKKIND[currentPkm->skill];
    information+='\n';

    return information;
}

void Fight::endFight()
{
    bool result;

    this->self->defense=this->selfDefense;
    this->opponent->defense=this->oppoDefense;

    this->selfTimer->stop ();
    this->oppoTimer->stop ();
    if(this->opponent->currentBlood==0)
    {
        QMessageBox::information (this,"Result","Victory","OK");
//        this->hide ();
//        this->parentWidget ()->show ();
        result=true;
    }
    else
    {
        QMessageBox::information (this,"Result","Defeat","OK");
//        this->hide ();
//        this->parentWidget ()->show ();
        result=false;
    }

    this->self->currentBlood=this->self->blood;//战斗之后回满血
    this->opponent->currentBlood=this->opponent->blood;

    emit sendResult (result);
}

void Fight::recvMsg(UDPPkm *self, UDPPkm *opponent)
{
    ui->labelUserPkm->setStyleSheet ("#labelUserPkm{border-image:url(:/img/"+
                                  self->name+".png);}");
    ui->labelAdmPkm->setStyleSheet ("#labelAdmPkm{border-image:url(:/img/"+
                                  opponent->name+".png);}");

    ui->progressBarSe->setRange (0,self->currentBlood);
    ui->progressBarSe->setValue (self->currentBlood);
    ui->progressBarOp->setRange (0,opponent->currentBlood);
    ui->progressBarOp->setValue (opponent->currentBlood);

    ui->labelSelfA->clear ();
    ui->labelOppoA->clear ();

    this->self=self;
    this->opponent=opponent;
    ui->labelUserPkm->setText (this->getInfo (self));
    ui->labelAdmPkm->setText (this->getInfo (opponent));

    this->selfDefense=self->defense;
    this->oppoDefense=opponent->defense;

    this->isSelfD=false;
    this->isOppoD=false;

//    if(self->kind==HIGH_SPEED)//初始化定时器
//    {
//        if(self->attr==S)
//            this->selfTimer->start(1600);
//        else if(self->attr==SS)
//            this->selfTimer->start(1300);
//        else if(self->attr==SSS)
//            this->selfTimer->start(1000);
//    }
//    else
//        this->selfTimer->start(1800);

//    if(opponent->kind==HIGH_SPEED)//初始化定时器
//    {
//        if(opponent->attr==S)
//            this->oppoTimer->start(1600);
//        else if(opponent->attr==SS)
//            this->oppoTimer->start(1300);
//        else if(opponent->attr==SSS)
//            this->oppoTimer->start(1000);
//    }
//    else
//        this->oppoTimer->start(1800);

    this->selfTimer->start(1800-(self->speed)/2);
    this->oppoTimer->start(1800-(opponent->speed)/2);
}

void Fight::selfAttack()
{
//    ui->labelOppoA->clear ();

    qsrand (QTime::currentTime ().msec ()*this->self->experience);
    uint probability=qrand ()%10;
    if(probability<1)//10%几率对手躲避攻击
    {
        ui->labelSelfA->setText (tr("Attack Failed, Be Dodge."));
        ui->labelUserPkm->setText (this->getInfo (self));
        ui->labelAdmPkm->setText (this->getInfo (opponent));
    }
    else if(probability<8)//70%几率普通攻击
    {
        NmlAttack(this->self,this->opponent);
        ui->labelSelfA->setText (tr("Using NormalAttack"));
        ui->labelUserPkm->setText (this->getInfo (self));
        ui->labelAdmPkm->setText (this->getInfo (opponent));
    }
    else             //20%几率使用技能
    {
        this->attackFunc[this->self->skill](this->self,this->opponent);
        ui->labelSelfA->setText (tr("Using %1").arg(ATTACKKIND[this->self->skill]));
        ui->labelUserPkm->setText (this->getInfo (self));
        ui->labelAdmPkm->setText (this->getInfo (opponent));

        if(this->self->skill==Barrier||this->self->skill==LightScreen)
            this->isSelfD=true;
    }

    ui->progressBarSe->setValue (self->currentBlood);//更新血量进度条
    ui->progressBarOp->setValue (opponent->currentBlood);

    if(this->isOppoD)
    {
        this->opponent->defense=this->oppoDefense;
        this->isOppoD=false;
    }

    if(this->opponent->currentBlood==0)
        this->endFight ();
}

void Fight::oppoAttack()
{
//    ui->labelSelfA->clear ();

    qsrand (QTime::currentTime ().msec ()*this->opponent->experience);
    uint probability=qrand ()%10;
    if(probability<1)//10%几率对手躲避攻击
    {
        ui->labelOppoA->setText (tr("Attack Failed, Be Dodge."));
        ui->labelUserPkm->setText (this->getInfo (self));
        ui->labelAdmPkm->setText (this->getInfo (opponent));
    }
    else if(probability<8)//70%几率普通攻击
    {
        NmlAttack(this->opponent,this->self);
        ui->labelOppoA->setText (tr("Using NormalAttack"));
        ui->labelUserPkm->setText (this->getInfo (self));
        ui->labelAdmPkm->setText (this->getInfo (opponent));
    }
    else             //20%几率使用技能
    {
        this->attackFunc[this->opponent->skill](this->opponent,this->self);
        ui->labelOppoA->setText (tr("Using %1").arg(ATTACKKIND[this->opponent->skill]));
        ui->labelUserPkm->setText (this->getInfo (self));
        ui->labelAdmPkm->setText (this->getInfo (opponent));

        if(this->opponent->skill==Barrier||this->opponent->skill==LightScreen)
            this->isOppoD=true;
    }

    ui->progressBarSe->setValue (self->currentBlood);//更新血量进度条
    ui->progressBarOp->setValue (opponent->currentBlood);

    if(this->isSelfD)
    {
        this->self->defense=this->selfDefense;
        this->isSelfD=false;
    }

    if(this->self->currentBlood==0)
        this->endFight ();
}
