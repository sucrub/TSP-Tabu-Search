#include<bits/stdc++.h>
#include<fstream>
using namespace std;

int N;
int **graph;

int main() {

    srand(time(NULL));
    N = rand() % (15 - 6 + 1) + 6;
    graph = new int *[N];
    for(int i = 0; i < N; i++) {
        graph[i] = new int [N];
    }
    ofstream f;
    f.open("test2.txt");
    f << N << endl;
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++) {

            if(i == j) {
                graph[i][j] = 0;
                graph[j][i] = 0;
            }
            if(i > j) continue;
            if(i < j) {
                graph[i][j] = rand() % (20 - 1 + 1) + 1;
                graph[j][i] = graph[i][j];
            }
        }
    }
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++) {
            f << graph[i][j] << " ";
        }
        f << endl;
    }
    f.close();
}