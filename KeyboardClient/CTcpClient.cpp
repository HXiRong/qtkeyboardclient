#include "CTcpClient.h"
#include <QHostAddress>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <QCoreApplication>
#include <QEvent>
#include <QMouseEvent>
#include <QGuiApplication>

CTcpClient* CTcpClient::m_spInstance = nullptr;

CTcpClient::CTcpClient(QObject *parent) : QObject(parent)
{
    m_bConnectOK = false;

    m_pTcpSocket = new QTcpSocket(this);
    connect(m_pTcpSocket,SIGNAL(connected()),this,SLOT(slotConnectOK()));
    connect(m_pTcpSocket,SIGNAL(disconnected()),this,SLOT(slotConnectFail()));
    connect(m_pTcpSocket,SIGNAL(readyRead()),SLOT(slotReadMessage()));
    connect(m_pTcpSocket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(slotDisplayError(QAbstractSocket::SocketError)));
    connect(this,SIGNAL(signalWriteMsg(QString)),this,SLOT(slotWriteMessage(QString)));

    QString ip = "127.0.0.1";
    int port = 10001;
    m_pTcpSocket->connectToHost(QHostAddress(ip),port);

    m_pTimer = new QTimer(this);
    connect(m_pTimer,SIGNAL(timeout()),this,SLOT(slotCheckConnectState()));
    m_pTimer->start(2000);
}

CTcpClient* CTcpClient::GetInstance()
{
    if(nullptr == m_spInstance)
        m_spInstance = new CTcpClient;
    return m_spInstance;
}

void CTcpClient::slotCheckConnectState()
{
    if(m_bConnectOK)
        return;

    QString ip = "127.0.0.1";
    int port = 10001;
    m_pTcpSocket->connectToHost(QHostAddress(ip),port);
}

void CTcpClient::slotConnectOK()
{
    m_bConnectOK = true;
    qDebug()<<Q_FUNC_INFO<<"连接服务器成功";
}

void CTcpClient::slotConnectFail()
{
    m_bConnectOK = false;
    qDebug()<<Q_FUNC_INFO<<"连接服务器失败";
}

void CTcpClient::slotReadMessage()
{
    QByteArray array = m_pTcpSocket->readAll();
    qDebug()<<Q_FUNC_INFO<<array.data();

    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(array,&err);
    if(QJsonParseError::NoError != err.error)
        return;

    QJsonObject rootObj = doc.object();
    QString msg = rootObj.value("msg").toString();
    int type = rootObj.value("type").toInt();

     emit cppSignaReceiveMsg(msg,type);

   // if(1 == type)
   //     showKeyboard();
}

void CTcpClient::slotDisplayError(QAbstractSocket::SocketError)
{
    QString data = m_pTcpSocket->errorString();
    //  qDebug()<<Q_FUNC_INFO<<data;
}

void CTcpClient::writeMessage(QString msg,int type)
{
    if(!m_bConnectOK)
        return;

    QJsonObject rootObj;
    rootObj.insert("type",type);
    rootObj.insert("msg",msg);
    QJsonDocument writeDoc(rootObj);
    QByteArray writeJson = writeDoc.toJson(QJsonDocument::Indented);

    int iRet = m_pTcpSocket->write(writeJson);
    qDebug()<<Q_FUNC_INFO<<writeJson.data()<<iRet;
}

int CTcpClient::getLanguage()
{
    QString path = QCoreApplication::applicationDirPath() + "/Resources/system.json";
    qDebug()<<Q_FUNC_INFO<<path;
    QFile qFile(path);
    if(!qFile.open(QIODevice::ReadOnly))
        return 1;

    QByteArray byteJson = qFile.readAll();
    qFile.close();

    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(byteJson,&err);
    if(QJsonParseError::NoError != err.error)
        return 1;

    QJsonObject rootObj = doc.object();
    QJsonObject norObj = rootObj.value("Normal_Setting").toObject();
    int iLan = norObj.value("language-flag").toString().toInt();
    qDebug()<<Q_FUNC_INFO<<byteJson<<iLan;
    return iLan;
}

void CTcpClient::setObject(QObject *obj)
{
    m_pObject = obj;
    qDebug()<<Q_FUNC_INFO<<m_pObject;
}

void CTcpClient::showKeyboard()
{
    QGuiApplication::inputMethod()->show();
}

void CTcpClient::slotWriteMessage(QString msg)
{
    int iRet = m_pTcpSocket->write(msg.toLocal8Bit());
    qDebug()<<Q_FUNC_INFO<<msg<<iRet;
}
