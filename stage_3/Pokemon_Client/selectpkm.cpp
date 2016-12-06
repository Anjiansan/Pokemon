#include "selectpkm.h"
#include "ui_selectpkm.h"

SelectPkm::SelectPkm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SelectPkm)
{
    ui->setupUi(this);

    connect(this,SIGNAL(sentSelectResult(uint)),
            this->parentWidget (),SLOT(recvSelectResult(uint)));
}

SelectPkm::~SelectPkm()
{
    delete ui;
}

QString SelectPkm::getInfo(uint index)
{
    UDPPkm *currentPkm;
    currentPkm=this->pkms[index];

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

void SelectPkm::recvSelect(QList<UDPPkm *> pkmList, uint *pkmIndex)
{
    this->pkms.clear ();
    this->pkms=pkmList;
    this->pkmIndex=pkmIndex;

    ui->comboBoxPkm->clear ();
    for(uint i=0;i<pkmList.length ();i++)
        ui->comboBoxPkm->addItem (pkmList[i]->name);
    ui->comboBoxPkm->setCurrentIndex (0);

    ui->labelPkmInfo->setText (this->getInfo (0));
}

void SelectPkm::on_comboBoxPkm_currentIndexChanged(int index)
{
    if(index>=0&&index<this->pkms.length ())
        ui->labelPkmInfo->setText (this->getInfo (index));
}

void SelectPkm::on_BtnCfm_clicked()
{
    emit sentSelectResult (this->pkmIndex[ui->comboBoxPkm->currentIndex ()]);
}
