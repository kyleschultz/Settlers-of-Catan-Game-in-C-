/* Liz Parker and Kyle Schultz
 * Final Project checkpoint1
 * This program is a simplified version of Settlers of Catan
 */


#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QObject>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include "player.h"
#include <QDebug>

int MainWindow::dice_num_ = 2;
int MainWindow::currPlayer = 0;
int MainWindow::player_ = 0;
int MainWindow::gameStarted_ = 0;
int MainWindow::builds_ = 0;
int MainWindow::totalB = 0;
int MainWindow::totalC = 0;
int MainWindow::totalL = 0;
bool MainWindow::diceRolled_ = false;
bool MainWindow::buyingUpgraded_ = false;
bool MainWindow::robberMode_ = false;

/*
    MainWindow constructor sets up UI and pieces
    @param *parent is the QWidget used in our UI
*/
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    scene = new QGraphicsScene;
    ui->setupUi(this);
    QGraphicsView * view = ui->graphicsView;
    view->setScene(scene);
    view->setSceneRect(0,0,view->frameSize().width(),view->frameSize().height());
    srand(time(0));
    Resource ress[3] = {cattle, lumber, brick};
    int roll = rand()%10 +2; //make random 2-12
    if (roll == 7)
    { //7 is reserved for the robber, change roll to 8
        roll = 8;
    }
    int res = rand() %2; //make random 0-2

    for(int i = 0; i < 350; i+=50)
    {
        for(int j = 0; j < 350; j+=50)
        {
            //create piece to be added
            Piece * r = Piece::CreatePiece(ress[res], roll, i, j);
            connect(r, &Piece::SquareClicked, this, &MainWindow::OnSquareClicked);
            //update iterators
            if (res == 2)
            {
                res = 0;
            }
            else
            {
                res++;
            }
            if (roll == 12)
            {
                roll = 2;
            }
            else if (roll == 6)
            {
                roll = 8;
            }
            else
            {
                roll++;
            }

            //add pieces in appropriate position
            if(i == 0 && j == 150)
            {
                scene->addItem(r);
                pieces_.push_back(r);
            }
            else if(i == 50 && j >= 100 && j < 201)
            {
                scene->addItem(r);
                pieces_.push_back(r);
            }
            else if(i == 100 && j >= 50 && j < 251)
            {
                scene->addItem(r);
                pieces_.push_back(r);
            }
            else if(i == 150)
            {
                scene->addItem(r);
                pieces_.push_back(r);
            }
            else if(i == 200 & j >= 50 && j < 251)
            {
                scene->addItem(r);
                pieces_.push_back(r);
            }
            else if(i == 250 && j >= 100 && j < 201)
            {
                scene->addItem(r);
                pieces_.push_back(r);
            }
            else if(i == 300  && j == 150)
            {
                scene->addItem(r);
                pieces_.push_back(r);
            }
        }
    }

}

/*
    MainWindow destructor
*/
MainWindow::~MainWindow()
{
    delete ui;
}

/*
    Creates players, adds them to the player vector and calls takeFirstTurn
*/
void MainWindow::StartGame()
{

    //Build player colors
    QColor l1(153,255,225);
    QColor d1(0,153,153);
    QColor l2(255,255,153);
    QColor d2(204,204,0);
    QColor l3(160,160,160);
    QColor d3(64,64,64);
    //set up players
    Player *p1 = new Player("Player1", false, l1, d1); //automatically NOT computer for checkpoint 1
    Player *p2 = new Player("Player2", false, l2, d2); //computer boolean hardcoded to false
    Player *p3 = new Player("Player3", false, l3, d3);
    //add players to list
    players_.push_back(p1);
    players_.push_back(p2);
    players_.push_back(p3);
    playing_ = p1;
    takeFirstTurn();
}

