#include "tsend.h"

tSend::tSend(QObject *parent) :
    QObject(parent)
{
    serverPort = 8080;
}

void tSend::setPath(QString dir)
{
    fileDir.setPath(dir);
}

void tSend::setPort(int port_num)
{
    serverPort = port_num;
}

int tSend::getPort()
{
    return serverPort;
}

void tSend::listen()
{
    server.listen(QHostAddress::Any, serverPort);
    connect(&server, &QTcpServer::newConnection, this, &tSend::acceptConnection);
    qDebug() << "Port" << serverPort << ": Listening for connections with file" << fileDir.dirName();
}

void tSend::acceptConnection()
{
    client = server.nextPendingConnection();
    connect(client, &QTcpSocket::readyRead, this, &tSend::sendFile);
}

void tSend::sendFile()
{
    QTcpSocket* socket = (QTcpSocket*)sender();

    QFile file(fileDir.absolutePath());
    file.open(QIODevice::ReadOnly);

    QTextStream fs(&file);
    QString fileData = fs.readAll();

    file.close();

    QTextStream ts(socket);
    ts.setAutoDetectUnicode(true);
    ts << "HTTP/1.0 200 Ok\r\n"
          "Server: EightyEighty\r\n"
          "Connection: close\r\n"
          "Content-Type: application/octet-stream\r\n"
          "Content-Disposition: attachment; filename=" << fileDir.dirName() << "\r\n"
          "Content-Length: " << file.size() << "\r\n\r\n"
       << fileData;

    socket->close();

    if (socket->state() == QTcpSocket::UnconnectedState)
    {
        delete socket;
    }

    server.close();
    qDebug() << "Port" << serverPort << ": Sent file" << fileDir.dirName() << "of size" << file.size();

    emit serverClosed(serverPort);
}
