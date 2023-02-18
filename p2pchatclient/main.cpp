#include <QCoreApplication>
#include <QObject>
#include <QTcpSocket>
#include <qtcpserver.h>
#include <QAbstractSocket>
#include <QDebug>
#include <QInputDialog>
#include <QApplication>
#include <iostream>
#include <sstream>
#include <string>

#include "tcpclient.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    TcpClient client;

    client.firstConnect(argv[1], atoi(argv[2]));

    QObject::connect(&client, &TcpClient::newConnection, [&](QTcpSocket *socket)
    {
        qDebug() << "New connection from: " << socket->peerAddress().toString();
        QObject::connect(socket, &QTcpSocket::readyRead, &client, &TcpClient::readFromAll);
        //QObject::connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);
    });

    QObject::connect(&client, &TcpClient::newMessageReceived, [&](QString message)
    {
        qDebug() << "New message received: " << message;
    });

    while(1)
    {
        QString message = QInputDialog::getText(nullptr, "Send message", "Enter message:");
        if(message.isEmpty())
        {
            continue;
        }
        client.sendToAll(message);
    }

    return a.exec();
}

