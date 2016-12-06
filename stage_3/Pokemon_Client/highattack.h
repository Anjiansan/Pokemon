#ifndef HIGHATTACK_H
#define HIGHATTACK_H

#include <QMainWindow>
#include <QObject>
#include <QWidget>
#include"pokemon.h"

class HighAttack : public Pokemon
{
    Q_OBJECT
public:
//    explicit HighAttack(QObject *parent = 0);
    HighAttack(uint qualification,uint skillIndex);
    ~HighAttack(){}

    uint Attack() override;
    void LevelUP() override ;

signals:

public slots:
};

#endif // HIGHATTACK_H
