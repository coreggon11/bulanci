#include "player.h"
#include "client.h"

//#include <QDebug>

Player::Player(int x, int y, int socket):
    QGraphicsRectItem(),
    socket(socket),
    facing(Direction::RIGHT),
    points(0),
    timer(new QTimer()),
    dead(false),
    client(nullptr)
{
    setRect(0,0,PLAYER_WIDTH,PLAYER_WIDTH);
    setPos(x,y);
    setBrush(Qt::red);

    score = new QGraphicsTextItem;
    score->setPos(20,720 - ((y / 125)) * 20);
    score->setPlainText("Score : 0");
}

Player::Player(int x, int y, int socket, int order):
    Player(x,y,socket)
{
    QBrush brush;
    switch(order){
    case 0:
        brush = Qt::red;
        break;
    case 1:
        brush = Qt::green;
        break;
    case 2:
        brush = Qt::blue;
        break;
    case 3:
        brush = Qt::yellow;
        break;
    case 4:
    default:
        brush = Qt::cyan;
        break;
    }
    setBrush(brush);
    score->setDefaultTextColor(brush.color());
}

void Player::keyPressEvent(QKeyEvent *event)
{
    int a = event->key();
    emit client->press(a);
}

void Player::setClient(Client *client)
{
    this->client = client;
}

void Player::move(QString way)
{
    int originalX = x();
    int originalY = y();

    if(way == "L"){
        facing = Direction::LEFT;
        setPos(x() - 10, y());
    }else if(way == "R"){
        facing = Direction::RIGHT;
        setPos(x() + 10, y());
    }else if(way == "U"){
        facing = Direction::UP;
        setPos(x(), y() - 10);
    }else if(way == "D"){
        facing = Direction::DOWN;
        setPos(x(), y() + 10);
    }

    if(x() <= MIN_X)
        setPos(MIN_X, y());
    if(x() + PLAYER_WIDTH >= MAX_X)
        setPos(MAX_X - PLAYER_WIDTH, y());
    if(y() <= MIN_Y)
        setPos(x(), MIN_Y);
    if(y() + PLAYER_WIDTH >= MAX_Y)
        setPos(x(), MAX_Y - PLAYER_WIDTH);

    for(auto * item : scene()->items())
        if(dynamic_cast<QGraphicsItem*>(item) &&
            dynamic_cast<QGraphicsItem*>(item) != this &&
            !(dynamic_cast<Bullet*>(item)) &&
            this->collidesWithItem(item))
        {
            setPos(originalX, originalY);
        }

    qDebug() << "New Player position X: " << this->x() << " and Y: " << this->y() << endl;
}

void Player::shoot()
{
    int bulletX = this->x();
    int bulletY = this->y();

    Bullet * bullet = new Bullet(bulletX, bulletY, facing, this);
    scene()->addItem(bullet);

    qDebug() << "Bullet X: " << bulletX << " and Y:" << bulletY << endl;
}

void Player::die()
{
    removePoint();
    thisScene = scene();
    scene()->removeItem(this);
    connect(timer,SIGNAL(timeout()), this, SLOT(respawn()));

    timer->start(5000);
}

void Player::addPoint()
{
    ++points;
    score->setPlainText("Score : " + QString::number(points));
    if(points >= MAX_POINTS)
        emit win(this);
}

void Player::removePoint()
{
    --points;
    score->setPlainText("Score : " + QString::number(points));
}

void Player::init()
{
    scene()->addItem(score);
    thisScene = scene();
}

void Player::respawn()
{
    timer->stop();
    setPos(125,125);
    thisScene->addItem(this);
}
