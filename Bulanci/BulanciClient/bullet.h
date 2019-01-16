#ifndef BULLET_H
#define BULLET_H

#include <QGraphicsRectItem>
#include <QTimer>
#include <QObject>

#include "defines.h"
#include "direction.h"

class Client;

class Bullet : public QObject, public QGraphicsRectItem
{
    Q_OBJECT
public:
    Bullet(int x, int y, Direction facing);

public slots:
    void move();

private:
    Direction facing;

};

#endif // BULLET_H
