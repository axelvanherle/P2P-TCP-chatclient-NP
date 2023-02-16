#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QAbstractSocket>
#include <QDebug>

#include <string>

class client : public QObject
{
    Q_OBJECT
public:
    explicit client(QObject *parent = 0);

    void firstConnect(std::string IP, int port);
    void sendMessage(void);
    void receiveMessage(void);

signals:

public slots:

private:
    QTcpSocket *socket;
    std::vector <QTcpSocket *> socketList;
};

#endif // client_H
