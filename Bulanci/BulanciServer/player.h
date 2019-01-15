#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>

class Player : public QObject
{
    Q_OBJECT
public:
    Player();

private:
    int x;
    int y;

};

#endif // PLAYER_H
