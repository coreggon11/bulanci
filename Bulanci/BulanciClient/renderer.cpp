#include "renderer.h"

Renderer::Renderer(QObject * parent):
    QObject(parent),
    addedPlayers(new QVector<Player*>())
{
}

int Renderer::exec(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QApplication::setApplicationDisplayName(Client::tr("Bulanci"));

    view = new QGraphicsView();
    scene = new QGraphicsScene();
    scene->setSceneRect(0,0,1280,720);
    client = new Client();
    connect(client, SIGNAL(addPlayer()), this, SLOT(onAddPlayer()));
    connect(client, SIGNAL(deletePlayer(Player*)), this, SLOT(deletePlayer(Player*)));
    connect(client, SIGNAL(end()), this, SLOT(onEnd()));
    view->setScene(scene);

    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    view->show();

    return app.exec();
}

void Renderer::onAddPlayer()
{
    for(auto *player : *(client->getPlayers())) {
        if(addedPlayers->contains(player)){
            continue;
        }
        connect(player,SIGNAL(bulletCollision(QGraphicsItem*)),client ,SLOT(onBulletCollision(QGraphicsItem*)));
        addedPlayers->append(player);
        scene->addItem(player);
        if(player == client->getMyPlayer()){
            player->setClient(client);
            player->setFlag(QGraphicsItem::ItemIsFocusable);
            player->setFocus();
        }
    }
}

void Renderer::deletePlayer(Player *player)
{
    addedPlayers->removeAll(player);
    if(scene->items().contains(player))
        scene->removeItem(player);
    //delete player;
    //player = nullptr;
}

void Renderer::onEnd()
{
    /*for(auto * item : scene->items())
        scene->removeItem(item);*/
    qDebug() << "end";
}