/*
    Allows players to take their first turn and place a settlement
*/
void MainWindow::takeFirstTurn()
{
    gameStarted_++;
    if(player_ == 0)
    {
        playing_ = players_[0];
        ui->prompt->setText("It is " + QString(playing_->get_name().c_str()) + "'s turn! Click on an intersection\nto place your first town, then click 'Finish turn'");
        player_++;
    }
    else if(player_ == 1)
    {
        playing_ = players_[1];
        ui->prompt->setText("It is " + QString(playing_->get_name().c_str()) + "'s turn! Click on an intersection\nto place your first town, then click 'Finish turn'");
        player_++;
    }
    else if(player_ == 2)
    {
        playing_ = players_[2];
        ui->prompt->setText("It is " + QString(playing_->get_name().c_str()) + "'s turn! Click on an intersection\nto place your first town, then click 'Finish turn'");
        player_++;
        diceRolled_ = true; //fix bug in first turn
        player_ = 0;
    }

}

/*
    SquareClicked slot that is called when the signal is emitted that a player clicked a square. Places a settlement at that location

    @param x X-coord of click, y Y-coord of click
*/
void MainWindow::OnSquareClicked(int x, int y)
{
    QTransform t;
    QPointF p(x,y);
    QGraphicsView * view = ui->graphicsView;
    QGraphicsItem * curr = view->scene()->itemAt(p,t);
    Piece * current = dynamic_cast<Piece *>(curr);
    if (robberMode_)
    {
        scene->removeItem(theRobber_);
        theRobber_ = new Robber(current->get_x(), current->get_y());
        theRobber_->setBlock(current);
        scene->addItem(theRobber_);
        robberMode_ = false;
    }
    else
    {
        Settlement *temp;
        if(builds_ == 0)
        {
            //create a settlement if the player clicks close enough to one of the coners of a piece
            if(abs(x-current->get_x()) <= 10 && abs(y - current->get_y()) <= 10 && current->get_addmode() == true)
            {
                if (buyingUpgraded_){
                    temp = new Settlement(true, playing_->getDColor(), current->get_x(), current->get_y());
                    buyingUpgraded_ = false;
                } else {
                    temp = new Settlement(false, playing_->getLColor(), current->get_x(), current->get_y());
                }
                playing_->addSettlement(temp);
                scene->addItem(temp);
                temp->checkNeighbors(scene);
                playing_->checkPieces();
                playing_->updateResourceVals();
                builds_++;
            }

            else if(abs(x-current->get_x()-50) <= 10 && abs(y - current->get_y()) <= 10 && current->get_addmode() == true)
            {
                if (buyingUpgraded_){
                    temp = new Settlement(true, playing_->getDColor(), current->get_x(), current->get_y());
                    buyingUpgraded_ = false;
                } else {
                    temp = new Settlement(false, playing_->getLColor(), current->get_x(), current->get_y());
                }
                playing_->addSettlement(temp);
                scene->addItem(temp);
                temp->checkNeighbors(scene);
                playing_->checkPieces();
                playing_->updateResourceVals();
                builds_++;
            }

            else if(abs(x-current->get_x()) <= 10 && abs(y - current->get_y()-50) <= 10 && current->get_addmode() == true)
            {
                if (buyingUpgraded_){
                    temp = new Settlement(true, playing_->getDColor(), current->get_x(), current->get_y());
                    buyingUpgraded_ = false;
                } else {
                    temp = new Settlement(false, playing_->getLColor(), current->get_x(), current->get_y());
                }
                playing_->addSettlement(temp);
                scene->addItem(temp);
                temp->checkNeighbors(scene);
                playing_->checkPieces();
                playing_->updateResourceVals();
                builds_++;
            }

            else if(abs(x-current->get_x()-50) <= 10 && abs(y - current->get_y()-50) <= 10 && current->get_addmode() == true)
            {
                if (buyingUpgraded_){
                    temp = new Settlement(true, playing_->getDColor(), current->get_x(), current->get_y());
                    buyingUpgraded_ = false;
                } else {
                    temp = new Settlement(false, playing_->getLColor(), current->get_x(), current->get_y());
                }
                playing_->addSettlement(temp);
                scene->addItem(temp);
                temp->checkNeighbors(scene);
                playing_->checkPieces();
                playing_->updateResourceVals();
                builds_++;
            }
        }
        updatePanel();
    }
    view->scene()->update();
    repaint();
}


