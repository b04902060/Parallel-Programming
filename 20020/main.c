#include <stdio.h>
#include <assert.h>
#include <inttypes.h>
#include <stdint.h>
#include <x86intrin.h>


static inline uint32_t rotate_left(uint32_t x, uint32_t n) {
    return  (x << n) | (x >> (32-n));
}
static inline uint32_t encrypt(uint32_t m, uint32_t key) {
    return (rotate_left(m, key&31) + key)^key;
}

static uint32_t f(int N, int off, uint32_t key1, uint32_t key2) {
    uint32_t sum = 0;
    __m256i vec1, vec2, result, tmp_sum;
    tmp_sum = _mm256_setzero_si256();

    for (int i = 0, j = off; i < N; i+=8, j+=8){
        vec1 = _mm256_set_epi32(encrypt(j, key1), encrypt(j+1, key1), encrypt(j+2, key1), encrypt(j+3, key1), encrypt(j+4, key1), encrypt(j+5, key1), encrypt(j+6, key1), encrypt(j+7, key1));
        vec2 = _mm256_set_epi32(encrypt(j, key2), encrypt(j+1, key2), encrypt(j+2, key2), encrypt(j+3, key2), encrypt(j+4, key2), encrypt(j+5, key2), encrypt(j+6, key2), encrypt(j+7, key2));
        result = _mm256_mullo_epi32(vec1, vec2);
        tmp_sum = _mm256_hadd_epi32(result, tmp_sum);
    }
    uint32_t* r = (uint32_t*) &tmp_sum;
    sum = r[0]+r[1]+r[2]+r[3]+r[4]+r[5]+r[6]+r[7];

    if(N%8 != 0){
        for(int i = 0; i < N%8; i++){
            sum += encrypt(off+8*(N/8)+i, key1) * encrypt(off+8*(N/8)+i, key2);
        }
    }

    return sum;
}
int main() {
    int N;
    uint32_t key1, key2;
    while (scanf("%d %" PRIu32 " %" PRIu32, &N, &key1, &key2) == 3) {
        int t = 100;
        uint32_t sum;
        while(t){
            sum = f(N, 0, key1, key2);
            t--;
        }
        printf("%" PRIu32 "\n", sum);
    }
    return 0;
}
