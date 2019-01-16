#include <QApplication>
#include "client.h"

#include <QGraphicsView>
#include <QGraphicsScene>

#include "renderer.h"

int main(int argc, char *argv[])
{
    Renderer renderer;
    return renderer.exec(argc,argv);
}
