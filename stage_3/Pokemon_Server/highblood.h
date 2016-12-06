#ifndef HIGHBLOOD_H
#define HIGHBLOOD_H

#include <QMainWindow>
#include <QObject>
#include <QWidget>
#include"pokemon.h"

class HighBlood : public Pokemon
{
public:
    HighBlood(uint qualification,uint skillIndex);
    HighBlood(uint l,uint e,uint a,uint b,uint d,uint s);
    ~HighBlood(){}

    uint Attack() override;
    void LevelUP() override ;
};

#endif // HIGHBLOOD_H
