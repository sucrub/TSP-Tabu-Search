#include<bits/stdc++.h>
#define max 1000
using namespace std;
int** graph;
int* sol = new int [max];
void make_graph(){// tao do thi
	graph = new int * [max];
	int *temp = new int [max*max];
	for (int i=0; i<max; i++){
		graph[i] = temp;
		temp += max;
	}
	int n = 7; // n la so thanh pho
	for (int i=0; i<n; i++)
		for (int j=0; j<n; j++)
			cin >> graph[i][j];	
}
int fitness(int* sol, int** graph, int n){ // tinh gia tri quang duong di theo thu tu trong mang sol
	int path = 0;
	for (int i=0; i<n-1; i++){
			path = path + graph[i][i+1];
	}
	path += graph[sol[n-1]][0];
	return path;
}
int* first_sol(int** graph, int n){// khoi tao truong hop dau tien (can lam back tracking)
	srand((int)time(0));
	int r;
	map<int, bool> vis;
	int* tmp = new int [max];
	tmp[0] = 0;
	for (int i=1; i<n; i++){
		do{
			r = 1 + rand() % ((n-1) + 1 - 1);
		}while (vis.find(r) != vis.end());
		vis[r] = true;
		tmp[i] = r;
	}
	return tmp;
}
void TSP(int** graph, int n){
	int tabu_list[n];// list tabu
	sol = first_sol(graph, n);// khoi tao
	int best_val = fitness(sol, graph, n);// gia tri duong di ban dau
	int* best_sol = sol;// truong hop tot nhat
	int* best_candidate = sol;// truong hop tot nhat tu vong lap truoc
	int* neighbor_candidate = sol;// cac truong hop tim duoc trong vong lap
	bool stop = false;// dk dung
	int t = sqrt(n);// so lan lap tabu cua 1 thanh pho
	int stopping_turn = 500;
	int best_keepping = 0;// so lan ko thay doi gia tri tot nhat
	while (!stop){
		int city1, city2; int cmp = INT_MAX;
		// tim kiem cac gia tri lan can tot nhat
		for (int i=1; i<n; i++)
			for (int j=1; j<n; j++){
				if (i == j) continue;
				int* tmp_sol = best_candidate;
				swap(tmp_sol[i], tmp_sol[j]);
				if (tabu_list[i] > 0 || tabu_list[j] > 0){
					if (fitness(tmp_sol, graph, n) < fitness(best_sol, graph, n)){
						best_sol = tmp_sol;
						best_val = fitness(tmp_sol, graph, n);
						best_keepping = 0;
						for (int i=0; i<n; i++) tabu_list[i] = 0;
						break;
					}
					else continue;
				}
				if (fitness(tmp_sol, graph, n) < cmp){
					neighbor_candidate = tmp_sol;
					cmp = fitness(neighbor_candidate, graph, n);
					city1 = i; city2 = j;
				}
			}
		best_candidate = neighbor_candidate;
		// neu lan can tim duoc chua thuoc tabu list thi dua vao
		if (tabu_list[city1] == 0 && tabu_list[city2] == 0){
			tabu_list[city1] = t;
		}
		// neu gia tri tim thay tot hon gia tri tot nhat hien có thì gán lai cac gia tri
		if (fitness(best_candidate, graph, n) < fitness(best_sol, graph, n)){
			best_sol = best_candidate;
			best_val = fitness(best_candidate, graph, n);
			best_keepping = 0;
		}
		// giam cac luot lap trong day tabu
		for (int i=0; i<n; i++){
			if (tabu_list[i] > 0)
				tabu_list[i]--;
		}
		// kiem tra dk dung: so lan ko doi gia tri tot nhat = stopping_turn
		if (best_keepping == stopping_turn)
			stop = true;
		best_keepping += 1;
	}
	cout << best_val << endl;
	for (int i=0; i<n; i++) cout << best_sol[i] << " ";
}
int main(){
	make_graph();
	TSP(graph, 7);
	delete [] sol;
	for (int i=0; i<max; i++)
		delete [] graph[i];
	delete [] graph;
}
