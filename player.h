/* Liz Parker and Kyle Schultz
 * Final Project checkpoint1
 * This program is a simplified version of Settlers of Catan
 */


#ifndef PLAYER_H
#define PLAYER_H
#include <vector>
#include <map>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMainWindow>
#include <QObject>
#include <QGraphicsItem>
using namespace std;
enum Resource { cattle, lumber, brick };

class Piece : public QObject, public QGraphicsItem {

    Q_OBJECT

public:
    static Piece* CreatePiece(Resource type, int n, int x, int y);
    //Piece(Resource r, int n, int x, int y);
    //~Piece();
    void set_x(int x) {x_ = x;}
    void set_y(int y) {y_ = y;}
    int get_x() {return x_;}
    int get_y() {return y_;}
    bool get_addmode() {return addmode_;}
    void set_addmode(bool mode) {addmode_ = mode;}
    virtual Resource getR() {return resource_;}
    int getN(){return num_; }
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;
protected:
    Resource resource_;
    QColor color_;
    int num_;
    bool robber_;
    int x_;
    int y_;
    const int width_ = 50;
    const int height_ = 50;
    bool addmode_;
signals:
    void SquareClicked(int x, int y);
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
};


class Cattle : public Piece{
public:
    Cattle(int n, int x, int y);
    Resource getR() {return cattle;}

};

class Lumber : public Piece{
public:
    Lumber(int n, int x, int y);
    Resource getR() {return lumber;}

};

class Brick : public Piece {
public:
    Brick(int n, int x, int y);
    Resource getR() {return brick;}

};


class Settlement : public QObject, public QGraphicsItem {

    Q_OBJECT

public:
    Settlement(bool up, QColor c, int x, int y);
    bool getIsUp(){return isUpgraded_; }
    int get_x() {return x_;}
    int get_y() {return y_;}
    vector<Piece*> get_pieces_touching() {return pieces_touching_;}
    void setIsUp(bool b){isUpgraded_ = b; }
    void checkNeighbors(QGraphicsScene* scene);
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;
private:
    vector<Piece*> pieces_touching_;
    bool isUpgraded_;
    int x_;
    int y_;
    QColor color_;
    const int width_ = 10;
};

class Robber : public QObject, public QGraphicsItem
{
    Q_OBJECT

public:
    Robber(int x, int y) {x_ = x; y_ = y; blocking_ = NULL; }
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;
    Piece* getBlock(){return blocking_;}
    void setBlock(Piece *i){blocking_ = i; }
private:
    int x_;
    int y_;
    Piece *blocking_;
    const QColor color_ = QColor(0,0,0);
    const int width_ = 30;
};

class Player
{
public:
    Player(string n, bool c, QColor colL, QColor colD);
    string get_name() {return name_;}
    vector<Piece*> getPieces() {return pieces_; }
    void checkPieces();
    bool enoughResources(bool up);
    void updateResourceVals();
    void buy(string item);
    bool getBuilding(){return building_; }
    void setBuilding(bool b){building_ = b; }
    void addSettlement(Settlement *s){settlements_.push_back(s); }
    QColor getLColor(){return lcolor_; }
    QColor getDColor(){return dcolor_; }
    //getter and setter for resources_ map
    int getResourceNum(Resource r){return resources_[r]; }
    void changeResourceNum(Resource r, int c);
    //getters and setters for panel values
    void setCatDist(int i){cat_r_dist += i; }
    int getCatDist(){return cat_r_dist; }
    void setCatUsed(int i){cat_r_used += i; }
    int getCatUsed(){return cat_r_used; }
    void setLumbDist(int i){lumb_r_dist += i; }
    int getLumbDist(){return lumb_r_dist; }
    void setLumbUsed(int i){lumb_r_used += i; }
    int getLumbUsed(){return lumb_r_used; }
    void setBrkDist(int i){brk_r_dist += i; }
    int getBrkDist(){return brk_r_dist; }
    void setBrkUsed(int i){brk_r_used += i; }
    int getBrkUsed(){return brk_r_used; }
private:
    string name_;
    bool isComputer_;
    bool building_;
    int points_;
    QColor lcolor_;
    QColor dcolor_;
    int cat_r_dist;
    int cat_r_used;
    int lumb_r_dist;
    int lumb_r_used;
    int brk_r_dist;
    int brk_r_used;
    bool first_add_;
    map<Resource, int> resources_;
    vector<Piece*> pieces_;
    vector<Settlement*> settlements_;


};

#endif // PLAYER_H
