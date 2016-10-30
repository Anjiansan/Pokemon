#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent, Login *login) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    this->pkm=login->getPkm ();
    this->username=login->getUsername ();
    this->port=login->getPort ();
//    qDebug()<<this->port;
    ui->setupUi(this);

    this->mode=0;//不显示

    client=new QUdpSocket(this);

    server=new QUdpSocket(this);
    server->bind(QHostAddress::LocalHost, this->port);
    connect(server, SIGNAL(readyRead()),this, SLOT(readPendingDatagrams()));

    this->m_pPkmGroup=new QButtonGroup(this);
    m_pPkmGroup->addButton (ui->radioButton_1,0);
    m_pPkmGroup->addButton (ui->radioButton_2,1);
    m_pPkmGroup->addButton (ui->radioButton_3,2);

    ui->radioButton_1->setChecked (true);
    ui->labelWho->setText ("My Pokemon");
    ui->labelInfo->setText (this->getInfo (0));

    connect(m_pPkmGroup,SIGNAL(buttonClicked(int)),this,SLOT(RecvPkmID(int)));

    ui->showInfo->setHidden (true);
    ui->listWidgetUsers->setHidden (true);
}

MainWindow::~MainWindow()
{
    delete ui;

    QByteArray data;
    QDataStream dsOut(&data,QIODevice::ReadWrite);
    dsOut<<SIGNOUT<<this->username<<this->port;
    QHostAddress serverAddress = QHostAddress("127.0.0.1");
    client->writeDatagram(data.data(), data.size(),serverAddress, 6665);

    delete server;
    delete client;
}

void MainWindow::readPendingDatagrams()
{
    QByteArray data;
    data.resize(server->pendingDatagramSize());
    server->readDatagram(data.data(), data.size());
    QDataStream dsIn(&data,QIODevice::ReadWrite);

    uint dataKind;
    dsIn>>dataKind;
    if(dataKind==ONLINEUSERS)
    {
        for(int i=0;i<this->onlineUsers.length ();i++)
            delete this->onlineUsers[i];
        this->onlineUsers.clear ();//每次查询前都清空

        while(!dsIn.atEnd ())
        {
            User *user=new User;
            dsIn>>*user;
            this->onlineUsers.append (user);
        }
    }
    else if(dataKind==ALLUSERS)
    {
        for(int i=0;i<this->allUsers.length ();i++)
            delete this->allUsers[i];
        this->allUsers.clear ();//每次查询前都清空

        while(!dsIn.atEnd ())
        {
            UserData *user=new UserData;
            dsIn>>*user;
            this->allUsers.append (user);
        }
    }
}

void MainWindow::RecvPkmID(int id)
{
    switch(id)
    {
    case 0:
        ui->labelInfo->setText (this->getInfo (0));
        break;
    case 1:
        ui->labelInfo->setText (this->getInfo (1));
        break;
    case 2:
        ui->labelInfo->setText (this->getInfo (2));
        break;
    }
}

