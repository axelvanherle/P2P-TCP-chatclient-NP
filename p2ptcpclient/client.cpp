#include "client.h"
#include <iostream>

#include <string>
#include <sstream>

using namespace std;

client::client(QObject *parent) : QObject(parent)
{
}

void client::firstConnect(std::string IP, int port)
{
    // Used to keep track of the current socket.
    string recv;
    QTcpSocket *firstSocket = new QTcpSocket(this);

    // Get me a damn socket! Grrrr
    firstSocket->connectToHost(IP.c_str(), port);
    firstSocket->write("Hello there!");
    firstSocket->waitForBytesWritten(1000);
    firstSocket->waitForReadyRead(1000);

    // Are we connected?
    if(firstSocket->state()!= QAbstractSocket::ConnectedState)
    {
        cout << "Failed to connect to " << IP << " on port " << port << "." << endl;
        exit(1);
    }

    socketList.push_back(firstSocket);

    recv = firstSocket->readAll();
    cout << "Successfully connected to " << IP << " on port " << port << "." << endl
         << "Received:" << endl << "==========" << endl << recv << "==========" << endl;

    // Parse the received buffer filled with new IP and Ports.
    istringstream ss(recv);
    string token, newIP, newPort;
    getline(ss, token); // discard first line

    while (getline(ss, token))
    {
        string temp;
        stringstream tokenStream(token);
        getline(tokenStream, newIP, ':');
        getline(tokenStream, newPort);

        QTcpSocket *socket = new QTcpSocket(this);

        socket->connectToHost(newIP.c_str(), atoi(newPort.c_str()));
        socket->write("IN LOOP");
        socket->waitForBytesWritten(1000);
        socket->waitForReadyRead(1000);

        temp = socket->readAll();
        // Are we connected?
        if(socket->state() == QAbstractSocket::ConnectedState)
        {
            cout << "Connected to " << newIP << ":" << newPort << " successfully!" <<endl;
            socketList.push_back(socket);
        }
        else
        {
           cout << "Did not connected to " << newIP << ":" << newPort << " successfully." <<endl;
        }
    }
}

void client::sendMessage(void)
{
    while(1)
    {
        char buffer[256] = {0};
        cin.getline(buffer,256);
        for (unsigned long long var = 0; var < socketList.size(); ++var)
        {
            socketList[var]->write(buffer);
            socketList[var]->waitForBytesWritten(1000);
        }
    }
}

void client::receiveMessage(void)
{
    while(1)
    {
        for (unsigned long long var = 0; var < socketList.size(); ++var)
        {
            string buffer;
            if (socketList[var]->waitForReadyRead(0))
            {
                buffer = socketList[var]->readAll();
                cout << "New message from [" << socketList[var] << "]: " << buffer << endl;
            }
        }
    }
}