/*
    Allows players to take turns by rolling the dice
*/
void MainWindow::takeTurn()
{
    ui->prompt->setText("Roll the dice\nMake any desired purchases, then click 'Finish turn'");
    diceRolled_ = false;
    if(player_ == 0)
    {
        playing_ = players_[0];
        ui->prompt->setText("It is " + QString(playing_->get_name().c_str()) + "'s turn! Roll the dice,\nmake any desired purchases, then click 'Finish turn'");
        player_++;
    }
    else if(player_ == 1)
    {
        playing_ = players_[1];
        ui->prompt->setText("It is " + QString(playing_->get_name().c_str()) + "'s turn! Roll the dice,\nmake any desired purchases, then click 'Finish turn'");
        player_++;
    }
    else if(player_ == 2)
    {
        playing_ = players_[2];
        ui->prompt->setText("It is " + QString(playing_->get_name().c_str()) + "'s turn! Roll the dice,\nmake any desired purchases, then click 'Finish turn'");
        player_= 0;
    }
    update();
    repaint();
}

/*
    buyTown button slot that is called when the button is clicked. Checks if the player has enough resources to buy
*/
void MainWindow::on_buyTown_clicked()
{
    buyingUpgraded_ = false;
    //check if sufficient resources
    if (playing_->enoughResources(false)){
        //output corresponding prompt
        ui->prompt->setText("You bought a town! Click an intersection to place it,\nmake any desired purchases, then click 'Finish turn'");
        //put into build mode
        builds_ = 0;
        for(int i = 0; i < int(pieces_.size()); i++)
        {
            //put each piece into build mode
            pieces_[i]->set_addmode(true);
        }
        updatePanel();
    } else {
        //output corresponding prompt
        ui->prompt->setText("Sorry, not enough resources to buy a town.\nMake any desired purchases, then click 'Finish turn'");
    }
}

/*
    buyCity button slot that is called when the button is clicked. Checks if the player has enough resources to buy
*/
void MainWindow::on_buyCity_clicked()
{
    buyingUpgraded_ = true;
    //check if sufficient resources
    if (playing_->enoughResources(true)){
        //output corresponding prompt
        ui->prompt->setText("You bought a city! Click an intersection to place it,\nmake any desired purchases, then click 'Finish turn'");
        //put into build mode
        builds_ = 0;
        for(int i = 0; i < int(pieces_.size()); i++)
        {
            //put each piece into build mode
            pieces_[i]->set_addmode(true);
        }
        updatePanel();
    } else {
        //output corresponding prompt
        ui->prompt->setText("Sorry, not enough resources to buy a city.\nMake any desired purchases, then click 'Finish turn'");
    }
}
/*
    diceButton button slot that is called when the button is clicked. Generates a random number from 2-12 and changes
    resource values in the table
*/
void MainWindow::on_diceButton_clicked()
{
    //make sure not rolling during initial placement
    if (gameStarted_ > 0 && !diceRolled_){
        dice_num_ = rand() % 11 + 2;
        ui->dice->setText(QString(std::to_string(dice_num_).c_str()));
        repaint();
        //if 7, move the robber
        if (dice_num_ == 7)
        {
            robberMode_ = true;
            ui->prompt->setText("You rolled a 7!\nClick a square to place the Robber, then 'Finish turn'");
            update();
            repaint();
        }
        else {
        //distribute any resources gained as appropriate
            for (int i = 0; i < int(players_.size()); i++){
                Player *play = players_[i];
                vector<Piece*> pcs = play->getPieces();
                for (int j = 0; j < int(pcs.size()); j++){
                    if ((pcs[j]->getN() == dice_num_)&&(pcs[j]!=theRobber_->getBlock())){
                        play->changeResourceNum(pcs[j]->getR(), 1);
                        updatePanel();
                    }
                }
            }
        }
        diceRolled_ = true;
    }
}

