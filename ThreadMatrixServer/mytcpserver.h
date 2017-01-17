#ifndef MYTCPSERVER_H
#define MYTCPSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>

class MyTcpServer : public QObject
{
    Q_OBJECT
public:
    explicit MyTcpServer(QObject *parent = 0);

signals:

    void sendMesseng(int numThread, int time);

public slots:
    void slotNewConnection();
    void slotServerRead();
    void slotClientDisconnected();
    void mulMatrixs(QByteArray array);
    void slotSendClient(int numThread, int time);

private:
    QTcpServer * mTcpServer;
    QTcpSocket * mTcpSocket;
    QByteArray dataArray;
};

#endif // MYTCPSERVER_H
