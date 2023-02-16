#include <iostream>
#include "client.h"
#include <QCoreApplication>

using namespace std;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    client client;

    // Check the arguments that we have received.
    if (argc == 1)
    {
        cout << "You passed 0 arguments. Waiting for a connection." << endl;
        client.waitForNewConnection();
    }
    else if (argc == 3)
    {
        cout << "You are sending to IP " << argv[1] << " and port " << argv[2] << endl;
        client.firstConnect(argv[1], atoi(argv[2]));
        client.sendMessage();
        //client.receiveMessage();
        //client.waitForNewConnection();

        cout << "test";
    }
    else
    {
        cout << "Wrong usage of arguments; pass IP <space> PORT." << endl;
    }

    return a.exec();
}
