// myserver.cpp

#include "server.h"

server::server(QObject *parent) :
    QObject(parent)
{
    myserver = new QTcpServer(this);

    connect(myserver, SIGNAL(newConnection()), this, SLOT(newConnection()));

    if(!myserver->listen(QHostAddress::Any, 12345))
    {
        qDebug() << "Server could not start!";
    }
    else
    {
        qDebug() << "Server started!";
    }
}

void server::newConnection()
{
    QTcpSocket *socket = myserver->nextPendingConnection();

    socket->write("hello client\r\n");
    socket->flush();

    socket->waitForBytesWritten(3000);

    socket->close();
}
