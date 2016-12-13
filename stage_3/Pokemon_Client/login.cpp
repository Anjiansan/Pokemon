#include "login.h"
#include "ui_login.h"
#include <QCryptographicHash>
#include <QMessageBox>

Login::Login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);

    client=new QUdpSocket(this);

    server=new QUdpSocket(this);
    this->port=6666;
    bool isOk=false;
    while(!isOk)
    {//判断端口是否被占用，若是则默认端口加一
        if(server->bind(QHostAddress::LocalHost, this->port))
            isOk=true;
        else
            this->port++;
//        qDebug()<<isOk;
    }
//    connect(server, SIGNAL(readyRead()),this, SLOT(readPendingDatagrams()));

    this->setObjectName ("Login");
    this->setStyleSheet ("#Login{border-image:url(:/img/Pokemon.png);}");
}

Login::~Login()
{
    delete ui;

    if(server)
    {
        server->close ();
        delete server;
    }
    if(client)
    {
        client->close ();
        delete client;
    }
}

UDPPkm **Login::getPkm()
{
    return this->pkm;
}

uint Login::getPkmNum()
{
    return this->userPkmNum;
}

QString Login::getUsername()
{
    return this->username;
}

uint Login::getPort()
{
    return this->port;
}

void Login::readPendingDatagrams()
 {
    QByteArray data;
    data.resize(server->pendingDatagramSize());
    server->readDatagram(data.data(), data.size());
    QDataStream dsIn(&data,QIODevice::ReadWrite);
    uint loginKind;
    QString username;
    QString pwd;
    dsIn>>loginKind;
    if(loginKind==SIGNINOK)
    {
        QMessageBox::information (this,"Sign In","Login Success","OK");

        dsIn>>this->userPkmNum;
        for(uint i=0;i<this->userPkmNum;i++)
        {
            UDPPkm *udpPkm=new UDPPkm;
            dsIn>>*udpPkm;
            pkm[i]=udpPkm;
        }

        client->close ();
        delete client;
        client=NULL;
        server->abort ();
        server->close ();
        delete server;
        server=NULL;
        accept ();
    }
    else if(loginKind==SIGNUPOK)
    {
        QMessageBox::information (this,"Sign Up","Sign Up Sucess","OK");

        dsIn>>this->userPkmNum;
        for(int i=0;i<this->userPkmNum;i++)
        {
            UDPPkm *udpPkm=new UDPPkm;
            dsIn>>*udpPkm;
            pkm[i]=udpPkm;
        }

        client->close ();
        delete client;
        client=NULL;
        server->abort ();
        server->close ();
        delete server;
        server=NULL;
        accept ();
    }
    else if(loginKind==NAMEEXIST)//用户名已存在
        QMessageBox::critical (this,"Sign Up Failed","Username Exist,Please Change Username.","OK");
    else if(loginKind==NOSUCHUSER)//不存在此用户
        QMessageBox::critical (this,"Sign In Failed","No Such User,Please Login Again.","OK");
    else if(loginKind==PWDDIFF)//密码不一致
        QMessageBox::critical (this,"Sign In Failed","Password Don't Match,Please Login Again.","OK");

 }

void Login::on_BtnSignUp_clicked()
{
    QString username=ui->lineEditName->text ();
    this->username=username;
    QString pwd=ui->lineEditPwd->text ();

    if(!username.isEmpty ()&&!pwd.isEmpty ())
    {
        QByteArray temp;//MD5加密
        temp = QCryptographicHash::hash ( pwd.toUtf8(), QCryptographicHash::Md5 );
        pwd.clear ();
        pwd.append(temp.toHex());

        QByteArray data;
        QDataStream dsOut(&data,QIODevice::ReadWrite);
        dsOut<<SIGNUP<<username<<pwd<<this->port;//发送登录类型，用户名密码以及端口号
        QHostAddress serverAddress = QHostAddress("127.0.0.1");
        client->writeDatagram(data.data(), data.size(),serverAddress, 6665);

        if(server->waitForReadyRead (600))//判断连接超时
            this->readPendingDatagrams ();
        else
            QMessageBox::critical (this,"Sign Up Failed","Connect Failed","OK");

    }
    else
        QMessageBox::critical (this,"Sign Up Failed","Username And Pwd Must Not Be Empty","OK");
}

void Login::on_BtnSignIn_clicked()
{
    QString username=ui->lineEditName->text ();
    this->username=username;
    QString pwd=ui->lineEditPwd->text ();

    if(!username.isEmpty ()&&!pwd.isEmpty ())
    {
        QByteArray temp;//MD5加密
        temp = QCryptographicHash::hash ( pwd.toUtf8(), QCryptographicHash::Md5 );
        pwd.clear ();
        pwd.append(temp.toHex());

        QByteArray data;
        QDataStream dsOut(&data,QIODevice::ReadWrite);
        dsOut<<SIGNIN<<username<<pwd<<this->port;//发送登录类型，用户名密码以及端口号
        QHostAddress serverAddress = QHostAddress("127.0.0.1");
        client->writeDatagram(data.data(), data.size(),serverAddress, 6665);

        if(server->waitForReadyRead (600))//判断连接超时
            this->readPendingDatagrams ();
        else
            QMessageBox::critical (this,"Sign In Failed","Connect Failed","OK");
    }
    else
        QMessageBox::critical (this,"Sign In Failed","Username And Pwd Must Not Be Empty","OK");
}
