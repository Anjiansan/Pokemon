#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    server=new QUdpSocket(this);//接受客户端消息
    server->bind(QHostAddress::LocalHost, 6665);
    connect(server, SIGNAL(readyRead()),this, SLOT(readPendingDatagrams()));

    client=new QUdpSocket(this);//向客户端发消息

    db=new QSqlDatabase();
    *db=QSqlDatabase::addDatabase ("QMYSQL");
    db->setDatabaseName ("pokemon");
    db->setUserName ("huang");
    db->setPassword ("1996");
    if(!db->open ())
    {
        qDebug()<<"Failed to mysql";
    }
}

MainWindow::~MainWindow()
{
    delete ui;

    delete server;
    delete client;
    delete db;
}

void MainWindow::CreatePkm(QString user)
{
    for(int i=0;i<3;i++)
    {
        qsrand ((unsigned)time(NULL)*i);//随机类型
        uint kind=qrand ()%4;

        qsrand ((unsigned)time(NULL)*i*kind);//随机属性
        uint qualification=qrand ()%10;
        if(qualification<7)//70%几率S
            qualification=S;
        else if(qualification<9)//20%几率S
            qualification=SS;
        else//10%几率S
            qualification=SSS;

        qsrand ((unsigned)time(NULL)*i*kind*qualification);//随机技能
        switch (kind) {
        case 0://HIGH_ATTACK
        {
            uint skillA=qrand ()%2;
            this->pokemon=new HighAttack(qualification,skillA);
            UDPPkm *attrA=pokemon->getAttr ();

            this->PutIntoSql (user,attrA);//写入数据库中

            delete attrA;
            delete this->pokemon;
            this->pokemon=NULL;
            break;
        }
        case 1://HIGH_BLOOD
        {
            uint skillB=qrand ()%2+2;
            this->pokemon=new HighBlood(qualification,skillB);
            UDPPkm *attrB=pokemon->getAttr ();

            this->PutIntoSql (user,attrB);//写入数据库中

            delete attrB;
            delete this->pokemon;
            this->pokemon=NULL;
            break;
        }
        case 2://HIGH_DEFENSE
        {
            uint skillD=qrand ()%2+4;
            this->pokemon=new HighDefense(qualification,skillD);
            UDPPkm *attrD=pokemon->getAttr ();

            this->PutIntoSql (user,attrD);//写入数据库中

            delete attrD;
            delete this->pokemon;
            this->pokemon=NULL;
            break;
        }
        case 3://HIGH_SPEED
        {
            uint skillS=qrand ()%2+6;
            this->pokemon=new HighSpeed(qualification,skillS);
            UDPPkm *attrS=pokemon->getAttr ();

            this->PutIntoSql (user,attrS);//写入数据库中

            delete attrS;
            delete this->pokemon;
            this->pokemon=NULL;
            break;
        }
        default:
            break;
        }
    }
}

void MainWindow::PutIntoSql(QString user,UDPPkm *pkm)
{
    QSqlQuery query;
    QString sqlString=tr("insert into pokemon values(\"%1\",\"%2\",%3,%4,%5,%6,%7,%8,%9,%10,%11);").
            arg (user).arg (pkm->name).arg (pkm->level).arg (pkm->experience).
            arg (pkm->attack).arg (pkm->blood).arg (pkm->defense).arg (pkm->speed).
            arg (pkm->kind).arg (pkm->attr).arg (pkm->skill);

    query.exec (sqlString);
}

void MainWindow::SentPkm(QDataStream &dsOut, QString user)
{
    QSqlQuery query;
    query.exec ("select * from pokemon where user=\""+user+"\";");
    while(query.next ())
    {
        UDPPkm udpPkm;
        udpPkm.name=query.value (1).toString ();
        udpPkm.level=query.value (2).toUInt ();
        udpPkm.experience=query.value (3).toUInt ();
        udpPkm.attack=query.value (4).toUInt ();
        udpPkm.blood=query.value (5).toUInt ();
        udpPkm.currentBlood=udpPkm.blood;
        udpPkm.defense=query.value (6).toUInt ();
        udpPkm.speed=query.value (7).toUInt ();
        udpPkm.kind=query.value (8).toUInt ();
        udpPkm.attr=query.value (9).toUInt ();
        udpPkm.skill=query.value (10).toUInt ();

        dsOut<<udpPkm;
    }
}

