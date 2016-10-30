#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork>
#include <QButtonGroup> //按钮分组类头文件
#include <QMessageBox>
#include <QListWidgetItem>

#include"pokemon.h"
#include"highattack.h"
#include"highblood.h"
#include"highdefense.h"
#include"highspeed.h"
#include "login.h"

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

struct UserData
{
    QString username;//用户名
    QList<UDPPkm*> pkm;//小精灵

    ~UserData()
    {
        for(int i=0;i<3;i++)
            delete this->pkm[i];
    }

    //友元声明，一对用于支持QDataStream 输入输出的函数
    friend QDataStream &operator<<(QDataStream &stream, const UserData &userData);
    friend QDataStream &operator>>(QDataStream &stream, UserData &userData);
};

class Login;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0, Login *login = NULL);
    ~MainWindow();

private slots:
     void readPendingDatagrams();

     void RecvPkmID(int id); //接收Pkm单选按钮分组信号

     QString getInfo(uint index);//从数据包中获得Pkm数据
     void on_BtnLoginOut_clicked();

     void on_BtnOnline_clicked();

     void on_BtnAllUsers_clicked();

     void on_listWidgetUsers_currentRowChanged(int currentRow);

private:
    Ui::MainWindow *ui;

    QUdpSocket *server;
    QUdpSocket *client;

    UDPPkm **pkm;
    QString username;
    uint port;
    QList<User*> onlineUsers;
    QList<UserData*> allUsers;
    uint mode;//列表的显示模式
    QButtonGroup *m_pPkmGroup;   //性别Pkm按钮分组
//    Pokemon *pokemon;
};

#endif // MAINWINDOW_H
