#include "tcpclient.h"

TcpClient::TcpClient(QObject *parent)
    : QObject(parent)
{
    // Create the server socket and listen for connections
    server = new QTcpServer(this);
    connect(server, SIGNAL(newConnection()), this, SLOT(handleNewConnection()));
    server->listen(QHostAddress::Any, 24042);
}

void TcpClient::handleNewConnection()
{
    QTcpSocket *socket = server->nextPendingConnection();
    std::string peers = getPeers();
    socket->write(peers.c_str());
    connect(socket, SIGNAL(readyRead()), this, SLOT(readFromAll()));
    m_sockets.append(socket);

    emit newConnection(socket);
}

void TcpClient::sendToAll(QString message)
{
    for (QTcpSocket *socket : m_sockets)
    {
        socket->write(message.toUtf8());
    }
}

void TcpClient::readFromAll()
{
    for (QTcpSocket *socket : m_sockets)
    {
        while (socket->bytesAvailable() > 0)
        {
            QString message = QString::fromUtf8(socket->readAll());
            emit newMessageReceived(message);
        }
    }
}

void TcpClient::firstConnect(std::string firstIp, int firstPort)
{
    QTcpSocket *firstSocket = new QTcpSocket(this);
    // Get me a damn socket! Grrrr
    firstSocket->connectToHost(firstIp.c_str(), firstPort);
    firstSocket->write("Hello there!");
    firstSocket->waitForBytesWritten(1000);
    firstSocket->waitForReadyRead(1000);

    if (firstSocket->waitForConnected())
    {
        connect(firstSocket, SIGNAL(readyRead()), this, SLOT(readFromAll()));
        m_sockets.append(firstSocket);
        emit newConnection(firstSocket);
        qDebug() << "Connected to: " << firstIp.c_str() << ":" << firstSocket;

        std::string recv;
        recv = firstSocket->readAll();

        // Parse the received buffer filled with new IP and Ports.
        std::istringstream ss(recv);
        std::string token, newIP, newPort;
        getline(ss, token); // discard first line

        while (getline(ss, token))
        {
            std::string temp;
            std::stringstream tokenStream(token);
            getline(tokenStream, newIP, ':');
            getline(tokenStream, newPort);

            QTcpSocket *socket = new QTcpSocket(this);

            socket->connectToHost(newIP.c_str(), atoi(newPort.c_str()));
            socket->write("Hello there!");
            socket->waitForBytesWritten(1000);
            socket->waitForReadyRead(1000);

            temp = socket->readAll();
            // Are we connected?
            if (socket->state() == QAbstractSocket::ConnectedState)
            {
                std::cout << "Connected to " << newIP << ":" << newPort << " successfully!" << std::endl;
                connect(socket, SIGNAL(readyRead()), this, SLOT(readFromAll()));
                m_sockets.append(socket);
                emit newConnection(socket);
            }
            else
            {
                std::cout << "Failed to connect to " << newIP << ":" << newPort << "." << std::endl;
            }
        }
    }
    else
    {
        qDebug() << "Failed to connect to: " << firstIp.c_str() << ":" << firstSocket;
    }
}

std::string TcpClient::getPeers(void)
{
    QString peerList = "TEST NEWCON";
    peerList += '\n';
    for (QTcpSocket *socket : m_sockets)
    {
        QString address = QHostAddress(socket->peerAddress().toIPv4Address()).toString();
        peerList += address + ":" + QString::number(socket->peerPort()) + '\n';
    }

    std::string temp = peerList.toStdString();
    return temp;
}
