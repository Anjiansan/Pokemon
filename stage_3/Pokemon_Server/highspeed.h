#ifndef HIGHSPEED_H
#define HIGHSPEED_H

#include <QMainWindow>
#include <QObject>
#include <QWidget>
#include"pokemon.h"

class HighSpeed : public Pokemon
{
public:
    HighSpeed(uint qualification,uint skillIndex);
    HighSpeed(uint l,uint e,uint a,uint b,uint d,uint s);

    uint Attack() override;
    void LevelUP() override ;
};

#endif // HIGHSPEED_H
