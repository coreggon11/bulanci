#include <QtWidgets>
#include <QtNetwork>
#include <QtCore>
#include <QDebug>
#include <QApplication>

#include "server.h"

Server::Server(QObject *parent):
    QObject(parent)
{
    players = new QVector<ServerPlayer*>();
    sockets = new QVector<QTcpSocket*>();
    // set up network
    QNetworkConfigurationManager manager;
    if (manager.capabilities() & QNetworkConfigurationManager::NetworkSessionRequired) {
        QSettings settings(QSettings::UserScope, QLatin1String("QtProject"));
        settings.beginGroup(QLatin1String("QtNetwork"));
        const QString id = settings.value(QLatin1String("DefaultNetworkConfiguration")).toString();
        settings.endGroup();
        QNetworkConfiguration config = manager.configurationFromIdentifier(id);
        if ((config.state() & QNetworkConfiguration::Discovered) !=
                QNetworkConfiguration::Discovered) {
            config = manager.defaultConfiguration();
        }

        networkSession = new QNetworkSession(config, this);
        connect(networkSession, &QNetworkSession::opened, this, &Server::sessionOpened);
        networkSession->open();
    } else {
        sessionOpened();
    }
    // connect signals and slots
    connect(tcpServer, &QTcpServer::newConnection, this, &Server::clientConnected);
    connect(this, SIGNAL(createPlayers()), SLOT(onCreatePlayers()));
}

Server::~Server()
{
    exit();
}

void Server::performAction(QTcpSocket *socket, QString message)
{
    if(message == "L" || message == "R" || message == "U" || message == "D")
    {
        QString newMessage = "move#" + message +
                "#" + QString::number(socket->socketDescriptor());
        for(auto * player : *players){
            if(player->getSocketDescriptor() == socket->socketDescriptor()){
                player->move(message);
            }
        }
        sendToAll(newMessage);
    } else if (message == "S"){
        QString newMessage = "shoot#" + QString::number(socket->socketDescriptor());
        sendToAll(newMessage);
    } else {
        if(message == "w")
        {
            QString rest = socket->readAll().data();
            QStringList messages = rest.split("#");
            QString winner = messages[1];
            for(int i = 1; i < players->size() + 1; i++)
            {
                if((*players)[i-1]->getSocketDescriptor() == winner.toInt()){
                    QString newMessage = "win#Player " + QString::number(i) + " won!";
                    sendToAll(newMessage);
                }
            }
        }
    }
}

void Server::createNewPlayer(QTcpSocket *socket)
{
    int x = (sockets->size() + 1) * 125;
    int y = (sockets->size() + 1) * 125;

    QString message = "";
    for(auto * player : *players){
        message += "other#";
        message += QString::number(player->getX());
        message += "#";
        message += QString::number(player->getY());
        message += "#";
        message += QString::number(player->getSocketDescriptor());
        message += "#";
    }

    message +="other#"+QString::number(x)+"#"+QString::number(y)+
            "#" + QString::number(socket->socketDescriptor());
    send(socket, message);
    ServerPlayer * player = new ServerPlayer(x,y,socket);
    players->append(player);

    emit createPlayers();
}

void Server::send(QTcpSocket *socket, QString message)
{
    socket->flush();
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_10);

    out << message;

    socket->write(block);
}

void Server::sendToAll(QString message)
{
    for(auto * socket : *sockets){
        send(socket, message);
    }
}

void Server::exit()
{
    networkSession->close();
    qDebug() << "blabla";
    QString message = "let";
    for(ServerPlayer * player : *players){
        message += "#" + QString::number(player->getSocketDescriptor());
    }
    if(players->size() > 0)
        sendToAll(message);
    delete tcpServer;
    delete networkSession;
    for(auto * player : * players)
        delete player;
    delete players;
    for(auto * socket : * sockets)
        delete socket;
    delete sockets;
}

void Server::sessionOpened()
{
    // Save the used configuration
    if (networkSession) {
        QNetworkConfiguration config = networkSession->configuration();
        QString id;
        if (config.type() == QNetworkConfiguration::UserChoice)
            id = networkSession->sessionProperty(QLatin1String("UserChoiceConfiguration")).toString();
        else
            id = config.identifier();

        QSettings settings(QSettings::UserScope, QLatin1String("QtProject"));
        settings.beginGroup(QLatin1String("QtNetwork"));
        settings.setValue(QLatin1String("DefaultNetworkConfiguration"), id);
        settings.endGroup();
    }
    tcpServer = new QTcpServer(this);
    if (!tcpServer->listen(QHostAddress::Any, SERVER_PORT)) {
        qDebug() << "Could not start the server.";
        return;
    }

    qDebug() << "The server is running on port" << tcpServer->serverPort();
}

void Server::clientConnected()
{
    QTcpSocket *clientConnection = tcpServer->nextPendingConnection();
    connect(clientConnection, &QAbstractSocket::disconnected,
            clientConnection, &QObject::deleteLater);
    connect(clientConnection, SIGNAL(disconnected()), this, SLOT(clientDisconnected()));

    qDebug() << clientConnection->socketDescriptor() <<" client connected";

    connect(clientConnection, SIGNAL(readyRead()), this, SLOT(readyRead()), Qt::DirectConnection);
    createNewPlayer(clientConnection);

    sockets->append(clientConnection);

}

void Server::readyRead()
{
    QByteArray msg;
    for(QTcpSocket * socket : *sockets){
        msg = socket->read(1);
        performAction(socket, msg);
        socket->readAll();
    }
}

void Server::onCreatePlayers()
{
    QString message = "";
    for(auto * player : *players){
        message += "other#";
        message += QString::number(player->getX());
        message += "#";
        message += QString::number(player->getY());
        message += "#";
        message += QString::number(player->getSocketDescriptor());
        message += "#";
    }
    qDebug() << message;
    sendToAll(message);
}

void Server::clientDisconnected()
{
    qDebug() << "client disconnected";
    QVector<QTcpSocket*> * tmp_sockets = new QVector<QTcpSocket*>();
    for(QTcpSocket * socket: *sockets){
        if(socket->state() == QTcpSocket::UnconnectedState)
        {
            tmp_sockets->push_back(socket);
        }
    }
    for(QTcpSocket * socket : *tmp_sockets)
    {
        sockets->removeAll(socket);
    }
    for(QTcpSocket * socket : *tmp_sockets)
    {
        for(ServerPlayer * player : *players){
            if(player->getSocket() == socket)
            {
                players->removeAll(player);
                delete player;
                player = nullptr;
            }
        }
    }
    tmp_sockets->clear();
    delete tmp_sockets;
    tmp_sockets = nullptr;
    QString message = "let";
    for(ServerPlayer * player : *players){
        message += "#" + QString::number(player->getSocketDescriptor());
    }
    if(players->size() > 0)
        sendToAll(message);
}
