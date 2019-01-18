#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <QTcpSocket>

#include "defines.h"

class ServerPlayer : public QObject
{
    Q_OBJECT
public:
    ServerPlayer(int x, int y, QTcpSocket * socket);
    int getX(){return x;}
    int getY(){return y;}
    QTcpSocket * getSocket(){return socket;}
    int getSocketDescriptor(){return socketDescriptor;}
    void move(QString way);
    void setPos(int x, int y);

private:
    int x;
    int y;
    int socketDescriptor;
    QTcpSocket * socket;

};

#endif // PLAYER_H
