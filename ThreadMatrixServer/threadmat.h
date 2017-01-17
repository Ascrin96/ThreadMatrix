#ifndef THREADMAT_H
#define THREADMAT_H

#include "QFile"
#include "QString"
#include "iostream"
#include "QDebug"
#include <pthread.h>
#include <stdlib.h>
#include <ctime>

typedef struct threadDiscriptor {
    int id;
    int n;
    int from;
    int to;
    int *mat1;
    int *mat2;
    int *mat3;
} td;

typedef struct outStruct{
    int time;
    int numThread;
}outStruct;

void printMat (const int* mat, int lenght );
void* mulRow(void *arg);
void initNULL(int* mat, int length);
int* createMatrix(int length);
QByteArray setData(QString fileName);


#endif // THREADMAT_H
