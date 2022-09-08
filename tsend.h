#ifndef TSEND_H
#define TSEND_H

#include <QObject>
#include <QString>
#include <QDir>
#include <QtCore/QFile>

#include <QTcpServer>
#include <QTcpSocket>

#include <QDebug>

class tSend : public QObject
{
    Q_OBJECT
public:
    explicit tSend(QObject *parent = 0);

signals:
    void serverClosed(int);

public slots:
    void acceptConnection();
    void sendFile();
    void setPath(QString);
    void setPort(int);
    int getPort();
    void listen();

private:
    QTcpServer server;
    QTcpSocket* client;
    QDir fileDir;
    int serverPort;
};

#endif // TSEND_H
