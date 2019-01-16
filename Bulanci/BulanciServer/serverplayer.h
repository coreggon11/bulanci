#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>

class ServerPlayer : public QObject
{
    Q_OBJECT
public:
    ServerPlayer(int x, int y, int socket);
    int getX(){return x;}
    int getY(){return y;}
    int getSocket(){return socket;}
    void move(QString way);
    void setPos(int x, int y);

private:
    int x;
    int y;
    int socket;

};

#endif // PLAYER_H
