#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include <QUdpSocket>

#include "mainwindow.h"
#include "pokemon.h"

const uint SIGNUP=1;
const uint SIGNIN=2;
const uint NAMEEXIST=3;
const uint SIGNUPOK=4;
const uint NOSUCHUSER=5;
const uint PWDDIFF=6;
const uint SIGNINOK=7;

namespace Ui {
class Login;
}

class Login : public QDialog
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = 0);
    ~Login();

    UDPPkm **getPkm();
    uint getPkmNum();
    QString getUsername();
    uint getPort();
private slots:

    void on_BtnSignUp_clicked();

    void on_BtnSignIn_clicked();

    void readPendingDatagrams();

private:
    Ui::Login *ui;

    QUdpSocket *client;
    QUdpSocket *server;

    uint userPkmNum;
    UDPPkm *pkm[9];
    QString username;
    uint port;
};

#endif // LOGIN_H
