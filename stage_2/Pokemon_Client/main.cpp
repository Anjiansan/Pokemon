#include <QApplication>
#include<QDebug>

#include "mainwindow.h"
#include "login.h"
#include"pokemon.h"
#include"highattack.h"
#include"highblood.h"
#include"highdefense.h"
#include"highspeed.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Login login;
    if(login.exec ()==QDialog::Accepted)
    {
        MainWindow w(0,&login);
        w.show();
        return a.exec ();
    }
    else
        return 0;
}
