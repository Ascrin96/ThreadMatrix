#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QHostAddress>
#include <QFileDialog>
#include <time.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    mTcpSocket = new QTcpSocket(this);
    connect(mTcpSocket, &QTcpSocket::connected, this, &MainWindow::slotConnected);
    connect(mTcpSocket, &QTcpSocket::readyRead, this, &MainWindow::slotReadyRead);
    connect(ui->btnClose, &QPushButton::clicked, this, &MainWindow::close);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnSend_clicked(){

    mTcpSocket->connectToHost(ui->lineEditHost->text(), ui->lineEditPort->text().toUInt());

    QString fileName = QFileDialog::getOpenFileName(0, "Open Dialog", "", "");
    QFile file(fileName);

    if(!file.open(QIODevice:: ReadOnly)){
        return;
    }

    QByteArray array = file.readAll();

    QDataStream os( mTcpSocket );
    os << (quint64)array.length();
    os.writeRawData(array.data(), array.length());

    mTcpSocket->waitForBytesWritten();

    file.close();

}

void MainWindow::slotReadyRead(){
    while(mTcpSocket->bytesAvailable()>0)
    {
        data += mTcpSocket->readAll();
    }

    if(data.length() < 2  * (sizeof(quint64) / sizeof(quint8))){
        return;
    }

    QDataStream stream(data);
    quint64 numThread;
    quint64 time;

    stream >> numThread;
    stream >> time;

    data.remove(0, 2  * (sizeof(quint64) / sizeof(quint8)));

    ui->textEdit->append( "The best number of threads: ");
    ui->textEdit->append(QString::number(numThread));
    ui->textEdit->append( "Run time of multiplication: ");
    ui->textEdit->append(QString::number(time));
    ui->textEdit->append("\n");
}

void MainWindow::slotConnected()
{

    ui->textEdit->append( "Succes, server connected.");
    ui->textEdit->append("\n");
}

