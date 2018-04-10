#include "matrix.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define MAXN 2048
#define UINT unsigned long
#define MAX_THREAD 4

typedef struct Para{
    int up, down, N;
    UINT *A, *B, *C;
}Para;


void transpose(int N, UINT M[][MAXN]){
    UINT tmp;
    for(int i = 0; i < N; i++) {
        for(int j = i+1; j < N; j++){
            tmp = M[i][j];
            M[i][j] = M[j][i];
            M[j][i] = tmp;
        }
    }
}

void* multiply_subtask(void* para){
    Para data = *(Para*)para;
    int u = data.up;
    int d = data.down;
    int N = data.N;
    UINT *A = data.A;
    UINT *B = data.B;
    UINT *C = data.C;
    UINT *a, *b;

    for(int i = u; i <= d; i++){
        for(int j = 0; j < N; j++){
            a = A + i * MAXN;
            b = B + j * MAXN;
            UINT sum = 0;
            for(int k = 0; k < N; k++){ // vector inner product
                sum += (*a) * (*b);
                a++;
                b++;
            }

            *(C + i * MAXN + j) = sum;
        }
    }
    return 0;
}

void multiply(int N, unsigned long A[][2048], unsigned long B[][2048], unsigned long C[][2048]) {

    transpose(N, B); // transpose B to have better performance for cache
    pthread_t threads[MAX_THREAD];

    for(int i = 0; i < MAX_THREAD; i++){ // every thread will have their own target rows of C
        Para *para = (Para*) malloc(sizeof(Para));
        para->up = i * N / MAX_THREAD;
        para->down = (i+1) * N / MAX_THREAD- 1;
        para->N = N;
        para->A = &A[0][0];
        para->B = &B[0][0];
        para->C = &C[0][0];

        pthread_create(&threads[i], NULL, multiply_subtask, (void *) para);

    }

    for (int i = 0; i < MAX_THREAD; i++)
       pthread_join(threads[i], NULL);

}
