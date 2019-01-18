#include "serverplayer.h"

ServerPlayer::ServerPlayer(int x, int y, QTcpSocket * socket):
    x(x), y(y), socket(socket)
{
    socketDescriptor = socket == nullptr? -1 : socket->socketDescriptor();
}

void ServerPlayer::move(QString way)
{
    if(way == "L"){
        setPos(x - 10, y);
    }else if(way == "R"){
        setPos(x + 10, y);
    }else if(way == "U"){
        setPos(x, y - 10);
    }else if(way == "D"){
        setPos(x, y + 10);
    }
}

void ServerPlayer::setPos(int x, int y){

    this->x = x;
    this->y = y;
    if(x < MIN_X)
        x= MIN_X;
    if(x > MAX_X - PLAYER_WIDTH)
        x= MAX_X - PLAYER_WIDTH;
    if(y < MIN_Y)
        y= MIN_Y;
    if(y > MAX_Y - PLAYER_WIDTH)
        x= MAX_Y - PLAYER_WIDTH;

}
