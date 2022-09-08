#include "treceive.h"

tReceive::tReceive(QObject *parent) :
    QObject(parent)
{
    client = new QTcpSocket(this);
    connect(client, &QTcpSocket::readyRead, this, &tReceive::readFile);
    connect(client, &QTcpSocket::connected, this, &tReceive::connectedToServer);

    firstRead = true;
    dataReceived = 0;
}

void tReceive::setPath(QString dir)
{
    fileDir.setPath(dir);
}

void tReceive::setPort(int port_num)
{
    serverPort = port_num;
}

void tReceive::setIP(QString ip)
{
    serverIP.clear();
    serverIP.append(ip);
}


void tReceive::setIPFinder(bool find)
{
    findIP = find;
}

int tReceive::getPort()
{
    return serverPort;
}

void tReceive::receiveFile()
{
    client->abort();
    client->connectToHost(serverIP, serverPort);
    qDebug() << "Port" << serverPort << ": Connecting to Host" << serverIP;
    if(!client->waitForConnected(5000))
    {
        qDebug() << "Port" << serverPort << ": Error" << client->errorString();
        emit clientClosed(serverPort);
    }
}

void tReceive::connectedToServer()
{
    client->write("GET / HTTP/1.0\r\n\r\n\r\n\r\n");

    file = new QFile(fileDir.absolutePath());
    file->open(QIODevice::WriteOnly | QIODevice::Truncate);
}

void tReceive::readFile()
{
    QTcpSocket* socket = (QTcpSocket*)sender();
    QTextStream inStream(socket);

    if(findIP) {
        dataReceived += socket->bytesAvailable();

        if(firstRead) {
            for(int i = 0; i < 6; i++) inStream.readLine();
            QString length = inStream.readLine();
            inStream.readLine();
            length.remove(0, 16);
            dataLength = length.toInt();
            firstRead = false;
        }

        for(int i = 0; i < 15; i++) inStream.readLine();
        QString tempBuffer = inStream.readLine();
        inStream.readAll();

        int backTrim = tempBuffer.lastIndexOf("<");
        int frontTrim = tempBuffer.indexOf(">");

        tempBuffer.truncate(backTrim);
        tempBuffer.remove(0, frontTrim+1);

        if(dataReceived >= dataLength && socket->bytesAvailable() == 0)
        {
            foundIP = tempBuffer;
            qDebug() << "Found external IP:" << foundIP;

            file->close();
            file->remove();
            delete file;
            socket->close();
            delete socket;
            if(!foundIP.isEmpty()) emit ipFound(foundIP);
        }

    }
    else {
        dataReceived += socket->bytesAvailable();

        QTextStream fileStream(file);

        if(firstRead) {
            for(int i = 0; i < 5; i++) inStream.readLine();
            QString length = inStream.readLine();
            inStream.readLine();
            length.remove(0, 16);
            dataLength = length.toInt();
            firstRead = false;
        }

        QString tempBuffer;
        do {
            tempBuffer = inStream.read(1024);
            fileStream << tempBuffer;
        } while(!tempBuffer.isNull());

        if(dataReceived >= dataLength && socket->bytesAvailable() == 0)
        {
            qDebug() << "Port" << serverPort << ": Downloaded:" << dataReceived << "from" << dataLength << "\nPending data:" << socket->bytesAvailable();

            file->close();
            delete file;
            socket->close();
            delete socket;
            emit clientClosed(serverPort);
        }
    }
}
