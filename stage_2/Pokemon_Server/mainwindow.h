#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork>
#include <QtSql>
#include <QtTest>

#include"pokemon.h"
#include"highattack.h"
#include"highblood.h"
#include"highdefense.h"
#include"highspeed.h"

const uint SIGNUP=1;
const uint SIGNIN=2;
const uint NAMEEXIST=3;
const uint SIGNUPOK=4;
const uint NOSUCHUSER=5;
const uint PWDDIFF=6;
const uint SIGNINOK=7;
const uint PKMDATA=8;
const uint SIGNOUT=9;
const uint ONLINEUSERS=10;
const uint ALLUSERS=11;

struct User//在线用户
{
    QString username;//用户名
    uint port;//端口号

    bool operator==(const User &user) const
    {
        return this->username==user.username&&this->port==user.port;
    }

    //友元声明，一对用于支持QDataStream 输入输出的函数
    friend QDataStream &operator<<(QDataStream &stream, const User &user);
    friend QDataStream &operator>>(QDataStream &stream, User &user);
};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void CreatePkm(QString user);//随机分配小精灵
    void PutIntoSql(QString user, UDPPkm *pkm);
    void SentPkm(QDataStream &dsOut, QString user);//发送小精灵的数据
    void SentOnlineUsers(uint port);//发送在线用户
    void SentAllUsers(uint port);//发送注册用户

private slots:
    void readPendingDatagrams();

private:
    Ui::MainWindow *ui;

    QUdpSocket *server;
    QUdpSocket *client;
    QSqlDatabase *db;
    QList<User> onlineUsers;

    Pokemon *pokemon;
};

#endif // MAINWINDOW_H
