#include "player.h"
#include "client.h"

//#include <QDebug>

Player::Player(int x, int y, int socket):
    QGraphicsRectItem(),
    socket(socket),
    facing(Direction::RIGHT),
    points(0),
    timer(new QTimer()),
    dead(false)
{
    setRect(x,y,PLAYER_WIDTH,PLAYER_WIDTH);
    setPos(x,y);
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
    if(way == "L"){
        facing = Direction::LEFT;
        setPos(x() - 10, y());
        this->rightArrowPressCount--;
    }else if(way == "R"){
        facing = Direction::RIGHT;
        setPos(x() + 10, y());
        this->rightArrowPressCount++;
    }else if(way == "U"){
        facing = Direction::UP;
        setPos(x(), y() - 10);
        this->upArrowKeyPressCount++;
    }else if(way == "D"){
        facing = Direction::DOWN;
        setPos(x(), y() + 10);
        this->upArrowKeyPressCount--;
    }

    if(x() < MIN_X)
        setPos(MIN_X, y());
    if(x() + PLAYER_WIDTH > MAX_X)
        setPos(MAX_X - PLAYER_WIDTH, y());
    if(y() < MIN_Y)
        setPos(x(), MIN_Y);
    if(y() + PLAYER_WIDTH > MAX_Y)
        setPos(x(), MAX_Y - PLAYER_WIDTH);

    qDebug() << "New Player position X: " << this->x() << " and Y: " << this->y() << endl;
}

void Player::shoot()
{
    qreal playerX = this->x();
    qreal playerY = this->y();

    qDebug() << "Player on position X: " << playerX << " and Y: " << playerY << endl;

    int bulletX = playerX - (this->rightArrowPressCount * 5);
    int bulletY = playerY + (this->upArrowKeyPressCount * 5);

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
    qDebug() << MAX_POINTS;
    if(points >= MAX_POINTS)
        emit win(this);
}

void Player::removePoint()
{
    --points;
}

void Player::respawn()
{
    timer->stop();
    setPos(125,125);
    thisScene->addItem(this);
}
