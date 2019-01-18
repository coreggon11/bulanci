#include "bullet.h"
#include "player.h"

Bullet::Bullet(int x, int y, Direction facing, Player * player):
    QGraphicsRectItem(),
    facing(facing),
    player(player)
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
    for(auto * item : scene()->items())
        if(x() >= item->x() && x() <= item->x() + PLAYER_WIDTH
                && y() >= item->y() && y() <= item->y() + PLAYER_WIDTH
                && item != this && item != player && !(dynamic_cast<Bullet*>(item)))
        {
            player->addPoint();
            emit player->bulletCollision(item);
            scene()->removeItem(this);
            delete this;
            return;
        }
    //qDebug() << "an item";
}

