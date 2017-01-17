#include <threadmat.h>

QByteArray setData(QString fileName){
    QFile *file = new QFile(fileName);
    if(!file->open(QIODevice:: ReadWrite | QIODevice::Text)){
        return 0;
    }

    QByteArray array = file->readAll();
    file->close();

    return array;
}

int *createMatrix(int length) {
    int* mat = (int*) malloc(sizeof(int) * length);

    for(int i = 0; i < length; i++) {
        mat[i] = 0;
    }

    return mat;
}

void initNULL(int *mat, int length) {
    for(int i = 0; i < length; i++) {
        mat[i] = 0;
    }
}

/*
   Функция потоков для построчного разбияния на потоки
*/
void *mulRow(void *arg) {
    td *mtx = (td*) arg;

    for (int rowId = mtx->from; rowId < mtx->to; rowId++) {
        for (int i = 0; i < mtx->n; i++) {
            for (int j = 0; j < mtx->n; j++) {
                mtx->mat3[mtx->n * rowId + i] += mtx->mat1[mtx->n * rowId + j] * mtx->mat2[mtx->n * j + i];
            }
        }
    }

    pthread_exit(0);
}

/*
   Функция потоков для поэлементного разбиения на потоки
*/
//void* mulRow(void *arg) {
//    td *mtx = (td*) arg;

//    for (int ind = mtx->from; ind < mtx->to; ind++) {
//        for (int k = 0; k < mtx->n; k++) {
//            mtx->mat3[ind] += mtx->mat1[mtx->n * (ind / mtx->n) + k] * mtx->mat2[mtx->n * k + (ind % mtx->n)];
//        }
//    }

//    pthread_exit(0);
//}

void printMat(const int *mat, int lenght) {
    int n = sqrt(lenght);
    for(int i = 0; i < lenght; i++){
        std::cout << mat[i];
        if(i%n == (n-1)){
            std::cout<< std::endl;
            continue;
        }
        std::cout << " " ;
    }

}
