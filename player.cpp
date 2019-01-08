#include "player.h"
#include <vector>
#include <map>
#include <QtWidgets>

/* Liz Parker and Kyle Schultz
 * Final Project checkpoint1
 * This program is a simplified version of Settlers of Catan
 */

//Constructor for Player object
Player::Player(string n, bool c, QColor colL, QColor colD)
{
    name_ = n;
    isComputer_ = c;
    points_ = 2;
    lcolor_ = colL;
    dcolor_ = colD;
    cat_r_dist = 0;
    cat_r_used = 0;
    lumb_r_dist = 0;
    lumb_r_used = 0;
    brk_r_dist = 0;
    brk_r_used = 0;
    first_add_ = true;
    resources_[brick]=0;
    resources_[cattle]=0;
    resources_[lumber]=0;
}

//causing errors so commented out
bool Player::enoughResources(bool up){
    int brk = brk_r_dist - brk_r_used;
    int lumb = lumb_r_dist - lumb_r_used;
    int cat = cat_r_dist - cat_r_used;
    if (up){
        //trying to buy a city
        if (cat >= 3 && lumb >= 3){
            //erase resources from resource_ list and update labels
            cat_r_used += 3;
            lumb_r_used +=3;
            changeResourceNum(cattle, -3);
            changeResourceNum(lumber, -3);
            return true;
        } else {
            return false;
        }
    } else {
        //trying to buy a town
        if (cat >= 2 && lumb >= 1 && brk >= 1){
            //erase resources from resource_ list and update labels
            cat_r_used += 2;
            lumb_r_used +=1;
            brk_r_used += 1;
            changeResourceNum(cattle, -2);
            changeResourceNum(lumber, -1);
            changeResourceNum(brick, -1);
            return true;
        } else {
            return false;
        }
    }
}

/*
    Checks the types of pieces near a players settlemets and adds to their resources if it is the first turn

*/
void Player::checkPieces(){
    for (int i = 0; i < int(settlements_.size()); i++){
        Settlement *temp = settlements_[i];
        for (int j = 0; j < int(temp->get_pieces_touching().size()); j++){
            pieces_.push_back(temp->get_pieces_touching()[j]);
        }
    }
    //add resources for first time
    if (first_add_){
        for (int i = 0; i < int(pieces_.size()); i++){
            Piece *tmp = pieces_[i];
            resources_[tmp->getR()] += 1;
        }
    }

}

/*
    Changes accounting of player's available and distributed resources
 */

void Player::changeResourceNum(Resource r, int c)
{
    resources_[r]+=c;
    if (c>0){
        if (r == cattle){
            cat_r_dist += c;
        } else if (r == lumber){
            lumb_r_dist += c;
        } else {
            brk_r_dist += c;
        }
    }
}

/*
    Updated the resources distributed to a player based on the value of the resources map, only used in game set up

*/
void Player::updateResourceVals(){
    if (first_add_){
        cat_r_dist = resources_[cattle];
        lumb_r_dist = resources_[lumber];
        brk_r_dist = resources_[brick];
        first_add_ = false;
    }
}

/*
    Static factory method that creates a certain subclass of a piece type and returns it

    @param type The type of piece to be created, n The piece number, x Pieces x-coord, y Pieces y-coord
    @return New piece object
*/
Piece* Piece::CreatePiece(Resource type, int n, int x, int y)
{
    if(type == cattle)
    {
        return new Cattle(n,x,y);
    }
    else if(type == lumber)
    {
        return new Lumber(n,x,y);
    }
    else
    {
        return new Brick(n,x,y);
    }
}

/*
 Constructor for cattle piece

 @param n Cattle piece number, x Cattle Piece x-coord, y Cattle Piece y-coord
*/
Cattle::Cattle(int n, int x, int y)
{
    addmode_ = false;
    num_ = n;
    robber_ = false;
    x_ = x;
    y_= y;
    QColor c(65,105,225);
    color_ = c;
}

/*
 Constructor for lumber piece

 @param n Lumber piece number, x Lumber Piece x-coord, y Lumber Piece y-coord
*/
Lumber::Lumber(int n, int x, int y)
{
    addmode_ = false;
    num_ = n;
    robber_ = false;
    x_ = x;
    y_= y;
    QColor c(34,139,34);
    color_ = c;
}

/*
 Constructor for brick piece

 @param n Brick piece number, x Brick Piece x-coord, y Brick Piece y-coord
*/
Brick::Brick(int n, int x, int y)
{
    addmode_ = false;
    num_ = n;
    robber_ = false;
    x_ = x;
    y_= y;
    QColor c(178,34,34);
    color_ = c;
}