void MainWindow::SentOnlineUsers(uint port)
{
    QByteArray dataOut;
    QDataStream dsOut(&dataOut,QIODevice::ReadWrite);

    dsOut<<ONLINEUSERS;
//    qDebug()<<"length:"<<this->onlineUsers.length ();
    for(int i=0;i<this->onlineUsers.length ();i++)
        dsOut<<this->onlineUsers[i];

    QHostAddress serverAddress = QHostAddress("127.0.0.1");
    client->writeDatagram(dataOut.data(), dataOut.size(),serverAddress, port);
}

void MainWindow::SentAllUsers(uint port)
{
    QByteArray dataOut;
    QDataStream dsOut(&dataOut,QIODevice::ReadWrite);
    dsOut<<ALLUSERS;

    QSqlQuery query;
    query.exec ("select * from users");
    while(query.next ())
    {
        dsOut<<query.value (0).toString ();
//        qDebug()<<query.value (0).toString ();
        this->SentPkm (dsOut,query.value (0).toString ());
    }

    QHostAddress serverAddress = QHostAddress("127.0.0.1");
    client->writeDatagram(dataOut.data(), dataOut.size(),serverAddress, port);
}

void MainWindow::readPendingDatagrams()
 {
    QByteArray dataIn;//读取数据
    dataIn.resize(server->pendingDatagramSize());
    server->readDatagram(dataIn.data(), dataIn.size());
    QDataStream dsIn(&dataIn,QIODevice::ReadWrite);
    QString username;//用户名
    QString pwd;//密码
    uint port;//端口号
    uint loginKind;
    dsIn>>loginKind>>username;

    if(loginKind==SIGNUP||loginKind==SIGNIN)
    {
        dsIn>>pwd>>port;
    }

    QByteArray dataOut;
    QDataStream dsOut(&dataOut,QIODevice::ReadWrite);
    QSqlQuery query;
    if(loginKind==SIGNUP)
    {
        query.exec ("select * from users where username=\""+username+"\";");
        if(query.next ())//用户名已存在
            dsOut<<NAMEEXIST;
        else
        {
            User user;
            user.username=username;
            user.port=port;
            this->onlineUsers.append (user);

            query.exec ("insert into users values(\""+username+"\",\""+pwd+"\",0,0);");
            this->CreatePkm (username);
            dsOut<<SIGNUPOK;
            SentPkm (dsOut,username);
        }
        QHostAddress serverAddress = QHostAddress("127.0.0.1");
        client->writeDatagram(dataOut.data(), dataOut.size(),serverAddress, port);
    }
    else if(loginKind==SIGNIN)
    {
        query.exec ("select * from users where username=\""+username+"\";");
        if(!query.next ())//数据库不存在此用户
            dsOut<<NOSUCHUSER;
        else
        {
            QString m_pwd=query.value (1).toString ();
            if(m_pwd!=pwd)
                dsOut<<PWDDIFF;
            else
            {
                User user;
                user.username=username;
                user.port=port;
                this->onlineUsers.append (user);

                dsOut<<SIGNINOK;
                SentPkm (dsOut,username);
            }
        }
        QHostAddress serverAddress = QHostAddress("127.0.0.1");
        client->writeDatagram(dataOut.data(), dataOut.size(),serverAddress, port);
    }
    else if(loginKind==SIGNOUT)
    {
        dsIn>>port;
        User user;
        user.username=username;
        user.port=port;
        this->onlineUsers.removeOne (user);
        qDebug()<<username<<"Login out";
        qDebug()<<this->onlineUsers.length ();
    }
    else if(loginKind==ONLINEUSERS)
    {
        dsIn>>port;
        this->SentOnlineUsers (port);
    }
    else if(loginKind==ALLUSERS)
    {
        dsIn>>port;
        this->SentAllUsers (port);
    }
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
