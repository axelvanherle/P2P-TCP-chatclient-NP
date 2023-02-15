#include "client.h"
#include <iostream>

client::client(QObject *parent) :
    QObject(parent)
{
}

void client::firstConnect(std::string IP, int port)
{
    socket = new QTcpSocket(this);

    socket->connectToHost(IP.c_str(), port);

    if(socket->waitForConnected(3000))
    {
        std::cout << "Connected!" << std::endl;

        socket->write("hello server\r\n\r\n\r\n\r\n");
        socket->write("hello server\r\n\r\n\r\n\r\n");
        socket->write("hello server\r\n\r\n\r\n\r\n");
        socket->waitForBytesWritten(1000);
        socket->waitForReadyRead(3000);
        qDebug() << "Reading: " << socket->bytesAvailable();

        qDebug() << socket->readAll();

        socket->close();
    }
    else
    {
        qDebug() << "No reply.";
    }
}
