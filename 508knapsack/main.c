#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXX 1000010

int W[MAXX];
int V[MAXX];
int dp[2][MAXX];

int max(int a, int b){
    return a > b ? a:b;
}

int main(){
    int N, M;

    while(scanf("%d %d", &N, &M) != EOF){
        for(int i = 0; i < N; i++){
            scanf("%d %d", &W[i], &V[i]);
        }

        memset(dp, 0, sizeof(dp));
        int a, b;
        a = 0;
        for(int i = 0; i < N; i++){
            b = 1 - a;
            #pragma omp parallel
            {
                int a_omp = a, b_omp = b;
                int v = V[i], w = W[i];
                #pragma omp for
                for (int i = w; i <= M; i++)
                    dp[b_omp][i] = max(dp[a_omp][i-w]+v, dp[a_omp][i]);
                #pragma omp for
                for (int i = 0; i < w; i++)
                    dp[b_omp][i] = dp[a_omp][i];
            }
            a = 1 - a;
        }
        printf("%d\n", dp[a][M]);


    }

    return 0;
}
