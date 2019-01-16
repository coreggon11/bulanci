#ifndef PLAYER_H
#define PLAYER_H

#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QKeyEvent>
#include <QDebug>

#include "direction.h"
#include "defines.h"
#include "bullet.h"

class Client;

class Player : public QGraphicsRectItem
{
public:
    Player(int x, int y, int socket);
    int getSocket(){return socket;}
    void keyPressEvent(QKeyEvent * event);
    void setClient(Client * client);
    Client * getClient(){return client;}
    void move(QString way);
    void shoot();

private:
    int socket;
    Client * client;
    Direction facing;
};

#endif // PLAYER_H
