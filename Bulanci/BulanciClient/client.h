#ifndef CLIENT_H
#define CLIENT_H

#include <QDataStream>
#include <QDialog>
#include <QTcpSocket>

#include "defines.h"

class QComboBox;
class QLabel;
class QLineEdit;
class QPushButton;
class QTcpSocket;
class QNetworkSession;

class Client : public QDialog
{
    Q_OBJECT

public:
    explicit Client(QWidget *parent = nullptr);

private slots:
    void requestNewFortune();
    void readFortune();
    void displayError(QAbstractSocket::SocketError socketError);
    void sessionOpened();

private:
    QPushButton *getFortuneButton = nullptr;

    QTcpSocket *tcpSocket = nullptr;
    QDataStream in;

    QNetworkSession *networkSession = nullptr;
};

#endif // CLIENT_H
