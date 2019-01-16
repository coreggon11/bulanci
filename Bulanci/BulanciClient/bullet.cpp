#include "bullet.h"
#include "client.h"

Bullet::Bullet(int x, int y, Direction facing):
    QGraphicsRectItem(),
    facing(facing)
{
    setRect(x,y,BULLET_WIDTH, BULLET_WIDTH);
    setPos(x,y);
    QTimer * timer = new QTimer();
    connect(timer,SIGNAL(timeout()), this, SLOT(move()));

    timer->start(15);
}

void Bullet::move()
{
    int velX = 0;
    int velY = 0;
    switch(facing){
    case Direction::LEFT:
        velX = - 10;
        break;
    case Direction::RIGHT:
        velX = 10;
        break;
    case Direction::UP:
        velY = -10;
        break;
    case Direction::DOWN:
        velY = 10;
        break;
    }
    setPos(x() + velX, y() + velY);
    if(x() < MIN_X || x() > MAX_X || y() < MIN_Y || y() > MAX_Y)
    {
        scene()->removeItem(this);
        delete this;
        return;
    }
}

