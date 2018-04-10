#include <stdio.h>
#include <stdlib.h>
#define P 10

int main(){
    int N;
    int a[1100000];

    scanf("%d", &N);
    for(int i = 0; i < N; i++)  scanf("%d", &a[i]);

    // every processor compute the prefix of their own domain
    for(int p = 0; p < P; p++)
        for(int i = 1; i < N/P; i++)
            a[p*(N/P) + i] += a[p*(N/P) + i-1];


    //log n algorithm prefix
    int jump = 1;
    for(int i = 1; i <= 4; i++){
        for(int j = P; j >= 1; j--){
            if ((j-jump) < 1) continue;
            //printf("i = %d, j = %d, %d + %d\n", i, j, a[j*(N/P)-1], a[(j-jump)*(N/P)-1]);
            a[j*(N/P)-1] += a[(j-jump)*(N/P)-1];
        }
        jump *= 2;
    }

    // every p-th processor add the last index of (p-1)th domain to their domain
    for(int p = 1; p < P; p++)
        for(int i = 0; i < N/P-1; i++)
            a[p*(N/P)+i] += a[p*(N/P)-1];


    //print the result
    for(int i = 0; i < N; i++){
        printf("%d", a[i]);
        if (i != N-1) printf(" ");
    }

    printf("\n");
    return 0;

}
