#ifndef SERVER_H
#define SERVER_H

#include <QTcpSocket>
#include <QObject>
#include <QString>
#include <QVector>

#include "serverplayer.h"
#include "defines.h"

class QTcpServer;
class QNetworkSession;

class Server : public QObject
{
    Q_OBJECT

public:
    explicit Server(QObject * parent = nullptr);
    ~Server();
    void performAction(QTcpSocket * socket, QString message);
    void createNewPlayer(QTcpSocket * socket);
    void send(QTcpSocket * socket, QString message);
    void sendToAll(QString message);
    void exit();

signals:
    void createPlayers();

private slots:
    void sessionOpened();
    void clientConnected();
    void readyRead();
    void onCreatePlayers();
    void clientDisconnected();

private:
    QTcpServer * tcpServer = nullptr;
    QNetworkSession * networkSession = nullptr;
    QVector<ServerPlayer*> * players;
    QVector<QTcpSocket*> * sockets;
};

#endif
