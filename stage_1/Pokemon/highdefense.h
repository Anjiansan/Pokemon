#ifndef HIGHDEFENSE_H
#define HIGHDEFENSE_H

#include <QMainWindow>
#include <QObject>
#include <QWidget>
#include"pokemon.h"

class HighDefense : public Pokemon
{
public:
    HighDefense(uint qualification,SKILL skillIndex);
    ~HighDefense(){}

    uint Attack() override;
    void LevelUP() override ;
};

#endif // HIGHDEFENSE_H
