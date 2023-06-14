#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "CTcpServer.h"
#include <QDebug>
#include <QJsonObject>
#include <QJsonDocument>
#include <unistd.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    CTcpServer::GetInstance();
    connect(CTcpServer::GetInstance(), &CTcpServer::signalReceiveClientMsg,
            this, &MainWindow::SlotReceiveClientMsg);

    ui->lineEdit->installEventFilter(this);
    ui->lineEdit_2->installEventFilter(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::eventFilter(QObject *pObject, QEvent *pEvent)
{
    if(QEvent::MouseButtonPress == pEvent->type())
    {
        if(pObject->inherits("QLineEdit"))
        {
            m_pCurrentLineEdit = (QLineEdit*)pObject;
            QString msg = m_pCurrentLineEdit->text();
            qDebug()<<"当前QLineEdit:"<<m_pCurrentLineEdit->objectName()<<msg;

            QJsonObject rootObj;
            rootObj.insert("type",1);
            rootObj.insert("msg",msg);
            QJsonDocument writeDoc(rootObj);
            QByteArray writeJson = writeDoc.toJson(QJsonDocument::Indented);

            CTcpServer::GetInstance()->writeMsg(writeJson.data());

            this->hide(); //不能close
        }
    }

    return QWidget::eventFilter(pObject, pEvent);
}

void MainWindow::SlotReceiveClientMsg(QByteArray byteMsg)
{
    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(byteMsg,&err);
    if(QJsonParseError::NoError != err.error)
        return;

    QJsonObject rootObj = doc.object();
    int type = rootObj.value("type").toInt();
    QString msg = rootObj.value("msg").toString();
    qDebug()<<type<<msg;

    m_pCurrentLineEdit->setText(msg);

    qDebug()<<"mainwindow isVisible:"<<this->isVisible();
    if(!this->isVisible())
       this->show();
}


void MainWindow::on_pushButton_clicked()
{
    this->close();

    sleep(3);

    this->show();
}
