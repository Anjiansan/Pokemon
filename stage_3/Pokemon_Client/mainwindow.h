#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork>
#include <QButtonGroup> //按钮分组类头文件
#include <QMessageBox>
#include <QListWidgetItem>

#include "pokemon.h"
#include "highattack.h"
#include "highblood.h"
#include "highdefense.h"
#include "highspeed.h"
#include "login.h"
#include "fight.h"
#include "selectpkm.h"

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

struct UserData
{
    uint pkmLen;
    QString username;//用户名
    QList<UDPPkm*> pkm;//小精灵

    UserData(QString name,uint Pkmlen):username(name),pkmLen(Pkmlen){}

    ~UserData()
    {
        for(uint i=0;i<pkmLen;i++)
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

signals:
    void sentMsg(UDPPkm *self,UDPPkm *opponent);//发送数据给类Fight

    void sentSelect(QList<UDPPkm *> pkmList, uint *pkmIndex);//发送数据给类SelectPkm

private slots:
     void readPendingDatagrams();

     QString getInfo(uint index);//从数据包中获得Pkm数据

     QString getAdmPkmInfo(uint index);//从数据包中获得Pkm数据

     void on_BtnLoginOut_clicked();

     void on_BtnOnline_clicked();

     void on_BtnAllUsers_clicked();

     void on_listWidgetUsers_currentRowChanged(int currentRow);

     void on_BtnUpgrade_clicked();

     void on_BtnDuel_clicked();

     void on_BtnConfirm_clicked();

     void on_CbxUser_currentIndexChanged(int index);

     void on_comboBox_currentIndexChanged(int index);

     void recvResult(bool isWin);//接受战斗结果

     void recvSelectResult(uint index);//接受用户选择的小精灵

     void on_BtnRate_clicked();

     void on_BtnBadge_toggled(bool checked);

private:
    Ui::MainWindow *ui;

    QUdpSocket *server;
    QUdpSocket *client;

    uint userPkmNum;
    UDPPkm **pkm;
    QString username;
    uint port;
    QList<User*> onlineUsers;
    QList<UserData*> allUsers;
    uint mode;//列表的显示模式
    uint gameKind;//比赛模式

    uint adminPkmLen;//系统小精灵个数
    QList<UDPPkm*> adminPkm;//系统小精灵
//    Pokemon *pokemon;

    Fight *fight=new Fight(this);
    SelectPkm *selectPkm=new SelectPkm(this);

    uint pkmIndex[3];
};

#endif // MAINWINDOW_H
