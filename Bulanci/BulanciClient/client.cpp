#include <QtWidgets>
#include <QtNetwork>

#include "client.h"

Client::Client(QObject *parent)
    : QObject(parent),
      tcpSocket(new QTcpSocket(this)),
      players(new QVector<Player*>()),
      myPlayer(nullptr)
{
    // find out IP addresses of this machine
    in.setDevice(tcpSocket);
    in.setVersion(QDataStream::Qt_4_0);

    connect(tcpSocket, &QIODevice::readyRead, this, &Client::readyRead, Qt::DirectConnection);
    connect(tcpSocket, SIGNAL(disconnected()), this, SLOT(onDisconnected()), Qt::DirectConnection);

    QNetworkConfigurationManager manager;
    if (manager.capabilities() & QNetworkConfigurationManager::NetworkSessionRequired) {
        // Get saved network configuration
        QSettings settings(QSettings::UserScope, QLatin1String("QtProject"));
        settings.beginGroup(QLatin1String("QtNetwork"));
        const QString id = settings.value(QLatin1String("DefaultNetworkConfiguration")).toString();
        settings.endGroup();

        // If the saved network configuration is not currently discovered use the system default
        QNetworkConfiguration config = manager.configurationFromIdentifier(id);
        if ((config.state() & QNetworkConfiguration::Discovered) !=
                QNetworkConfiguration::Discovered) {
            config = manager.defaultConfiguration();
        }

        networkSession = new QNetworkSession(config, this);
        connect(networkSession, &QNetworkSession::opened, this, &Client::sessionOpened);
        networkSession->open();
    }
    tcpSocket->connectToHost(QHostAddress::LocalHost, SERVER_PORT);
}

Client::~Client()
{
    delete tcpSocket;
    delete networkSession;
    for(auto * player : *players)
        delete player;
    delete players;
}

void Client::sendMessage(QString message)
{
    QByteArray block = message.toUtf8();
    tcpSocket->write(block);
}

// actually ready read method
void Client::readyRead()
{
    in.startTransaction();

    QString nextFortune;
    in >> nextFortune;

    if (!in.commitTransaction())
        return;

    QStringList strings = nextFortune.split("#");

    if(strings[0] == "other"){
        int now = 1;
        while(now < strings.size()){
            if(strings[now] == "")
                break;
            if(strings[now] == "other")
                ++now;
            int x = QString(strings[now++]).toInt();
            int y = QString(strings[now++]).toInt();
            int socket = QString(strings[now++]).toInt();
            if(players->size() > 0)
                for(int i = 0; i < players->size(); ++i) {
                    Player * player = (*players)[i];
                    if(player->getSocket() == socket){
                        continue;
                    }
                    Player * newPlayer = new Player(x,y,socket);
                    players->append(newPlayer);
                    connect(newPlayer, SIGNAL(win(Player*)), this, SLOT(onWin(Player*)));
                    if(now >= strings.size())
                        myPlayer = newPlayer;
                }
            else{
                Player * player = new Player(x,y,socket);
                connect(player, SIGNAL(win(Player*)), this, SLOT(onWin(Player*)));
                players->append(player);
                if(now >= strings.size())
                    myPlayer = player;
            }
        }
        connect(this, SIGNAL(press(int)), SLOT(onPress(int)));
        emit addPlayer();
    }else if(strings[0] == "move") {
        int socket = QString(strings[2]).toInt();
        for(auto * player: *players){
            if(player->getSocket() == socket){
                player->move(strings[1]);
            }
        }
    }else if(strings[0] == "shoot"){
        int socket = QString(strings[1]).toInt();
        for(auto * player: *players){
            if(player->getSocket() == socket){
                player->shoot();
            }
        }
    }else if(strings[0] == "let"){
        int i;
        int socket;
        bool deletion = true;
        QVector<Player*> playersToDelete;
        for(auto * player : *players)
        {
            deletion = true;
            i = 1;
            while(i < strings.size())
            {
                socket = QString(strings[i]).toInt();
                if(player->getSocket() == socket)
                    deletion = false;
                ++i;
            }
            if(deletion)
            {
                playersToDelete.push_back(player);
            }
        }
        for(auto * player : playersToDelete)
        {
            players->removeAll(player);
            emit deletePlayer(player);
        }
    }

}

void Client::sessionOpened()
{
    // Save the used configuration
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

void Client::onPress(int a)
{
    switch(a)
    {
    case Qt::Key_Left:
        sendMessage("L");
        break;
    case Qt::Key_Right:
        sendMessage("R");
        break;
    case Qt::Key_Up:
        sendMessage("U");
        break;
    case Qt::Key_Down:
        sendMessage("D");
        break;
    case Qt::Key_Space:
        sendMessage("S");
        break;
    default:
        return;
    }
}

void Client::onDisconnected()
{
    sendMessage("disconnceted");
}

void Client::onBulletCollision(QGraphicsItem *item)
{
    Player * player;
    if(player = dynamic_cast<Player*>(item))
    {
        player->die();
    }
}

void Client::onWin(Player *player)
{
    sendMessage("win#" + QString::number(player->getSocket()));
    emit end();
}
