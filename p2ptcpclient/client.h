#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <qtcpserver.h>
#include <QAbstractSocket>
#include <QDebug>

#include <iostream>
#include <sstream>
#include <string>

using namespace std;

class client : public QObject
{
    Q_OBJECT
public:
    explicit client(QObject *parent = 0);

    void firstConnect(std::string IP, int port);
    void sendMessage(void);
    void receiveMessage(void);
    void waitForNewConnection(void);
signals:

public slots:

private:
    std::vector<QTcpSocket *> socketList;
};

#endif // client_H
