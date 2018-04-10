#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <pthread.h>
#include "utils.h"

#define MAXN 10000005
#define MAX_THREAD 4
uint32_t prefix_sum[MAXN];

typedef struct Interval{
    int left, right;
    uint32_t *arr;
    uint32_t key;
}Interval;

int min(int x, int y){
    return x < y ? x : y;
}

void* owndomain(void *irv){ 
    Interval data = *((Interval *) irv);
    int l = data.left;
    int r = data.right;
    uint32_t *arr = data.arr;
    uint32_t key = data.key;
    uint32_t sum = 0;

    int k = 0;
    for(int i = l; i <= r; i++){
        sum += encrypt(i, key);
        arr[k] = sum;
        k++;
    }
    free(irv);
}

void* collectall(void *irv){
    Interval data = *((Interval *) irv);
    int l = data.left;
    int r = data.right;
    uint32_t *arr = data.arr;
    uint32_t addthis = data.key;

    int k = 0;
    for(int i = l; i <= r; i++){
        arr[k] += addthis;
        k++;
    }
    free(irv);
}

int main() {
    int n;
    uint32_t key;
    while (scanf("%d %" PRIu32, &n, &key) == 2) {
        int BLOCK = (n+MAX_THREAD-1) / MAX_THREAD;
        int used_thread = 0;
        pthread_t threads[MAX_THREAD];

        for (int i = 1; i <= n;) { //  assign works to threads
            Interval *irv = (Interval *) malloc(sizeof(Interval)); // intervals of each thread
            irv->left = i;
            irv->right = min(n, i + BLOCK - 1);
            irv->arr = prefix_sum + i;
            irv->key = key;
            i += BLOCK;

            pthread_create(&threads[used_thread], NULL, owndomain, irv);
            used_thread++;
        }
        for (int i = 0; i < used_thread; i++)
           pthread_join(threads[i], NULL);
        used_thread = 0;


        uint32_t block_sum = 0;
        for(int i = 1; i <= n;){
            Interval *irv = (Interval *) malloc(sizeof(Interval));
            irv->left = i;
            irv->right = min(n, i + BLOCK - 1);
            irv->arr = prefix_sum + i;
            irv->key = block_sum; // use key to pass block sum

            pthread_create(&threads[used_thread], NULL, collectall, irv);
            block_sum += prefix_sum[min(i+BLOCK-1, n)];
            i+=BLOCK;
            used_thread++;
        }
        for (int i = 0; i < used_thread; i++)
           pthread_join(threads[i], NULL);


        output(prefix_sum, n);
    }
    return 0;
}
