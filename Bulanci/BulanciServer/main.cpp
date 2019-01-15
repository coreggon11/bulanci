#include <QApplication>

#include "server.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    Server server;
    //server.run();

    return app.exec();
}
