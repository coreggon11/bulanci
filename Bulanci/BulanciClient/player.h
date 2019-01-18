#ifndef PLAYER_H
#define PLAYER_H

#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QKeyEvent>
#include <QDebug>
#include <QObject>
#include <QTimer>

#include "direction.h"
#include "defines.h"
#include "bullet.h"

class Client;

class Player : public QObject, public QGraphicsRectItem
{
    Q_OBJECT
public:
    Player(int x, int y, int socket);
    int getSocket(){return socket;}
    void keyPressEvent(QKeyEvent * event);
    void setClient(Client * client);
    Client * getClient(){return client;}
    void move(QString way);
    void shoot();
    void die();
    void addPoint();
    void removePoint();

signals:
    void bulletCollision(QGraphicsItem * item);
    void win(Player * player);

public slots:
    void respawn();

private:
    int points;
    int socket;
    Client * client;
    Direction facing;
    QTimer * timer;
    bool dead;
    QGraphicsScene * thisScene;

    int rightArrowPressCount = 0;
    int upArrowKeyPressCount = 0;
};

#endif // PLAYER_H
