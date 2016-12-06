#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork>
#include <QtSql>
#include <QtTest>

#include "pokemon.h"
#include "highattack.h"
#include "highblood.h"
#include "highdefense.h"
#include "highspeed.h"

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
const uint GETADMINPKM=12;//获取系统小精灵
const uint UPGRADE=13;//升级赛
const uint DUEL=14;//决斗赛
const uint UPDATED=15;//更新后小精灵
const uint WINNINGRATE=16;//获胜率

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
    void CreateOnePkm(uint index, uint pkmLevel);//随机生成一个小精灵
    void PutIntoSql(QString user, UDPPkm *pkm);//写入数据库
    void SentPkm(QDataStream &dsOut, QString user);//发送小精灵的数据
    void SentAdminPkm(QDataStream &dsOut);//发送系统小精灵
    void SentOnlineUsers(uint port);//发送在线用户
    void SentAllUsers(uint port);//发送注册用户
    void UpdatePkm(QString username, uint index, uint adminIndex);//更新用户升级的小精灵
    void AddPkm(QString username, uint index);//用户增加小精灵，更新数据库
    void DeletePkm(QString username, uint index);//用户删除小精灵，更新数据库
    void UpdateUserPkm(QString username);//判断用户是否没有小精灵并分配

private slots:
    void readPendingDatagrams();

private:
    Ui::MainWindow *ui;

    QUdpSocket *server;
    QUdpSocket *client;
    QSqlDatabase *db;
    QList<User> onlineUsers;

    Pokemon *pokemon;
    uint adminPkmLen;//系统小精灵个数
    QList<UDPPkm*> adminPkm;
};

#endif // MAINWINDOW_H
