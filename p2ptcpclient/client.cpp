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
    if (firstSocket->state() != QAbstractSocket::ConnectedState)
    {
        cout << "Failed to connect to " << IP << " on port " << port << "." << endl;
        exit(1);
    }

    socketList.push_back(firstSocket);

    recv = firstSocket->readAll();
    cout << "Successfully connected to " << IP << " on port " << port << "." << endl
         << "Received:" << endl
         << "==========" << endl
         << recv << "==========" << endl;

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
        if (socket->state() == QAbstractSocket::ConnectedState)
        {
            cout << "Connected to " << newIP << ":" << newPort << " successfully!" << endl;
            socketList.push_back(socket);
        }
        else
        {
            cout << "Failed to connect to " << newIP << ":" << newPort << "." << endl;
        }
    }
}

void client::sendMessage()
{
    while (1)
    {
        // Read a line of input from the user
        string buffer;
        cout << "Message: ";
        getline(std::cin, buffer);

        // Loop through all connected sockets and send the input to each one
        for (QTcpSocket *socket : socketList)
        {
            socket->write(buffer.c_str());
            socket->waitForBytesWritten(1000);
        }
    }
}

void client::receiveMessage()
{
    while (1)
    {
        // Loop through all connected sockets
        for (QTcpSocket *socket : socketList)
        {
            // Check if there is any data available to read
            if (socket->waitForReadyRead(0))
            {
                // Read the data and print a message
                QString buffer = socket->readAll();
                qDebug() << "New message from" << socket->peerAddress().toString() << ": " << buffer;
            }
        }
    }
}

void client::waitForNewConnection(void)
{
    // Create a new TCP server object
    QTcpServer server(this);

    int port = 24042;

    // Listen for incoming connections on the specified port
    if (!server.listen(QHostAddress::Any, port))
    {
        qDebug() << "Server failed to start. Error:" << server.errorString();
        exit(1);
    }

    cout << "Server started. Listening on port " << port;

    // Wait for new connections
    while (server.isListening())
    {
        if (server.waitForNewConnection(-1))
        {
            // Accept the new connection
            QTcpSocket *socket = server.nextPendingConnection();

            // Add the new socket to the socket list
            socketList.push_back(socket);

            qDebug() << "New connection from" << socket->peerAddress().toString() << ":" << socket->peerPort();
        }
    }
}
