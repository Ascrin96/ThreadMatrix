#include "mytcpserver.h"
#include <QDebug>
#include <QCoreApplication>
#include <QDataStream>
#include <threadmat.h>
#include <QRegularExpression>
#include <unistd.h>

MyTcpServer::MyTcpServer(QObject *parent) : QObject(parent)
{
    mTcpServer = new QTcpServer(this);

    connect(mTcpServer, &QTcpServer::newConnection, this, &MyTcpServer::slotNewConnection);
    if(!mTcpServer->listen(QHostAddress::Any, 6000)){
        qDebug() << "server is not started";
    } else {
        qDebug() << "server is started";
    }
}

void MyTcpServer::slotNewConnection(){
    mTcpSocket = mTcpServer->nextPendingConnection();

    connect(mTcpSocket, &QTcpSocket::readyRead, this, &MyTcpServer::slotServerRead);
    connect(mTcpSocket, &QTcpSocket::disconnected, this, &MyTcpServer::slotClientDisconnected);
    fork();
}

void MyTcpServer::slotServerRead()
{
    while(mTcpSocket->bytesAvailable()>0){

        dataArray += mTcpSocket->readAll();
    }

    if(dataArray.length() < sizeof(quint64) / sizeof(quint8)){
        return;
    }

    QDataStream stream(dataArray) ;
    quint64 dataLength;
    stream >> dataLength; //считал первые 8 байт , которые заголовок
    qDebug() << dataArray.length();

    if(dataArray.length() <  (sizeof(quint64) / sizeof(quint8) + dataLength)){
        return;
    }

    QByteArray array = dataArray.mid(8, dataLength);

    dataArray.remove(0, (sizeof(quint64) / sizeof(quint8) + dataLength)); //удалили данные размером с файл из массива

    mulMatrixs(array);


}

void MyTcpServer::slotClientDisconnected()
{
    mTcpSocket->close();
}

void MyTcpServer::mulMatrixs(QByteArray array) {

    QString strData = QString::fromLatin1(array);
    strData = strData.replace("\n","");
    QStringList strList = strData.split(';',  QString::SkipEmptyParts);
    std::cout <<strList.size() << std::endl;
    QList<int> numbers;
    for(int i = 0; i < strList.size(); i++){
        QStringList numList =  strList[i].split(',',  QString::SkipEmptyParts);

        for(int j = 0; j < numList.size(); j++){
            numbers.append(numList[j].toInt());
        }
    }

    int n = numbers[0];
    int matrixLen = n*n;
    numbers.removeFirst();

    int* mat1 = createMatrix(matrixLen);
    int* mat2 = createMatrix(matrixLen);

    for(int i = 0; i < matrixLen; i++){
        mat1[i] = numbers[0];
        numbers.removeFirst();
    }

    for(int i = 0; i < matrixLen; i++){
        mat2[i] = numbers[0];
        numbers.removeFirst();
    }

    outStruct out;
    out.time = 0;
    out.numThread = 0;

    int numThread = 1;

    /*
        Цикл для построчного разбиения на потоки
    */
    while(numThread < n){

        pthread_t* threads = (pthread_t*) malloc(sizeof(pthread_t) * numThread);

        td* tStruct = (td*) malloc(sizeof(td) * numThread);

        int* resMat = createMatrix(matrixLen);
        int rowsPerThread = n / numThread;
        int residue = n % numThread;

        int time;
        time = clock();

        for(int i = 0; i < numThread; i++){
            tStruct[i].mat1 = mat1;
            tStruct[i].mat2 = mat2;
            tStruct[i].mat3 = resMat;
            tStruct[i].n = n;
            tStruct[i].id = i;
            tStruct[i].from = rowsPerThread * i ;
            tStruct[i].to = (i + 1)*rowsPerThread;

            if(i == (numThread - 1)){
                tStruct[i].to += residue;
            }
            pthread_create(&threads[i], NULL, mulRow, (void*) &tStruct[i]);
        }

        for(int i = 0; i < numThread; i++){
            pthread_join(threads[i], NULL);
        }

        time = clock() - time;
        if((time < out.time) || (out.time == 0)){
            out.time = time;
            out.numThread = numThread;
        }


        std::cout << std::endl << "Number of thread: " <<numThread<< std::endl;
        std::cout << "Run time of multiplication: " << time << std::endl;
        numThread = numThread + 1;

        free(resMat);
        free(threads);
        free(tStruct);
    }

    /*
        Цикл для поэлементного разбиения
    */

//    while(numThread < n){

//        pthread_t* threads = (pthread_t*) malloc(sizeof(pthread_t) * numThread);

//        td* tStruct = (td*) malloc(sizeof(td) * numThread);

//        int* resMat = createMatrix(matrixLen);
//        int rowsPerThread = matrixLen / numThread;
//        int residue = matrixLen % numThread;

//        int time;
//        time = clock();

//        for(int i = 0; i < numThread; i++){
//            tStruct[i].mat1 = mat1;
//            tStruct[i].mat2 = mat2;
//            tStruct[i].mat3 = resMat;
//            tStruct[i].n = n;
//            tStruct[i].id = i;
//            tStruct[i].from = rowsPerThread * i ;
//            tStruct[i].to = (i + 1)*rowsPerThread;

//            if(i == (numThread - 1)){
//                tStruct[i].to += residue;
//            }
//            pthread_create(&threads[i], NULL, mulRow, (void*) &tStruct[i]);
//        }

//        for(int i = 0; i < numThread; i++){
//            pthread_join(threads[i], NULL);
//        }

//        time = clock() - time;
//        if((time < out.time) || (out.time == 0)){
//            out.time = time;
//            out.numThread = numThread;
//        }


//        std::cout << std::endl << "Number of thread: " <<numThread<< std::endl;
//        std::cout << "Run time of multiplication: " << time << std::endl;
//        numThread = numThread + 1;

//        free(resMat);
//        free(threads);
//        free(tStruct);
//    }


    free(mat1);
    free(mat2);

    std::cout << std::endl<< "Summary:" << std::endl;
    std::cout << "\tThe best number of threads:\t" << out.numThread << std::endl;
    std::cout << "\tRun time of multiplication:\t" << out.time << std::endl;




    slotSendClient(out.numThread, out.time);
}

void MyTcpServer::slotSendClient(int numThread, int time){

    QDataStream os( mTcpSocket );
    os << (quint64) numThread;
    os << (quint64) time;

    mTcpSocket->waitForBytesWritten();
}
