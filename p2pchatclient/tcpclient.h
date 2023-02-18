#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QCoreApplication>
#include <QObject>
#include <QTcpSocket>
#include <qtcpserver.h>
#include <QAbstractSocket>
#include <QDebug>
#include <QInputDialog>


#include <iostream>
#include <sstream>
#include <string>


class TcpClient : public QObject
{
    Q_OBJECT

public:
    explicit TcpClient(QObject *parent = nullptr);
    std::string getPeers(void);
    void firstConnect(std::string firstIp, int firstPort);

signals:
    void sendMessage(QString message);
    void newMessageReceived(QString message);
    void newConnection(QTcpSocket *socket);

public slots:
    void handleNewConnection();
    void sendToAll(QString message);
    void readFromAll();
private:
    QList<QTcpSocket *> m_sockets;
    QTcpServer *server;
};


#endif // TCPCLIENT_H
