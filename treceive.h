#ifndef TRECEIVE_H
#define TRECEIVE_H

#include <QObject>
#include <QString>
#include <QDir>
#include <QFile>

#include <QTcpSocket>

#include <QDebug>

class tReceive : public QObject
{
    Q_OBJECT
public:
    explicit tReceive(QObject *parent = 0);

signals:
    void clientClosed(int);
    void ipFound(QString);

public slots:
    void receiveFile();
    void setPath(QString);
    void setPort(int);
    void setIP(QString);
    void setIPFinder(bool);
    int getPort();

private slots:
    void readFile();
    void connectedToServer();

private:
    QTcpSocket *client;
    QString serverIP;
    int serverPort;
    QDir fileDir;
    bool findIP;
    QString foundIP;
    quint64 dataLength;
    quint64 dataReceived;
    QFile *file;
    bool firstRead;
};

#endif // TRECEIVE_H