/*
    finishTurnButton button slot that is called when the button is clicked. If it is the first time the button is
    clicked then call takeFirstTurn, otherwise call takeTurn
*/
void MainWindow::on_finishTurnButton_clicked()
{
    if(gameStarted_ ==  0)
    {
        //first click to start the game
        for(int i = 0; i < pieces_.size(); i++)
        {
            pieces_[i]->set_addmode(true); //allow placing of settlements
        }
        StartGame();
        ui->finishTurnButton->setText("Finish turn");
        builds_ = 0;
        repaint();
        update();
    } else if (gameStarted_ < 3){
        //first 3 turns
        for(int i = 0; i < pieces_.size(); i++)
        {
            pieces_[i]->set_addmode(true); //allow placing of settlements
        }
        builds_ = 0;
        takeFirstTurn();
        repaint();
        update();
    }
    else
    {
        for(int i = 0; i < int(pieces_.size()); i++)
        {
            pieces_[i]->set_addmode(false); //disable placing of settlements
        }
        if (diceRolled_ == false){
            ui->prompt->setText("Oops! You forgot to roll! Roll the dice,\nmake any desired purchases, then click 'Finish turn'");
        } else {
            takeTurn();
        }
    }
}

/*
    Updates the table of the state of the game by changing the resource labels for the players and totals
*/
void MainWindow::updatePanel(){
    int tb = 0;
    int tc = 0;
    int tl = 0;
    int bd, bu, cd, cu, ld, lu;
    for (int i = 0; i < int(players_.size()); i++){
        Player *play = players_[i];
        bd = play->getBrkDist();
        bu = play->getBrkUsed();
        cd = play->getCatDist();
        cu = play->getCatUsed();
        ld = play->getLumbDist();
        lu = play->getLumbUsed();
        tb += (bd);
        tc += (cd);
        tl += (ld);
        if (play->get_name() == "Player1"){
            ui->p1Brick->setText(QString(std::to_string(bd).c_str()));
            ui->p1BrickUsed->setText(QString(std::to_string(bu).c_str()));
            ui->p1Cattle->setText(QString(std::to_string(cd).c_str()));
            ui->p1CattleUsed->setText(QString(std::to_string(cu).c_str()));
            ui->p1Lumber->setText(QString(std::to_string(ld).c_str()));
            ui->p1LumberUsed->setText(QString(std::to_string(lu).c_str()));
        } else if (play->get_name() == "Player2"){
            ui->p2Brick->setText(QString(std::to_string(bd).c_str()));
            ui->p2BrickUsed->setText(QString(std::to_string(bu).c_str()));
            ui->p2Cattle->setText(QString(std::to_string(cd).c_str()));
            ui->p2CattleUsed->setText(QString(std::to_string(cu).c_str()));
            ui->p2Lumber->setText(QString(std::to_string(ld).c_str()));
            ui->p2LumberUsed->setText(QString(std::to_string(lu).c_str()));
        } else {
            ui->p3Brick->setText(QString(std::to_string(bd).c_str()));
            ui->p3BrickUsed->setText(QString(std::to_string(bu).c_str()));
            ui->p3Cattle->setText(QString(std::to_string(cd).c_str()));
            ui->p3CattleUsed->setText(QString(std::to_string(cu).c_str()));
            ui->p3Lumber->setText(QString(std::to_string(ld).c_str()));
            ui->p3LumberUsed->setText(QString(std::to_string(lu).c_str()));
        }
    }
    //set label for total resources distributed
    totalB = tb;
    totalC = tc;
    totalL = tl;
    ui->brickLabel->setText(QString(std::to_string(totalB).c_str()));
    ui->lumberLabel->setText(QString(std::to_string(totalL).c_str()));
    ui->cattleLabel->setText(QString(std::to_string(totalC).c_str()));
    //repaint();
}
