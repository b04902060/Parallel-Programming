#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int queen_map[35][35];
int N;
int numSolution = 0;

int abs(int a){
    return a>0? a:-a;
}

int good(int position[], int current, int test_num){
    if(queen_map[current][test_num] == 0) return 0;
    for (int i = 0; i < current; i++)
        if (position[i] == test_num || (abs(test_num - position[i]) == current - i)){
            return 0;
        }
    return 1;
}

int queen(int position[] , int current){
    if (current >= N){
        return 1;
    }
    int sum = 0;
    for (int test_num = 0; test_num < N; test_num++){
        if (good(position, current, test_num) == 1) {
            position[current] = test_num;
            sum += queen(position, current+1);
            position[current] = -100;
        }
    }
    return sum;
}


int main() {
    int cases = 1;
    char s[32];

    while (scanf("%d", &N) != EOF) {
        //set the map
        for (int i = 0; i < N; i++) {
            scanf("%s", s);
            for(int j = 0; j < N; j++){
                if (s[j] == '*') queen_map[i][j] = 0;
                else queen_map[i][j] = 1;
            }
        }

        int position[32];
        for(int i = 0; i < 32; i++)
            position[i] = -100;

        numSolution = 0;
        #pragma omp parallel for collapse(3) schedule(dynamic, 4) private (position) reduction (+:numSolution)
            for(int i = 0; i < N; i++){
                for(int j = 0; j < N; j++){
                    for(int k = 0; k < N; k++){
                        if(queen_map[0][i]==0|| queen_map[2][k]==0 || queen_map[1][j]==0) continue;
                        if(i==j || j==k || i==k || abs(i-j)==1 || abs(j-k)==1 || abs(i-k)==2) continue;
                        position[0] = i;
                        position[1] = j;
                        position[2] = k;
                        numSolution += queen(position, 3);
                    }
                }
            }

        printf("Case %d: %d\n", cases, numSolution);
        cases++;
    }

    return 0;
}
