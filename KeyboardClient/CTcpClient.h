#ifndef CTCPCLIENT_H
#define CTCPCLIENT_H

#include <QObject>
#include <QAbstractSocket>
#include <QTcpSocket>
#include <QTimer>
#include <QQmlEngine>

class CTcpClient : public QObject
{
    Q_OBJECT
public:    
    static CTcpClient* GetInstance();

    Q_INVOKABLE void writeMessage(QString msg,int type=1);
    Q_INVOKABLE int getLanguage();
    Q_INVOKABLE void setObject(QObject* obj);
    Q_INVOKABLE void showKeyboard();

    static QObject* qmlSingletonInstance(QQmlEngine* engine,QJSEngine* scriptEngine)
    {
        Q_UNUSED(engine)
        Q_UNUSED(scriptEngine)
        return GetInstance();
    }

signals:
    void signalWriteMsg(QString msg);
    void cppSignaReceiveMsg(QString msg,int type);

private slots:
    void slotConnectOK();
    void slotConnectFail();
    void slotReadMessage();
    void slotDisplayError(QAbstractSocket::SocketError);
    void slotWriteMessage(QString msg);
    void slotCheckConnectState();

private:
    explicit CTcpClient(QObject *parent = nullptr);

private:
    static CTcpClient* m_spInstance;
    QTcpSocket *m_pTcpSocket;
    bool m_bConnectOK;
    QTimer* m_pTimer;

    QObject* m_pObject;
};

#endif // CTCPCLIENT_H
