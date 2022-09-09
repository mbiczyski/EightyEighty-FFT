#include "transfers.h"

Transfers::Transfers(QObject *parent) :
    QObject(parent)
{

}


void Transfers::setupSend(QString path, int port)
{
    tSend *sendObject = new tSend();
    tSendList.push_back(sendObject);
    int pos = tSendList.length()-1;
    tSendList[pos]->setPath(path);
    tSendList[pos]->setPort(port);
    tSendList[pos]->listen();

    connect(tSendList[pos], &tSend::serverClosed, this, &Transfers::closeSend);
}

void Transfers::setupReceive(QString path, QString ip, int port)
{
    receiveObject = new tReceive();
    receiveObject->setIP(ip);
    receiveObject->setIPFinder(false);
    receiveObject->setPath(path);
    receiveObject->setPort(port);
    receiveObject->receiveFile();

    connect(receiveObject, &tReceive::clientClosed, this, &Transfers::closeReceive);
}
void Transfers::setupIPFinder()
{
    receiveObject = new tReceive();
    receiveObject->setIP("whatismyip.org");
    receiveObject->setIPFinder(true);
    receiveObject->setPath("tempip.txt");
    receiveObject->setPort(80);
    receiveObject->receiveFile();

    connect(receiveObject, &tReceive::ipFound, this, &Transfers::closeIPFinder);
}

void Transfers::closeSend(int port)
{
    int pos;
    for(pos = 0; pos < tSendList.length(); pos++) {
        if(tSendList[pos]->getPort() == port) break;
    }
    disconnect(tSendList[pos], &tSend::serverClosed, this, &Transfers::closeSend);
    tSend *tempSendObject = tSendList[pos];
    tSendList.removeAt(pos);

    emit sendClosed(port);
    tempSendObject->deleteLater();
}

void Transfers::closeReceive(int port)
{
    disconnect(receiveObject, &tReceive::clientClosed, this, &Transfers::closeReceive);

    emit receiveClosed(port);
    receiveObject->deleteLater();
}

void Transfers::closeIPFinder(QString found)
{
    disconnect(receiveObject, &tReceive::ipFound, this, &Transfers::closeIPFinder);

    emit finderClosed(found);
    receiveObject->deleteLater();
}
