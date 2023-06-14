#include "CTcpServer.h"
#include <QDateTime>

static QString CurrentTime()
{
    return QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");
}

CTcpServer* CTcpServer::m_spInstance = nullptr;

CTcpServer::CTcpServer()
{
    m_ip = "127.0.0.1";
    m_port = 10001;
    _CreateConnecttion(m_ip,m_port);
}

CTcpServer* CTcpServer::GetInstance()
{
    if(nullptr == m_spInstance)
        m_spInstance = new CTcpServer;
    return m_spInstance;
}

CTcpServer::~CTcpServer()
{
    m_tcpServer->close();
    m_tcpServer->deleteLater();
    m_tcpSocket->deleteLater();

    qDebug()<<CurrentTime()<<__FUNCTION__<<"销毁服务端:"<<m_ip<<m_port;
}

void CTcpServer::writeMsg(QString strMsg)
{
    m_tcpSocket->write(strMsg.toLocal8Bit());
}

void CTcpServer::_SlotRecvConnect()
{
    qDebug()<<CurrentTime()<<__FUNCTION__<<"TcpServer收到连接,连接成功"<<m_ip<<m_port;
    m_tcpSocket = m_tcpServer->nextPendingConnection();
    connect(m_tcpSocket,SIGNAL(readyRead()),this,SLOT(_SlotReadMessage()));
    connect(m_tcpSocket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(_SlotDisplayError(QAbstractSocket::SocketError)));
}

void CTcpServer::_SlotDisplayError(QAbstractSocket::SocketError)
{
    qDebug()<<CurrentTime()<<__FUNCTION__<<m_tcpSocket->errorString();
}

void CTcpServer::_SlotReadMessage()
{
    QByteArray byteRead = m_tcpSocket->readAll();
    qDebug()<<CurrentTime()<<__FUNCTION__<<"read message:"<<m_ip<<m_port<<byteRead.data();

    emit signalReceiveClientMsg(byteRead);
}

bool CTcpServer::_CreateConnecttion(QString ip, int port)
{
    m_tcpServer = new QTcpServer(this);
    m_tcpSocket = new QTcpSocket(this);

    qDebug()<<CurrentTime()<<__FUNCTION__<<"开始监听:"<<ip<<port;

    if(!m_tcpServer->listen(QHostAddress(ip),port))
    {
        qDebug()<<CurrentTime()<<__FUNCTION__<<"tcp listen error:" + m_tcpServer->errorString();

        m_tcpServer->close();
        return false;
    }
    qDebug()<<CurrentTime()<<__FUNCTION__<<"监听OK:"<<ip<<port;
    connect(m_tcpServer,SIGNAL(newConnection()),this,SLOT(_SlotRecvConnect()));
    return true;
}
