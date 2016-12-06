#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent, Login *login) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    this->fight->hide ();
    connect(this,SIGNAL(sentMsg(UDPPkm*,UDPPkm*)),this->fight,SLOT(recvMsg(UDPPkm*,UDPPkm*)));

    this->selectPkm->hide ();
    connect(this,SIGNAL(sentSelect(QList<UDPPkm*>,uint*)),
            this->selectPkm,SLOT(recvSelect(QList<UDPPkm*>,uint*)));

    this->userPkmNum=login->getPkmNum ();
    this->pkm=login->getPkm ();
    this->username=login->getUsername ();
    this->port=login->getPort ();
//    qDebug()<<this->port;
    ui->setupUi(this);

    this->mode=0;//不显示
    this->gameKind=0;//无比赛模式

    client=new QUdpSocket(this);

    server=new QUdpSocket(this);
    server->bind(QHostAddress::LocalHost, this->port);
    connect(server, SIGNAL(readyRead()),this, SLOT(readPendingDatagrams()));

    ui->labelUser->setText (tr("User:%1").arg(this->username));

    for(uint i=0;i<this->userPkmNum;i++)//显示用户小精灵
        ui->CbxUser->addItem (this->pkm[i]->name);
    ui->CbxUser->setCurrentIndex (0);

    ui->labelWho->setText (tr("%1's Pokemon").arg(this->username));
    ui->labelInfo->setText (this->getInfo (0));

    ui->showInfo->setHidden (true);
    ui->listWidgetUsers->setHidden (true);
    ui->labelServer->setHidden (true);
    ui->labelServer->setText ("Opponent");
    ui->comboBox->setHidden (true);
    ui->BtnConfirm->setHidden (true);
    ui->labelSPkm->setHidden (true);
    ui->labelBadge->setHidden (true);

    QPalette pa;//设置字体颜色
    pa.setColor(QPalette::WindowText,Qt::blue);
    ui->labelBadge->setPalette(pa);

//    layout ()->setSizeConstraint (QLayout::SetFixedSize);
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
            QString name;
            uint total;
            dsIn>>name>>total;
            UserData *user=new UserData(name,total);
            dsIn>>*user;
            this->allUsers.append (user);
