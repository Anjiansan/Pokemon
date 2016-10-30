#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    pokemon=new HighAttack(R,Cut);
    ui->InfoLabel->setText (pokemon->getInformation ());
    ui->AInfoLabel->hide ();
}

MainWindow::~MainWindow()
{
    delete ui;
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
