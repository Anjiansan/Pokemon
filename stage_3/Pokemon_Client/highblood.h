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
    ~HighBlood(){}

    uint Attack() override;
    void LevelUP() override ;
};

#endif // HIGHBLOOD_H
