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
    int i = 0;
    string recv;

    // Get the first socket.
    socketList.push_back(new QTcpSocket(this));

    // Get me a damn socket! Grrrr
    socketList[i]->connectToHost(IP.c_str(), port);
    socketList[i]->write("BLA");
    socketList[i]->waitForBytesWritten(1000);
    socketList[i]->waitForReadyRead(1000);

    // Were at the last point in the firstconnect phase where we need this socket, so we can move on to the next one.
    recv = socketList[i++]->readAll();
    cout << "received : " << recv << endl;

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
        cout << "IP: " << newIP << ", Port: " << newPort << std::endl;

        socketList.push_back(new QTcpSocket(this));

        socketList[i]->connectToHost(newIP.c_str(), atoi(newPort.c_str()));
        socketList[i]->write("IN LOOP");
        socketList[i]->waitForBytesWritten(1000);
        socketList[i]->waitForReadyRead(1000);
        // Were at the last point in the firstconnect phase where we need this socket, so we can move on to the next one.
        temp = socketList[i++]->readAll();
        cout << temp << endl;
    }
}

void client::sendMessage(void)
{
    string buffer;
    cin >> buffer;
    cout << buffer;
}

void client::receiveMessage(void)
{
}