//            qDebug()<<total<<user->username;
        }
    }
    else if(dataKind==GETADMINPKM)
    {
        for(int i=0;i<this->adminPkm.length ();i++)
            delete this->adminPkm[i];
        this->adminPkm.clear ();//每次查询前都清空

        dsIn>>this->adminPkmLen;
        for(uint i=0;i<this->adminPkmLen;i++)
        {
            UDPPkm *udpPkm=new UDPPkm();
            dsIn>>*udpPkm;
            this->adminPkm.append (udpPkm);
        }
    }
    else if(dataKind==UPDATED)
    {
        ui->CbxUser->clear ();

        dsIn>>this->userPkmNum;
        for(uint i=0;i<this->userPkmNum;i++)
        {
            UDPPkm *udpPkm=new UDPPkm;
            dsIn>>*udpPkm;
            pkm[i]=udpPkm;
        }
    }
    else if(dataKind==WINNINGRATE)
    {
        while(!dsIn.atEnd ())
        {
            QString user;
            uint win,lose;
            dsIn>>user>>win>>lose;
            if(win+lose==0)
                ui->listWidgetUsers->addItem (tr("%1\t0").
                                              arg (user));
            else
                ui->listWidgetUsers->addItem (tr("%1\t%2").
                                          arg (user).arg((float)win/(win+lose)));
        }
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

QString MainWindow::getAdmPkmInfo(uint index)
{
    UDPPkm *currentPkm;
    currentPkm=this->adminPkm[index];

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
    if((ui->showInfo->isHidden ()&&ui->listWidgetUsers->isHidden ())||
            this->mode==2||this->mode==3)
    {
        if(this->mode==2)
        {
            this->mode=1;//显示在线用户模式

            ui->CbxUser->clear ();
            for(uint i=0;i<this->userPkmNum;i++)
                ui->CbxUser->addItem (this->pkm[i]->name);
            ui->CbxUser->setCurrentIndex (0);//显示当前用户的Pokemon
            ui->labelWho->setText (tr("%1's Pokemon").arg(this->username));
            ui->labelInfo->setText (this->getInfo (0));
        }

        this->mode=1;//显示在线用户模式
        QByteArray data;
        QDataStream dsOut(&data,QIODevice::ReadWrite);
        dsOut<<ONLINEUSERS<<this->username<<this->port;//发送类型，用户名以及端口号
        QHostAddress serverAddress = QHostAddress("127.0.0.1");
        client->writeDatagram(data.data(), data.size(),serverAddress, 6665);

        if(server->waitForReadyRead (600))//判断连接超时
        {
//            this->readPendingDatagrams ();

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
    if((ui->showInfo->isHidden ()&&ui->listWidgetUsers->isHidden ())||
            this->mode==1||this->mode==3)
    {
        this->mode=2;//显示全部用户模式

        QByteArray data;
        QDataStream dsOut(&data,QIODevice::ReadWrite);
        dsOut<<ALLUSERS<<this->username<<this->port;//发送类型，用户名以及端口号
        QHostAddress serverAddress = QHostAddress("127.0.0.1");
        client->writeDatagram(data.data(), data.size(),serverAddress, 6665);

        if(server->waitForReadyRead (600))//判断连接超时
        {
//            this->readPendingDatagrams ();

            ui->listWidgetUsers->clear ();
            ui->showInfo->setText ("ALL Users");
            ui->listWidgetUsers->addItem ("Username");
            for(int i=0;i<this->allUsers.length ();i++)
                ui->listWidgetUsers->addItem (tr("%1").
                                              arg (this->allUsers[i]->username));

            ui->listWidgetUsers->setCurrentRow (1);
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

        ui->CbxUser->setCurrentIndex (0);
        ui->labelWho->setText (tr("%1's Pokemon").arg(this->username));
        ui->labelInfo->setText (this->getInfo (0));
    }
}

QDataStream &operator<<(QDataStream &stream, const UserData &userData)
{
    stream<<userData.username;
    for(uint i=0;i<userData.pkmLen;i++)
        stream<<*(userData.pkm[i]);

    return stream;
}

QDataStream &operator>>(QDataStream &stream, UserData &userData)
{
    for(uint i=0;i<userData.pkmLen;i++)
    {
        UDPPkm *temp=new UDPPkm;
        stream>>*temp;
        userData.pkm.append (temp);
    }

    return stream;
}

void MainWindow::on_listWidgetUsers_currentRowChanged(int currentRow)
{
    if(currentRow!=-1&&currentRow!=0&&this->mode==2)
    {
        ui->labelWho->setText (tr("%1's Pokemon").arg(this->allUsers[currentRow-1]->username));
        ui->CbxUser->clear ();
        for(uint i=0;i<this->allUsers[currentRow-1]->pkmLen;i++)
            ui->CbxUser->addItem (this->allUsers[currentRow-1]->pkm[i]->name);
        ui->CbxUser->setCurrentIndex (0);
        ui->labelInfo->setText (this->getInfo (0));
    }
}

void MainWindow::on_BtnUpgrade_clicked()//反复显示和隐藏
{
    if(ui->labelServer->isHidden ())
    {
        this->gameKind=1;//升级赛模式

        QByteArray data;
        QDataStream dsOut(&data,QIODevice::ReadWrite);
        dsOut<<GETADMINPKM<<this->username<<this->port;//发送类型，发送用户名以及端口号
        QHostAddress serverAddress = QHostAddress("127.0.0.1");
        client->writeDatagram(data.data(), data.size(),serverAddress, 6665);

        if(server->waitForReadyRead (600))//判断连接超时
        {
            ui->comboBox->clear ();

            for(uint i=0;i<this->adminPkmLen;i++)//显示系统小精灵
                ui->comboBox->addItem (this->adminPkm[i]->name);
            ui->comboBox->setCurrentIndex (0);
        }
        else
            QMessageBox::critical (this,"Get Data Failed","Connect Failed","OK");

        ui->BtnDuel->setDisabled (true);
        ui->labelServer->setHidden (false);
        ui->comboBox->setHidden (false);
        ui->BtnConfirm->setHidden (false);
        ui->labelSPkm->setHidden (false);
    }
    else
    {
        this->gameKind=0;//无比赛模式
        ui->BtnDuel->setDisabled (false);
        ui->labelServer->setHidden (true);
        ui->comboBox->setHidden (true);
        ui->BtnConfirm->setHidden (true);
        ui->labelSPkm->setHidden (true);
    }
}

void MainWindow::on_BtnDuel_clicked()//反复显示和隐藏
{
    if(ui->labelServer->isHidden ())
    {
        this->gameKind=2;//决斗赛模式

        QByteArray data;
        QDataStream dsOut(&data,QIODevice::ReadWrite);
        dsOut<<GETADMINPKM<<this->username<<this->port;//发送类型，发送用户名以及端口号
        QHostAddress serverAddress = QHostAddress("127.0.0.1");
        client->writeDatagram(data.data(), data.size(),serverAddress, 6665);

        if(server->waitForReadyRead (600))//判断连接超时
        {
            ui->comboBox->clear ();

            for(uint i=0;i<this->adminPkmLen;i++)//显示系统小精灵
                ui->comboBox->addItem (this->adminPkm[i]->name);
            ui->comboBox->setCurrentIndex (0);
        }
        else
            QMessageBox::critical (this,"Get Data Failed","Connect Failed","OK");

        ui->BtnUpgrade->setDisabled (true);
        ui->labelServer->setHidden (false);
        ui->comboBox->setHidden (false);
        ui->BtnConfirm->setHidden (false);
        ui->labelSPkm->setHidden (false);
    }
    else
    {
        this->gameKind=0;//无比赛模式
        ui->BtnUpgrade->setDisabled (false);
        ui->labelServer->setHidden (true);
        ui->comboBox->setHidden (true);
        ui->BtnConfirm->setHidden (true);
        ui->labelSPkm->setHidden (true);
    }
}

void MainWindow::on_CbxUser_currentIndexChanged(int index)
{
    if(index < 0)   //序号为负数不处理
    {
        return;
    }
    //正常序号
    ui->labelInfo->setText (this->getInfo (index));
}

void MainWindow::on_BtnConfirm_clicked()
{
    QByteArray data;
    QDataStream dsOut(&data,QIODevice::ReadWrite);

    QString who=ui->labelWho->text ().split ("'")[0];
    if(who!=this->username)
        QMessageBox::critical (this,"ERROR","Please Change the Pokemon to Yours.","OK");
    else
    {
        UDPPkm *self=this->pkm[ui->CbxUser->currentIndex ()];
        UDPPkm *opponent=this->adminPkm[ui->comboBox->currentIndex ()];
        emit this->sentMsg (self,opponent);
        this->hide ();
        this->fight->show ();

//        if(this->gameKind==1)
//            dsOut<<UPGRADE;//发送升级赛类型
//        else
//            dsOut<<DUEL;//发送决斗赛类型
//        dsOut<<this->username<<this->port;//发送用户名以及端口号
//        dsOut<<ui->CbxUser->currentIndex ();//发送用于升级的小精灵
//        dsOut<<ui->comboBox->currentIndex ();//发送用户选择的系统对战小精灵
//        QHostAddress serverAddress = QHostAddress("127.0.0.1");
//        client->writeDatagram(data.data(), data.size(),serverAddress, 6665);

//        if(server->waitForReadyRead (600))//判断连接超时
//        {
//            this->readPendingDatagrams ();


//        }
//        else
//            QMessageBox::critical (this,"Get Data Failed","Connect Failed","OK");
    }
}

void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    if(index < 0)   //序号为负数不处理
    {
        return;
    }
    //正常序号
    ui->labelSPkm->setText (this->getAdmPkmInfo (index));
}

void MainWindow::recvResult(bool isWin)
{
    if(isWin)
    {
        QByteArray data;
        QDataStream dsOut(&data,QIODevice::ReadWrite);
        if(this->gameKind==1)
            dsOut<<UPGRADE;//发送升级赛类型
        else
            dsOut<<DUEL;//发送决斗赛类型
        dsOut<<this->username<<this->port;//发送用户名以及端口号
        dsOut<<ui->CbxUser->currentIndex ();//发送战斗的小精灵
        dsOut<<isWin;//发送战斗结果
        dsOut<<ui->comboBox->currentIndex ();//发送用于战斗的系统小精灵
        QHostAddress serverAddress = QHostAddress("127.0.0.1");
        client->writeDatagram(data.data(), data.size(),serverAddress, 6665);

        if(server->waitForReadyRead (600))//判断连接超时
        {
            for(uint i=0;i<this->userPkmNum;i++)//显示用户小精灵
                ui->CbxUser->addItem (this->pkm[i]->name);
            ui->CbxUser->setCurrentIndex (0);
        }
        else
            QMessageBox::critical (this,"Send Data Failed","Connect Failed","OK");

        this->show ();
        this->fight->hide ();
    }
    else if(gameKind==2)
    {
        QList<UDPPkm *> selected;
        for(uint i=0;i<3;i++)
            this->pkmIndex[i]=i;

        if(this->userPkmNum<4)
        {
            for(uint i=0;i<this->userPkmNum;i++)
                selected.append (this->pkm[i]);

            emit sentSelect(selected,pkmIndex);
            this->fight->hide ();
            this->selectPkm->show ();
        }
        else
        {
            qsrand ((unsigned)time(NULL));//随机选三个小精灵
            int a=-1,b=-1,c=-1;
            a=qrand ()%(this->userPkmNum);
            selected.append (this->pkm[a]);
            pkmIndex[0]=a;

            b=qrand ()%(this->userPkmNum);
            while(a==b)
                b=qrand ()%(this->userPkmNum);
            selected.append (this->pkm[b]);
            pkmIndex[1]=b;

            c=qrand ()%(this->userPkmNum);
            while(c==a||c==b)
                c=qrand ()%(this->userPkmNum);
            selected.append (this->pkm[c]);
            pkmIndex[2]=c;

            emit sentSelect (selected,pkmIndex);
            this->fight->hide ();
            this->selectPkm->show ();
        }
    }
    else
    {
        QByteArray data;
        QDataStream dsOut(&data,QIODevice::ReadWrite);
        dsOut<<UPGRADE;//发送升级赛类型
        dsOut<<this->username<<this->port;//发送用户名以及端口号
        dsOut<<ui->CbxUser->currentIndex ();//发送战斗的小精灵
        dsOut<<isWin;//发送战斗结果
        dsOut<<ui->comboBox->currentIndex ();//发送用于战斗的系统小精灵
        QHostAddress serverAddress = QHostAddress("127.0.0.1");
        client->writeDatagram(data.data(), data.size(),serverAddress, 6665);

        if(server->waitForReadyRead (600))//判断连接超时
        {
            for(uint i=0;i<this->userPkmNum;i++)//显示用户小精灵
                ui->CbxUser->addItem (this->pkm[i]->name);
            ui->CbxUser->setCurrentIndex (0);
        }
        else
            QMessageBox::critical (this,"Send Data Failed","Connect Failed","OK");

        this->show ();
        this->fight->hide ();
    }
}

void MainWindow::recvSelectResult(uint index)
{
    QByteArray data;
    QDataStream dsOut(&data,QIODevice::ReadWrite);
    dsOut<<DUEL;//发送决斗赛类型
    dsOut<<this->username<<this->port;//发送用户名以及端口号
    dsOut<<index;//发送用户选择送出的小精灵
    dsOut<<false<<ui->comboBox->currentIndex ();//战斗结果及用于战斗的系统小精灵
    QHostAddress serverAddress = QHostAddress("127.0.0.1");
    client->writeDatagram(data.data(), data.size(),serverAddress, 6665);

    if(server->waitForReadyRead (600))//判断连接超时
    {
        for(uint i=0;i<this->userPkmNum;i++)//显示用户小精灵
            ui->CbxUser->addItem (this->pkm[i]->name);
        ui->CbxUser->setCurrentIndex (0);
    }
    else
        QMessageBox::critical (this,"Send Data Failed","Connect Failed","OK");

    this->show ();
    this->selectPkm->hide ();
}


void MainWindow::on_BtnRate_clicked()
{
    if(this->mode!=3)
    {
        ui->CbxUser->clear ();
        for(uint i=0;i<this->userPkmNum;i++)
            ui->CbxUser->addItem (this->pkm[i]->name);
        ui->CbxUser->setCurrentIndex (0);//显示当前用户的Pokemon
        ui->labelWho->setText (tr("%1's Pokemon").arg(this->username));
        ui->labelInfo->setText (this->getInfo (0));

        QByteArray data;
        QDataStream dsOut(&data,QIODevice::ReadWrite);
        dsOut<<WINNINGRATE<<this->username<<this->port;//发送类型，用户名以及端口号
        QHostAddress serverAddress = QHostAddress("127.0.0.1");
        client->writeDatagram(data.data(), data.size(),serverAddress, 6665);

        ui->listWidgetUsers->clear ();
        ui->showInfo->setText ("Winning Rate");
        ui->listWidgetUsers->addItem ("Username\tWinning Rate");
        if(server->waitForReadyRead (600))//判断连接超时
        {
            ;
        }
        else
            QMessageBox::critical (this,"Get Data Failed","Connect Failed","OK");

        if(this->mode==0)
        {
            ui->showInfo->setHidden (false);
            ui->listWidgetUsers->setHidden (false);
        }

        this->mode=3;//显示用户或胜率模式
    }
    else
    {
        this->mode=0;//不显示模式
        ui->showInfo->setHidden (true);
        ui->listWidgetUsers->setHidden (true);
    }
}

void MainWindow::on_BtnBadge_toggled(bool checked)
{
    if(checked)
    {
        QString badgeNum;
        if(this->userPkmNum<=3)
            badgeNum="Bronze";
        else if(this->userPkmNum<=6)
            badgeNum="Silver";
        else
            badgeNum="Golden";

        QString badgeAdvanced;
        uint advancedNum=0;
        for(uint i=0;i<this->userPkmNum;i++)
            if(this->pkm[i]->level==15)
                advancedNum+=1;

        if(advancedNum<1)
            badgeAdvanced="Bronze";
        else if(advancedNum<3)
            badgeAdvanced="Silver";
        else
            badgeAdvanced="Golden";

        ui->labelBadge->setText ("The Number of Pkm:"+badgeNum+
                                 "\nThe Number of Advanced Pkm:"+badgeAdvanced);

        ui->labelBadge->setHidden (false);
    }
    else
        ui->labelBadge->setHidden (true);
}
