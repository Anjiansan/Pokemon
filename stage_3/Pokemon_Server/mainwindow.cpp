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

    this->adminPkmLen=10;
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

void MainWindow::CreateOnePkm(uint index, uint pkmLevel)
{
    qsrand ((unsigned)time(NULL)*index);//随机类型
    uint kind=qrand ()%4;

    qsrand ((unsigned)time(NULL)*index*kind);//随机属性
    uint qualification=qrand ()%10;
    if(qualification<5)//50%几率S
        qualification=S;
    else if(qualification<8)//80%几率SS
        qualification=SS;
    else//20%几率SSS
        qualification=SSS;

    qsrand ((unsigned)time(NULL)*index*kind*qualification);//随机技能
    switch (kind) {
    case 0://HIGH_ATTACK
    {
        uint skillA=qrand ()%2;
        this->pokemon=new HighAttack(qualification,skillA);

        qsrand((unsigned)time(NULL)*index);//升级
        uint experience;
        if(pkmLevel==1)
            experience=qrand ()%50;
        else if(pkmLevel==2)
            experience=qrand()%(50*8*(8+1)/2-50*2*(2+1)/2)+50*2*(2+1)/2;
        else if(pkmLevel==3)
            experience=qrand()%(50*15*(15+1)/2-50*8*(8+1)/2)+50*8*(8+1)/2;
        else
            experience=50*15*(15+1)/2;
        this->pokemon->ExperienceUp (experience);

        UDPPkm *attrA=pokemon->getAttr ();

        this->adminPkm.append (attrA);//加入系统小精灵

        attrA=NULL;
        delete this->pokemon;
        this->pokemon=NULL;
        break;
    }
    case 1://HIGH_BLOOD
    {
        uint skillB=qrand ()%2+2;
        this->pokemon=new HighBlood(qualification,skillB);

        qsrand((unsigned)time(NULL)*index);//升级
        uint experience;
        if(pkmLevel==1)
            experience=qrand ()%50;
        else if(pkmLevel==2)
            experience=qrand()%(50*8*(8+1)/2-50*2*(2+1)/2)+50*2*(2+1)/2;
        else if(pkmLevel==3)
            experience=qrand()%(50*15*(15+1)/2-50*8*(8+1)/2)+50*8*(8+1)/2;
        else
            experience=50*15*(15+1)/2;
        this->pokemon->ExperienceUp (experience);

        UDPPkm *attrB=pokemon->getAttr ();

        this->adminPkm.append (attrB);//加入系统小精灵

        attrB=NULL;
        delete this->pokemon;
        this->pokemon=NULL;
        break;
    }
    case 2://HIGH_DEFENSE
    {
        uint skillD=qrand ()%2+4;
        this->pokemon=new HighDefense(qualification,skillD);

        qsrand((unsigned)time(NULL)*index);//升级
        uint experience;
        if(pkmLevel==1)
            experience=qrand ()%50;
        else if(pkmLevel==2)
            experience=qrand()%(50*8*(8+1)/2-50*2*(2+1)/2)+50*2*(2+1)/2;
        else if(pkmLevel==3)
            experience=qrand()%(50*15*(15+1)/2-50*8*(8+1)/2)+50*8*(8+1)/2;
        else
            experience=50*15*(15+1)/2;
        this->pokemon->ExperienceUp (experience);

        UDPPkm *attrD=pokemon->getAttr ();

        this->adminPkm.append (attrD);//加入系统小精灵

        attrD=NULL;
        delete this->pokemon;
        this->pokemon=NULL;
        break;
    }
    case 3://HIGH_SPEED
    {
        uint skillS=qrand ()%2+6;
        this->pokemon=new HighSpeed(qualification,skillS);

        qsrand((unsigned)time(NULL)*index);//升级
        uint experience;
        if(pkmLevel==1)
            experience=qrand ()%50;
        else if(pkmLevel==2)
            experience=qrand()%(50*8*(8+1)/2-50*2*(2+1)/2)+50*2*(2+1)/2;
        else if(pkmLevel==3)
            experience=qrand()%(50*15*(15+1)/2-50*8*(8+1)/2)+50*8*(8+1)/2;
        else
            experience=50*15*(15+1)/2;
        this->pokemon->ExperienceUp (experience);

        UDPPkm *attrS=pokemon->getAttr ();

        this->adminPkm.append (attrS);//加入系统小精灵

        attrS=NULL;
        delete this->pokemon;
        this->pokemon=NULL;
        break;
    }
    default:
        break;
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
    query.last();
    uint total=query.at()+1;
//    qDebug()<<total;
    dsOut<<total;//写入小精灵个数
    query.first ();
    do{
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
    }while(query.next ());
}

void MainWindow::SentAdminPkm(QDataStream &dsOut)
{
    dsOut<<this->adminPkmLen;
    for(uint i=0;i<this->adminPkmLen;i++)
    {
        dsOut<<*(this->adminPkm[i]);
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
        this->SentPkm (dsOut,query.value (0).toString ());
    }

    QHostAddress serverAddress = QHostAddress("127.0.0.1");
    client->writeDatagram(dataOut.data(), dataOut.size(),serverAddress, port);
}

void MainWindow::UpdatePkm(QString username, uint index, uint adminIndex)
{
    QSqlQuery query;
    query.exec ("select * from pokemon where user=\""+username+"\";");
    for(uint i=0;i<index;i++)
        query.next ();
    query.next ();
    UDPPkm udpPkm;
    udpPkm.level=query.value (2).toUInt ();
    udpPkm.experience=query.value (3).toUInt ();
    udpPkm.attack=query.value (4).toUInt ();
    udpPkm.blood=query.value (5).toUInt ();
    udpPkm.defense=query.value (6).toUInt ();
    udpPkm.speed=query.value (7).toUInt ();
    udpPkm.kind=query.value (8).toUInt ();
    udpPkm.attr=query.value (9).toUInt ();
    udpPkm.skill=query.value (10).toUInt ();

    switch(udpPkm.kind)
    {
        case HIGH_ATTACK:
            this->pokemon=new HighAttack(udpPkm.level,udpPkm.experience,udpPkm.attack,
                                         udpPkm.blood,udpPkm.defense,udpPkm.speed);
            this->pokemon->ExperienceUp (25+(this->adminPkm[adminIndex]->level-1)*15);
            break;
        case HIGH_BLOOD:
            this->pokemon=new HighBlood(udpPkm.level,udpPkm.experience,udpPkm.attack,
                                        udpPkm.blood,udpPkm.defense,udpPkm.speed);
            this->pokemon->ExperienceUp (25+(this->adminPkm[adminIndex]->level-1)*15);
            break;
        case HIGH_DEFENSE:
            this->pokemon=new HighDefense(udpPkm.level,udpPkm.experience,udpPkm.attack,
                                          udpPkm.blood,udpPkm.defense,udpPkm.speed);
            this->pokemon->ExperienceUp (25+(this->adminPkm[adminIndex]->level-1)*15);
            break;
        case HIGH_SPEED:
            this->pokemon=new HighSpeed(udpPkm.level,udpPkm.experience,udpPkm.attack,
                                        udpPkm.blood,udpPkm.defense,udpPkm.speed);
            this->pokemon->ExperienceUp (25+(this->adminPkm[adminIndex]->level-1)*15);
            break;
    }
    QList<uint> updated=this->pokemon->getUpdateInfo ();
    QString sqlString=tr("update pokemon set level=%1,experience=%2,attack=%3,"
                         "blood=%4,defense=%5,speed=%6 where user=\"%7\" and "
                         "attack=%8 and blood=%9 and attr=%10;").
            arg (updated[0]).arg (updated[1]).
            arg (updated[2]).arg (updated[3]).
            arg (updated[4]).arg (updated[5]).
            arg (username).arg (udpPkm.attack).
            arg (udpPkm.blood).arg (udpPkm.attr);

    query.exec (sqlString);

    delete this->pokemon;
}

void MainWindow::AddPkm(QString username, uint index)
{
    UDPPkm *pkm=this->adminPkm[index];

    QSqlQuery query;
    QString sqlString=tr("insert into pokemon values(\"%1\",\"%2\",%3,%4,%5,%6,%7,%8,%9,%10,%11);").
            arg (username).arg (pkm->name).arg (pkm->level).arg (pkm->experience).
            arg (pkm->attack).arg (pkm->blood).arg (pkm->defense).arg (pkm->speed).
            arg (pkm->kind).arg (pkm->attr).arg (pkm->skill);

    query.exec (sqlString);
}

void MainWindow::DeletePkm(QString username, uint index)
{
    QSqlQuery query;
    query.exec ("select * from pokemon where user=\""+username+"\";");
    query.last();
    uint total=query.at();

    query.exec ("select * from pokemon where user=\""+username+"\";");
    for(uint i=0;i<index;i++)
        query.next ();
    query.next ();
    UDPPkm udpPkm;
    udpPkm.name=query.value (1).toString ();
    udpPkm.level=query.value (2).toUInt ();
    udpPkm.experience=query.value (3).toUInt ();
    udpPkm.attack=query.value (4).toUInt ();
    udpPkm.blood=query.value (5).toUInt ();
    udpPkm.defense=query.value (6).toUInt ();
    udpPkm.speed=query.value (7).toUInt ();
    udpPkm.kind=query.value (8).toUInt ();
    udpPkm.attr=query.value (9).toUInt ();
    udpPkm.skill=query.value (10).toUInt ();

    QString sqlString=tr("delete from pokemon where user=\"%1\" and "
                         "attack=%2 and blood=%3 and attr=%4;").
            arg (username).arg (udpPkm.attack).
            arg (udpPkm.blood).arg (udpPkm.attr);

    query.exec (sqlString);

    query.exec ("select * from pokemon where user=\""+username+"\";");
    query.last();
    uint totalAfter=query.at();
    while(totalAfter+1<total)
    {
        sqlString=tr("insert into pokemon values(\"%1\",\"%2\",%3,%4,%5,%6,%7,%8,%9,%10,%11);").
                arg (username).arg (udpPkm.name).arg (udpPkm.level).
                arg (udpPkm.experience).arg (udpPkm.attack).arg (udpPkm.blood).
                arg (udpPkm.defense).arg (udpPkm.speed).arg (udpPkm.kind).
                arg (udpPkm.attr).arg (udpPkm.skill);

        query.exec (sqlString);

        totalAfter++;
    }
}

void MainWindow::UpdateUserPkm(QString username)
{
    QSqlQuery query;
    query.exec ("select * from pokemon where user=\""+username+"\";");
    if(!query.next ())
    {
//        qDebug()<<"Yes";

        qsrand ((unsigned)time(NULL));//随机类型
        uint kind=qrand ()%4;

        qsrand ((unsigned)time(NULL)*kind);//随机属性
        uint qualification=qrand ()%10;
        if(qualification<7)//70%几率S
            qualification=S;
        else if(qualification<9)//20%几率S
            qualification=SS;
        else//10%几率S
            qualification=SSS;

        qsrand ((unsigned)time(NULL)*kind*qualification);//随机技能
        switch (kind) {
        case 0://HIGH_ATTACK
        {
            uint skillA=qrand ()%2;
            this->pokemon=new HighAttack(qualification,skillA);
            UDPPkm *attrA=pokemon->getAttr ();

            this->PutIntoSql (username,attrA);//写入数据库中

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

            this->PutIntoSql (username,attrB);//写入数据库中

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

            this->PutIntoSql (username,attrD);//写入数据库中

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

            this->PutIntoSql (username,attrS);//写入数据库中

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
        qDebug()<<"SentOnlineUsers";
    }
    else if(loginKind==ALLUSERS)
    {
        dsIn>>port;
        this->SentAllUsers (port);
        qDebug()<<"SentAllUsers";
    }
    else if(loginKind==GETADMINPKM)
    {
        dsIn>>port;
        dsOut<<GETADMINPKM;

        this->adminPkm.clear ();
        for(uint i=0;i<3;i++)//随机生成3个一级系统小精灵
        {
            this->CreateOnePkm (i,1);
        }

        for(uint i=0;i<3;i++)//随机生成3个中级系统小精灵
        {
            this->CreateOnePkm (i,2);
        }

        for(uint i=0;i<2;i++)//随机生成2个高级系统小精灵
        {
            this->CreateOnePkm (i,3);
        }

        for(uint i=0;i<2;i++)//随机生成2个满级系统小精灵
        {
            this->CreateOnePkm (i,4);
        }

        this->SentAdminPkm (dsOut);
        QHostAddress serverAddress = QHostAddress("127.0.0.1");
        client->writeDatagram(dataOut.data(), dataOut.size(),serverAddress, port);
    }
    else if(loginKind==UPGRADE)
    {
        dsIn>>port;

        uint pkmIndex;
        dsIn>>pkmIndex;

        bool isWin;//战斗结果
        dsIn>>isWin;

        uint admPkmIndex;//系统用于战斗的小精灵
        dsIn>>admPkmIndex;

        if(isWin)
            query.exec("update users set win=win+1 where username=\""+username+"\";");
        else
            query.exec("update users set lose=lose+1 where username=\""+username+"\";");

        if(isWin)
            this->UpdatePkm (username,pkmIndex,admPkmIndex);//更新数据库的小精灵
        dsOut<<UPDATED;//发送更新后的小精灵
        SentPkm (dsOut,username);
        QHostAddress serverAddress = QHostAddress("127.0.0.1");
        client->writeDatagram(dataOut.data(), dataOut.size(),serverAddress, port);
    }
    else if(loginKind==DUEL)
    {
        dsIn>>port;

        uint pkmIndex;//用户用于战斗的小精灵
        dsIn>>pkmIndex;

        bool isWin;//战斗结果
        dsIn>>isWin;
        if(isWin)
            query.exec("update users set win=win+1 where username=\""+username+"\";");
        else
            query.exec("update users set lose=lose+1 where username=\""+username+"\";");

        uint admPkmIndex;//系统用于战斗的小精灵
        dsIn>>admPkmIndex;

        if(isWin)
        {
            this->UpdatePkm (username,pkmIndex,admPkmIndex);//升级数据库的小精灵
            this->AddPkm (username,admPkmIndex);//增加用户数据库中的小精灵
        }
        else
        {
            this->DeletePkm (username,pkmIndex);
//            qDebug()<<"deletes";
            this->UpdateUserPkm (username);
        }

        dsOut<<UPDATED;//发送更新后的小精灵
        SentPkm (dsOut,username);
        QHostAddress serverAddress = QHostAddress("127.0.0.1");
        client->writeDatagram(dataOut.data(), dataOut.size(),serverAddress, port);
    }
    else if(loginKind==WINNINGRATE)
    {
        dsIn>>port;

        dsOut<<WINNINGRATE;//发送用户的获胜率
        query.exec ("select * from users");
        while(query.next ())
        {
            dsOut<<query.value (0).toString ();//用户名
            dsOut<<query.value (2).toUInt ();//获胜场数
            dsOut<<query.value(3).toUInt ();//失败场数
        }
        QHostAddress serverAddress = QHostAddress("127.0.0.1");
        client->writeDatagram(dataOut.data(), dataOut.size(),serverAddress, port);
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
