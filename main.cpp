#include <bits/stdc++.h>
#include <fstream>
#include <conio.h>
using namespace std;

// The travelling man start from city 0

#define MAX 1000

// Number of city
int N;

// Distance matrix between each city
int **graph;

// Route solution is an array
int *sol = new int[MAX];

/*
	Read problem from file
*/
void readFile() {

	ifstream f;
	f.open("test.txt");
	f >> N;

	graph = new int *[N];
	for(int i = 0; i < N; i++) {
		graph[i] = new int [N];
	}

	for(int i = 0; i < N; i++) {
		for(int j = 0; j < N; j++) {
			f >> graph[i][j];
		}
	}
	f.close();
}

/*
	Caculated the length of the solution's route
*/
int fitness(int *sol) {

	int path = 0; // Path's length

	for (int i = 0; i < N - 1; i++) {
		path = path + graph[sol[i]][sol[i + 1]];
	}

	path += graph[sol[N - 1]][0]; // Back to the first city
	return path;
}

/*
	Create a first solution randomly
*/
int *first_sol() {

	srand(time(NULL));
	int r;

	// To check if we have pass that city
	map<int, bool> vis;

	int *tmp = new int[MAX];
	tmp[0] = 0;

	// Random route
	for (int i = 1; i < N; i++) {
		do {
			r = 1 + rand() % ((N - 1) + 1 - 1);
		} while (vis.find(r) != vis.end());
		vis[r] = true;
		tmp[i] = r;
	}
	return tmp;
}

/*
	Tabu Search
*/
void tabuSearch() {

	int tabu_list[N]; // Tabu list
	memset(tabu_list, N, 0);

	sol = first_sol(); // Make a first solution

	int best_val = fitness(sol); // Best value for this problem that tabu search can find 

	int *best_sol = new int [MAX]; // Best solution for this problem that tabu search can find
	for (int i = 0; i < N; i++) best_sol[i] = sol[i];

	int *best_candidate = new int [MAX]; // Best solution from last loop
	for (int i = 0; i < N; i++) best_candidate[i] = sol[i];

	int *neighbor_candidate = new int [MAX]; // Best solution for current neighbor 
	for (int i = 0; i < N; i++) neighbor_candidate[i] = sol[i];

	bool stop = false; // stop condition

	int t = (int)sqrt(N); // Tabu tenure

	int best_keepping = 0; // Time that the best value unchange

	int stopping_turn = 1000; // If best_keeping == stopping_turn then stop = true

	while(!stop) {

		int city1, city2;
		int cmp = INT_MAX;

		for(int i = 1; i < N; i++) {
			for(int j = 1; j < N; j++) {

				if(i == j) continue;

				int* tmp_sol = new int [MAX]; // Current solution 
				for(int i = 0; i < N; i++) {
					tmp_sol[i] = best_candidate[i]; // Equal to the last loop's solution for base
				}

				swap(tmp_sol[i], tmp_sol[j]); // Swap two city to make a new route 

				// Check the route if its tabu
				// Doesn't tabu
				if(tabu_list[i] == 0 && tabu_list[j] == 0) {
					
					// If this solution is currently the best one so far in this neighbor
					if(fitness(tmp_sol) < cmp) {

						// Update the value 
						for (int i = 0; i < N; i++) neighbor_candidate[i] = tmp_sol[i];
						cmp = fitness(neighbor_candidate);
						city1 = i;
						city2 = j;
					}
				}

				// Does tabu
				else {

					// Check if this solution is currently better than the best one so far of tabu search
					if(fitness(tmp_sol) < best_val
						&& fitness(tmp_sol) < cmp) {

						// Update the value
						for (int i = 0; i < N; i++) neighbor_candidate[i] = tmp_sol[i];
						cmp = fitness(neighbor_candidate);
						city1 = i;
						city2 = j;
					}

					// Discard it when it doesn't meet the requirement
					else { 
						delete[] tmp_sol; 
						continue;
					}
				}

			delete [] tmp_sol;
			}
		}

		//After checking out all the neighbor, best one stay in neighbor_candidate, city1 and city2 is the two cities that swap

		// Update the value
		for (int i = 0; i < N; i++) 
			best_candidate[i] = neighbor_candidate[i]; // Update the best value from the last loop

		if(fitness(best_candidate) < best_val) { // If this solution is better than what we have so far

			for (int i = 0; i < N; i++) best_sol[i] = best_candidate[i];
			best_val = fitness(best_sol);
			best_keepping = -1;
		}

		// Update the tabu list

		if(tabu_list[city1] == 0 && tabu_list[city2] == 0) { // If the solution is not tabu
			for(int i = 0; i < N; i++) {
				if(tabu_list[i] > 0) tabu_list[i]--;
			}
			tabu_list[city1] += t;
		}

		else { // If the solution is tabu
			for(int i = 0; i < N; i++) {
				tabu_list[i] = 0;
			}
			tabu_list[city1] += t;
		}

		// Checking the stop_condition
		best_keepping++;
		if(best_keepping == stopping_turn) {
			stop = true;
		}
	}

	// Print the result
	cout << "Best value: " << best_val << endl;
	cout << "Route: ";
	for (int i = 0; i < N; i++)
		cout << best_sol[i] << " ";
	cout << endl;

	delete [] best_sol;
	delete [] best_candidate;
	delete [] neighbor_candidate;
}

int main() {
	readFile();
	tabuSearch();

	// Comment this so it won't delay when testing
	// delete[] sol;
	// for (int i = 0; i < MAX; i++)
	// 	delete[] graph[i];
	// delete[] graph;
}
