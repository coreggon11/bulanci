#include <QtWidgets>
#include <QtNetwork>
#include <QtCore>
#include <QDebug>

#include "server.h"

Server::Server(QObject *parent):
    QObject(parent)
{
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
    connect(tcpServer, &QTcpServer::newConnection, this, &Server::sendFortune);
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
    QString ipAddress = QHostAddress(QHostAddress::LocalHost).toString();
    QString a = tr("The server is running on\n\nIP: %1\nport: %2\n\n"
                   "Run the Fortune Client example now.")
            .arg(ipAddress).arg(tcpServer->serverPort());
    qDebug() << a;
}

void Server::sendFortune()
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_10);

    QString msg = "esketit od servera";

    out << msg;

    QTcpSocket *clientConnection = tcpServer->nextPendingConnection();
    connect(clientConnection, &QAbstractSocket::disconnected,
            clientConnection, &QObject::deleteLater);

    clientConnection->write(block);
    clientConnection->disconnectFromHost();
}
