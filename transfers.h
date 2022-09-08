#ifndef TRANSFERS_H
#define TRANSFERS_H

#include "tsend.h"
#include "treceive.h"

#include <QList>
#include <QObject>

class Transfers : public QObject
{
    Q_OBJECT

public:
    explicit Transfers(QObject *parent = 0);

public slots:
    void setupSend(QString, int);
    void setupReceive(QString, QString, int);
    void closeSend(int);
    void closeReceive(int);
    void setupIPFinder();
    void closeIPFinder(QString);

signals:
    void sendClosed(int);
    void receiveClosed(int);
    void finderClosed(QString);

private:
    QList<tSend*> tSendList;
    tReceive *receiveObject;
};

#endif // TRANSFERS_H
