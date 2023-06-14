#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QLineEdit>
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:


protected:
    virtual bool eventFilter(QObject *pObject, QEvent *pEvent);

public slots:
    void SlotReceiveClientMsg(QByteArray byteMsg);

private slots:
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;

    QLineEdit* m_pCurrentLineEdit;
};
#endif // MAINWINDOW_H
