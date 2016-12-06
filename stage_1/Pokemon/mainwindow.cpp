#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->CreatePkm ();
}

MainWindow::~MainWindow()
{
    delete ui;

    delete pokemon;
}

void MainWindow::CreatePkm()
{
    qsrand ((unsigned)time(NULL));//随机类型
    uint kind=qrand ()%4;
    uint qualification=qrand ()%3;
    SKILL skill=(SKILL)(kind*2+qrand ()%2);
    switch (kind) {
    case HIGH_ATTACK:
        pokemon=new HighAttack(qualification,skill);
        ui->InfoLabel->setText (pokemon->getInformation ());
        ui->AInfoLabel->hide ();
        break;
    case HIGH_BLOOD:
        pokemon=new HighBlood(qualification,skill);
        ui->InfoLabel->setText (pokemon->getInformation ());
        ui->AInfoLabel->hide ();
        break;
    case HIGH_DEFENSE:
        pokemon=new HighDefense(qualification,skill);
        ui->InfoLabel->setText (pokemon->getInformation ());
        ui->AInfoLabel->hide ();
        break;
    case HIGH_SPEED:
        pokemon=new HighSpeed(qualification,skill);
        ui->InfoLabel->setText (pokemon->getInformation ());
        ui->AInfoLabel->hide ();
        break;
    default:
        break;
    }
}

void MainWindow::on_AttackBtn_clicked()
{
    ui->AInfoLabel->show ();
    ui->AInfoLabel->setText (pokemon->getName ()+QString(" using ")+
                             ATTACKKIND[pokemon->Attack ()]);
}

void MainWindow::on_LevelUpBtn_clicked()
{
    pokemon->ExperienceUp (50*pokemon->getLevel ());
    ui->InfoLabel->setText (pokemon->getInformation ());

    if(pokemon->getLevel ()>=MAX_LEVEL)
        ui->LevelUpBtn->setDisabled (true);
}

void MainWindow::on_BtnChange_clicked()
{
    delete pokemon;

    ui->LevelUpBtn->setDisabled (false);

    this->CreatePkm ();
}
