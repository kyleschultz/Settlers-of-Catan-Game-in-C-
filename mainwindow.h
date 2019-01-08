/* Liz Parker and Kyle Schultz
 * Final Project checkpoint1
 * This program is a simplified version of Settlers of Catan
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vector>
#include "player.h"
using namespace std;


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void StartGame();
    void takeTurn();
    void takeFirstTurn();
    void compTakeTurn();
    void updatePanel();

private slots:
    void on_buyTown_clicked();

    void on_buyCity_clicked();

    void on_diceButton_clicked();

    void OnSquareClicked(int x, int y);

    void on_finishTurnButton_clicked();

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    Player *playing_;
    static int dice_num_;
    Robber *theRobber_;
    vector<Piece*> pieces_;
    vector<Player*> players_;
    vector<Settlement*> settlements_;
    static int currPlayer;
    static int gameStarted_;
    static int player_;
    static int builds_;
    static int totalB;
    static int totalC;
    static int totalL;
    static bool diceRolled_;
    static bool buyingUpgraded_;
    static bool robberMode_;
};

#endif // MAINWINDOW_H
