#include "VSQRT.h"
#include <math.h>
#include <x86intrin.h>


void sqrt2(float *begin, float *end) {
    int n = end - begin;
    __m256 vec;

    for (int i = 0; i < n/8; i++){
        vec = _mm256_load_ps(begin);
        _mm256_store_ps(begin, _mm256_sqrt_ps(vec));
        begin+=8;
    }

    for (int i = 0; i < n%8; i++){ // deal with overload
       *begin = sqrt(*begin);
       begin++;
    }
}
