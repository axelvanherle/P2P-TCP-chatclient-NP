#include "client.h"
#include <iostream>

#include <string>
#include <sstream>

using namespace std;

client::client(QObject *parent) :
    QObject(parent)
{
}

void client::firstConnect(std::string IP, int port)
{
    string recv;

    socket = new QTcpSocket(this);

    socket->connectToHost(IP.c_str(), port);

    if(socket->waitForConnected(3000))
    {
        socket->write("NEWCON");
        socket->waitForBytesWritten(1000);
        socket->waitForReadyRead(10000);

        recv = socket->readAll();
        cout << "received : "<< recv << endl;

        istringstream ss(recv);
        string token, ip, port;
        getline(ss, token); // discard first line
        while (getline(ss, token))
        {
            stringstream tokenStream(token);
            getline(tokenStream, ip, ':');
            getline(tokenStream, port);
            cout << "IP: " << ip << ", Port: " << port << std::endl;
        }
        socket->close();
    }
    else
    {
        qDebug() << "No reply.";
    }
}
