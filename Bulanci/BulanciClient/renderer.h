#ifndef RENDERER_H
#define RENDERER_H

#include <QApplication>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QVector>

#include "client.h"
#include "player.h"

class Renderer : public QObject
{
    Q_OBJECT
public:
    Renderer(QObject * parent = nullptr);
    ~Renderer();
    int exec(int argc, char *argv[]);
public slots:
    void onAddPlayer();
private:
    QApplication * app;
    QGraphicsView * view;
    QGraphicsScene * scene;
    Client * client;
    QVector<Player*> * addedPlayers;
};

#endif // RENDERER_H
