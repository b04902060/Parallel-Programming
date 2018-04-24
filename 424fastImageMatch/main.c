#include <stdio.h>
#include <stdlib.h>

#define THREADS 8


int A[510][510];
int B[510][510];
int C[510][510];

int diff(int startx, int starty, int bx, int by){
    int result = 0;
    for(int i = 0; i < bx; i++){
        for(int j = 0; j < by; j++){
            result += (A[startx+i][starty+j]-B[i][j]) * (A[startx+i][starty+j]-B[i][j]);
        }
    }
    return result;
}

int max(int a, int b){
    return a > b ? a:b;
}

int main(){

    int Ax, Ay, Bx, By;
    while(scanf("%d %d %d %d", &Ax, &Ay, &Bx, &By) == 4){

        for(int i = 0; i < Ax; i++)for(int j = 0; j < Ay; j++)
            scanf("%d", &A[i][j]);
        for(int i = 0; i < Bx; i++)for(int j = 0; j < By; j++)
            scanf("%d", &B[i][j]);

    # pragma omp parallel for
        for(int i = 0; i <= Ax-Bx; i++){
            for(int j = 0; j <= Ay-By; j++){
                C[i][j] = diff(i, j, Bx, By);
            }
        }

        int result = 99999999; // I am lazy.
        int ansx, ansy;
        for(int i = 0; i <= Ax-Bx; i++){
            for(int j = 0; j <= Ay-By; j++){
                if (C[i][j] < result){
                    result = C[i][j];
                    ansx = i;
                    ansy = j;
                }
            }
        }


        printf("%d %d\n", ansx+1, ansy+1);
    }

    return 0;

}
