#include<bits/stdc++.h>
#include<fstream>
using namespace std;

int main() {
    ifstream f;
    f.open("test.txt");
    int n;
    f >> n;
    cout << n;
    cout << endl;
    int arr[n][n];
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            f >> arr[i][j];
        }
    }
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            cout << arr[i][j] << " ";
        }
        cout << endl;
    }
}