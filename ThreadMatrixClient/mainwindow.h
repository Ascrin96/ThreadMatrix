#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_btnSend_clicked();
    void slotReadyRead();
    void slotConnected();

private:
    Ui::MainWindow *ui;
    QTcpSocket * mTcpSocket;
    QByteArray data;
};

#endif // MAINWINDOW_H
