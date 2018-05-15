#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXN 4096
#define SIDE ( MAXN + 2)


char A[SIDE][SIDE];
char B[SIDE][SIDE];

int main(){
    int n, era;
    while(scanf("%d %d", &n, &era) != EOF){
        memset (A, 0, sizeof(A));
        memset (B, 0, sizeof(A));

        for(int i = 1; i <= n; i++){
            char tmp[MAXN];
            scanf("%s", tmp);
            # pragma omp parallel for
            for(int j = 1; j <= n; j++){
                A[i][j] = tmp[j-1] - '0';
            }
        }

        int nln ;
        for (int g = 0; g < era; g++){
            #pragma omp parallel
            {
                if (g%2 == 0){
                # pragma omp for schedule(static, 64) private(nln)/* from A to B */
                    for (int i = 1; i <= n; i++){
                        for (int j = 1; j <= n; j++){
                            nln = A[i+1][j] + A[i-1][j] + A[i][j+1]+ A[i][j-1] + A[i+1][j+1] + A[i+1][j-1]+ A[i-1][j+1] + A[i-1][j-1];
                            B[i][j] = ((A[i][j] == 0 && nln == 3) || (A[i][j] == 1 && (nln == 2 || nln == 3)));                        }
                        }
                    }
                else{
                    # pragma omp for schedule(static, 64) private(nln)/* from B to A */   // big chunk for cache hit
                    for (int i = 1; i <= n; i++){
                        for (int j = 1; j <= n; j++){
                            nln = B[i+1][j] + B[i-1][j] + B[i][j+1]+ B[i][j-1] + B[i+1][j+1] + B[i+1][j-1]+ B[i-1][j+1] + B[i-1][j-1];
                            A[i][j] = ((B[i][j] == 0 && nln == 3) || (B[i][j] == 1 && (nln == 2 || nln == 3)));
                        }
                    }
                }
            }
        }

        if(era%2){
            for(int i = 1; i <= n; i++){
                for(int j = 1; j <= n; j++){
                    printf("%d", B[i][j]);
                }
                printf("\n");
            }
        }else{
            for(int i = 1; i <= n; i++){
                for(int j = 1; j <= n; j++){
                    printf("%d", A[i][j]);
                }
                printf("\n");
            }
        }
    }

    return 0;
}
