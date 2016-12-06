#ifndef FIGHT_H
#define FIGHT_H

#include <QDialog>
#include <QTimer>

#include "pokemon.h"
#include "highattack.h"
#include "highblood.h"
#include "highdefense.h"
#include "highspeed.h"
#include "fight.h"

class MainWindow;

namespace Ui {
class Fight;
}

class Fight : public QDialog
{
    Q_OBJECT

public:
    explicit Fight(QWidget *parent = 0);
    ~Fight();

    QString getInfo(UDPPkm *currentPkm);
    void endFight();//战斗结束函数
public slots:
    void recvMsg(UDPPkm *self,UDPPkm *opponent);//接收数据

    void selfAttack();//用户攻击

    void oppoAttack();//对手攻击

signals:
    void sendResult(bool isWin);//发送战斗结果

private:
    Ui::Fight *ui;

    UDPPkm *self;
    UDPPkm *opponent;

    QTimer *selfTimer;//用户攻击的定时器
    QTimer *oppoTimer;//对手攻击的定时器

    void (*attackFunc[8])(UDPPkm *self, UDPPkm *opponent);//技能函数指针数组

    bool isSelfD;//用户是否使用防御技能
    bool isOppoD;//对手是否使用防御技能
    uint selfDefense;//用户防御
    uint oppoDefense;//对手防御
};

#endif // FIGHT_H