QString MainWindow::getInfo(uint index)
{
    UDPPkm *currentPkm;
    if(this->mode!=2)//除了模式为显示全部用户外
        currentPkm=this->pkm[index];
    else//模式为显示全部用户
        currentPkm=this->allUsers[ui->listWidgetUsers->currentRow ()-1]->pkm[index];

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


void MainWindow::on_BtnLoginOut_clicked()
{
//    QByteArray data;
//    QDataStream dsOut(&data,QIODevice::ReadWrite);
//    dsOut<<SIGNOUT<<this->username<<this->port;
//    QHostAddress serverAddress = QHostAddress("127.0.0.1");
//    client->writeDatagram(data.data(), data.size(),serverAddress, 6665);

    this->close ();
}

QDataStream &operator<<(QDataStream &stream, const User &user)
{
    stream<<user.username;
    stream<<user.port;

    return stream;
}

QDataStream &operator>>(QDataStream &stream, User &user)
{
    stream>>user.username;
    stream>>user.port;

    return stream;
}

void MainWindow::on_BtnOnline_clicked()
{
    if((ui->showInfo->isHidden ()&&ui->listWidgetUsers->isHidden ())||this->mode==2)
    {
        this->mode=1;//显示在线用户模式
        QByteArray data;
        QDataStream dsOut(&data,QIODevice::ReadWrite);
        dsOut<<ONLINEUSERS<<this->username<<this->port;//发送类型，用户名以及端口号
        QHostAddress serverAddress = QHostAddress("127.0.0.1");
        client->writeDatagram(data.data(), data.size(),serverAddress, 6665);

        if(server->waitForReadyRead (600))//判断连接超时
        {
            this->readPendingDatagrams ();

            ui->listWidgetUsers->clear ();
            ui->showInfo->setText ("Online Users");
            ui->listWidgetUsers->addItem ("Username\tPort");
            for(int i=0;i<this->onlineUsers.length ();i++)
                ui->listWidgetUsers->addItem (tr("%1\t%2").
                                              arg (this->onlineUsers[i]->username).
                                              arg(this->onlineUsers[i]->port));

            ui->showInfo->setHidden (false);
            ui->listWidgetUsers->setHidden (false);
        }
        else
            QMessageBox::critical (this,"Get Data Failed","Connect Failed","OK");
    }
    else
    {
        ui->showInfo->setHidden (true);
        ui->listWidgetUsers->setHidden (true);
        this->mode=0;//不显示的模式
    }
}

void MainWindow::on_BtnAllUsers_clicked()
{
    if((ui->showInfo->isHidden ()&&ui->listWidgetUsers->isHidden ())||this->mode==1)
    {
        this->mode=2;//显示全部用户模式

        QByteArray data;
        QDataStream dsOut(&data,QIODevice::ReadWrite);
        dsOut<<ALLUSERS<<this->username<<this->port;//发送类型，用户名以及端口号
        QHostAddress serverAddress = QHostAddress("127.0.0.1");
        client->writeDatagram(data.data(), data.size(),serverAddress, 6665);

        if(server->waitForReadyRead (600))//判断连接超时
        {
            this->readPendingDatagrams ();

            ui->listWidgetUsers->clear ();
            ui->showInfo->setText ("ALL Users");
            ui->listWidgetUsers->addItem ("Username");
            for(int i=0;i<this->allUsers.length ();i++)
                ui->listWidgetUsers->addItem (tr("%1").
                                              arg (this->allUsers[i]->username));

            ui->listWidgetUsers->setCurrentRow (1);
            ui->labelWho->setText (tr("%1's Pokemon").arg(this->allUsers[0]->username));
            ui->showInfo->setHidden (false);
            ui->listWidgetUsers->setHidden (false);
        }
        else
            QMessageBox::critical (this,"Get Data Failed","Connect Failed","OK");
    }
    else
    {
        ui->showInfo->setHidden (true);
        ui->listWidgetUsers->setHidden (true);

        this->mode=0;//不显示的模式

        ui->radioButton_1->setChecked (true);
        ui->labelWho->setText ("My Pokemon");
        ui->labelInfo->setText (this->getInfo (0));
    }
}

QDataStream &operator<<(QDataStream &stream, const UserData &userData)
{
    stream<<userData.username;
    for(int i=0;i<3;i++)
        stream<<*(userData.pkm[i]);

    return stream;
}

QDataStream &operator>>(QDataStream &stream, UserData &userData)
{
    stream>>userData.username;
    for(int i=0;i<3;i++)
    {
        UDPPkm *temp=new UDPPkm;
        stream>>*temp;
        userData.pkm.append (temp);
    }

    return stream;
}

void MainWindow::on_listWidgetUsers_currentRowChanged(int currentRow)
{
    if(currentRow!=-1&&this->mode==2)
    {
        ui->labelWho->setText (tr("%1's Pokemon").arg(this->allUsers[currentRow-1]->username));
        ui->radioButton_1->setChecked (true);
        ui->labelInfo->setText (this->getInfo (0));
    }
}
