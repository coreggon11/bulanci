#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QString>
#include <QVector>

#include "player.h"
#include "defines.h"

class QTcpServer;
class QNetworkSession;

class Server : public QObject
{
    Q_OBJECT

public:
    explicit Server(QObject * parent = nullptr);

private slots:
    void sessionOpened();
    void sendFortune();

private:
    QTcpServer * tcpServer = nullptr;
    QNetworkSession * networkSession = nullptr;
    QVector<Player*> * players;
};

#endif
