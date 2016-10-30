#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>


#include"pokemon.h"
#include"highattack.h"
#include"highblood.h"
#include"highdefense.h"
#include"highspeed.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void on_AttackBtn_clicked();

    void on_LevelUpBtn_clicked();

private:
    Ui::MainWindow *ui;

    Pokemon *pokemon;
};

#endif // MAINWINDOW_H
