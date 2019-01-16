#include "renderer.h"

Renderer::Renderer(QObject * parent):
    QObject(parent),
    addedPlayers(new QVector<Player*>())
{
}

Renderer::~Renderer()
{
    delete scene;
    delete view;
    delete app;
    delete client;
    delete addedPlayers;
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
        addedPlayers->append(player);
        scene->addItem(player);
        if(player == client->getMyPlayer()){
            player->setClient(client);
            player->setFlag(QGraphicsItem::ItemIsFocusable);
            player->setFocus();
        }
    }
}
