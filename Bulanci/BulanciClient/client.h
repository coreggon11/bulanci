#ifndef CLIENT_H
#define CLIENT_H

#include <QDataStream>
#include <QDialog>
#include <QTcpSocket>
#include <QStringList>
#include <QString>
#include <QVector>

#include "defines.h"
#include "player.h"

class QTcpSocket;
class QNetworkSession;

class Client : public QObject
{
    Q_OBJECT

public:
    explicit Client(QObject *parent = nullptr);
    ~Client();
    QVector<Player*> * getPlayers(){return players;}
    Player * getMyPlayer(){return myPlayer;}

private slots:
    void sendMessage(QString message);
    void readyRead();
    void sessionOpened();
    void onPress(int a);
    void onDisconnected();
    void onBulletCollision(QGraphicsItem * item);
    void onWin(Player * player);

private:
    QTcpSocket *tcpSocket = nullptr;
    QDataStream in;
    QNetworkSession *networkSession = nullptr;
    QVector<Player*> * players;
    Player * myPlayer;

signals:
    void addPlayer();
    void press(int a);
    void deletePlayer(Player * player);
    void end();

};

#endif // CLIENT_H
