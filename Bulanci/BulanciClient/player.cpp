#include "player.h"
#include "client.h"

Player::Player(int x, int y, int socket):
    QGraphicsRectItem(),
    socket(socket),
    facing(Direction::RIGHT)
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
}

void Player::shoot()
{
    Bullet * bullet = new Bullet(x(), y(), facing);
    scene()->addItem(bullet);
}
