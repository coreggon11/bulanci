#ifndef BULLET_H
#define BULLET_H

#include <QGraphicsRectItem>
#include <QTimer>
#include <QObject>
#include <QDebug>

#include "defines.h"
#include "direction.h"

class Player;

class Bullet : public QObject, public QGraphicsRectItem
{
    Q_OBJECT
public:
    Bullet(int x, int y, Direction facing, Player * player);

public slots:
    void move();

private:
    Direction facing;
    Player * player;

};

#endif // BULLET_H
