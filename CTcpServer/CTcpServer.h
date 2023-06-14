#ifndef CMYSERVER_H
#define CMYSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>

class CTcpServer : public QObject
{
    Q_OBJECT
public:
    static CTcpServer* GetInstance();

    void writeMsg(QString strMsg);

signals:
    void signalReceiveClientMsg(QByteArray byteMsg);

private slots:
    void _SlotRecvConnect();
    void _SlotDisplayError(QAbstractSocket::SocketError);
    void _SlotReadMessage();

private:
    explicit CTcpServer();
    ~CTcpServer();

    bool _CreateConnecttion(QString ip,int port);

private:
    static CTcpServer* m_spInstance;
    QString m_ip;
    int m_port;
    QTcpServer* m_tcpServer;
    QTcpSocket* m_tcpSocket;
};

#endif // CMYSERVER_H
