#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QDebug>

#include <string>

class client : public QObject
{
    Q_OBJECT
public:
    explicit client(QObject *parent = 0);

    void firstConnect(std::string IP, int port);

signals:

public slots:

private:
    QTcpSocket *socket;

};

#endif // client_H
