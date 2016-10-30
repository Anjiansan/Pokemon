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
    ~HighSpeed(){};

    uint Attack() override;
    void LevelUP() override ;
};

#endif // HIGHSPEED_H