/*
    Builds a QRectF with xy position, height, and width determined by Piece fields
    @return the built QRectF
*/
QRectF Piece::boundingRect() const
{
    return QRectF(x_, y_, width_, height_);
}


/*
    Builds a QPainterPath and adds rectangle with xy position, height, and width determined by Piece fields
    @return the built path
*/
QPainterPath Piece::shape() const
{
    QPainterPath path;
    path.addRect(x_, y_, width_, height_);
    return path;
}


/*
    Sets up QBrush and painter to paint UI as desired
    @param painter, option, and widget are QT objects that hold and perform our UI preferences
*/
void Piece::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);

    QBrush b = painter->brush();
    painter->setBrush(QBrush(color_.dark(option->state & QStyle::State_Sunken ? 100 : 100)));

    painter->drawRect(QRect(this->x_, this->y_, this->width_, this->height_));
    const QString tm = QString::fromStdString(to_string(this->num_));
    painter->drawText(QRect(this->x_, this->y_, this->width_, this->height_), Qt::AlignCenter, tm);
    painter->setBrush(b);
}

/*
    Routes event to emit signals
    @param is the event done by user that causes mousePressEvent to be called
*/
void Piece::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->buttons() == Qt::LeftButton)
    {
        emit Piece::SquareClicked(int(event->pos().x()), int(event->pos().y()));
    }
}

/*
    constructor for settlement object
*/
Settlement::Settlement(bool up, QColor c, int x, int y){
    isUpgraded_ = up;
    color_ = c;
    x_ = x;
    y_ = y;
}

/*
    Checks the types of pieces around a settlement and adds the to the pieces_touching_ vector

    @param scene The QGraphicsScene with the settlement so the itemAt function can be used
*/
void Settlement::checkNeighbors(QGraphicsScene* scene)
{
    vector <QGraphicsItem*> is_touching;
    QTransform t;
    QPointF p0(x_+10,y_+10);    //piece to upper right
    QPointF p1(x_-10,y_-10);    //piece to lower left
    QPointF p2(x_-10,y_+10);    //piece to upper left
    QPointF p3(x_+10,y_-10);    //piece to lower right
    QGraphicsItem * curr0 = scene->itemAt(p0,t);
    is_touching.push_back(curr0);
    QGraphicsItem * curr1 = scene->itemAt(p1,t);
    is_touching.push_back(curr1);
    QGraphicsItem * curr2 = scene->itemAt(p2,t);
    is_touching.push_back(curr2);
    QGraphicsItem * curr3 = scene->itemAt(p3,t);
    is_touching.push_back(curr3);
    for(int i = 0; i < is_touching.size(); i++)
    {
        if(is_touching[i] != 0) //itemAt will return 0 if there is no item at that point
        {
            Piece * current = dynamic_cast<Piece *>(is_touching[i]);
            pieces_touching_.push_back(current);
        }
    }
    qDebug() << "pieces" << pieces_touching_.size();
}

/*
    Builds a QRectF with xy position, height, and width determined by Settlement fields
    @return the built QRectF
*/
QRectF Settlement::boundingRect() const
{

    return QRectF(x_, y_, width_, width_);
}

/*
    Builds a QPainterPath and adds rectangle with xy position, height, and width determined by Settlement fields
    @return the built path
*/
QPainterPath Settlement::shape() const
{

    QPainterPath path;
    path.addEllipse(x_, y_, width_, width_);
    return path;
}
/*
    Sets up QBrush and painter to paint UI as desired
    @param painter, option, and widget are QT objects that hold and perform our UI preferences
*/
void Settlement::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);


    QBrush b = painter->brush();
    painter->setBrush(QBrush(color_.dark(option->state & QStyle::State_Sunken ? 120 : 100)));

    painter->drawEllipse(QRect(this->x_-5, this->y_-5, this->width_, this->width_));
    painter->setBrush(b);
}

/*
    Builds a QRectF with xy position, height, and width determined by Robber fields
    @return the built QRectF
*/
QRectF Robber::boundingRect() const
{

    return QRectF(x_, y_, width_, width_);
}

/*
    Builds a QPainterPath and adds elipse with xy position, height, and width determined by Robber fields
    @return the built path
*/
QPainterPath Robber::shape() const
{

    QPainterPath path;
    path.addEllipse(x_, y_, width_, width_);
    return path;
}
/*
    Sets up QBrush and painter to paint UI as desired
    @param painter, option, and widget are QT objects that hold and perform our UI preferences
*/
void Robber::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);


    QBrush b = painter->brush();
    painter->setBrush(QBrush(color_.dark(option->state & QStyle::State_Sunken ? 120 : 100)));

    painter->drawEllipse(QRect(this->x_+10, this->y_+10, this->width_, this->width_));
    painter->setBrush(b);
}

