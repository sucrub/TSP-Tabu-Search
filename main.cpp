/*
	Cho số thành phố ra ngoài
	Khởi tạo mảng động mặc định ngay từ đầu
	Đọc file data
	Mặc định xe xuất phát từ 0
	Chỉnh sửa sang đọc tọa độ, tính tọa độ
*/

#include <bits/stdc++.h>
#include <fstream>
#include <conio.h>
using namespace std;

#define MAX 1000

// Số thành phố
int N;

// Ma trận khoảng cách giữa các thành phố
int **graph;

// Mảng lời giải là thứ tự di chuyển các thành phố
int *sol = new int[MAX];

/*
	Khởi tạo mảng hai chiều random
	Mảng chứa khoảng cách di chuyển từ thành phố i đến j
*/
// void make_graph() {

// 	// Khởi tạo mảng động hai chiều
// 	graph = new int *[MAX];
// 	int *temp = new int[MAX * MAX];
// 	for (int i = 0; i < MAX; i++) {
// 		graph[i] = temp;
// 		temp += MAX;
// 	}

// 	// n là số thành phố
// 	int n = 7;

// 	// Nhập số
// 	for (int i = 0; i < n; i++)
// 		for (int j = 0; j < n; j++)
// 			cin >> graph[i][j];
// }

void readFile() {

	ifstream f;
	f.open("test2.txt");
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
	Hàm tính giá trị quãng đường đi theo thứ tự thành phố trong sol
	Tham số chiếu vào:
		sol: Thứ tự quãng đường đi
		graph: Ma trận khoảng cách giữa các thành phố
		n: Số thành phố
*/
int fitness(int *sol) {

	int path = 0; // độ dài quãng đường

	for (int i = 0; i < N - 1; i++) {
		path = path + graph[sol[i]][sol[i + 1]];
	}

	path += graph[sol[N - 1]][0]; // quay lại thành phố đầu
	return path;
}

/*
	Hàm khởi tạo kết quả đầu tiên
	Tham số chiếu vào:
		graph: Ma trận khoảng cách giữa các thành phố
		n: Số thành phố
*/
int *first_sol() {

	srand(time(NULL));
	int r;
	// Kiểm tra thành phố đã đi qua chưa
	map<int, bool> vis;

	int *tmp = new int[MAX];
	tmp[0] = 0;

	// Random thành phố (?)
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
	Hàm Tabu Search
	Tham số chiếu vào:
		graph: Ma trận khoảng cách giữa các thành phố
		n: Số thành phố
*/
void tabuSearch() {

	int tabu_list[N]; // Tabu list
	memset(tabu_list, N, 0);

	sol = first_sol(); // Khởi tạo solution đầu tiên

	int best_val = fitness(sol); // Value cho solution tốt nhất
	// cout << "////////GIA TRI KHOI TAO/////////" << endl;
	// cout << best_val << endl;
	// for(int i = 0; i < N; i++) {
	// 	cout << sol[i] << " ";
	// }
	// cout << endl << "/////////////////" << endl;

	int *best_sol = new int [MAX]; // Solution tốt nhất
	for (int i = 0; i < N; i++) best_sol[i] = sol[i];

	int *best_candidate = new int [MAX]; // Trường hợp tốt nhất từ vòng lặp trước
	for (int i = 0; i < N; i++) best_candidate[i] = sol[i];

	int *neighbor_candidate = new int [MAX]; // Miền láng giềng tốt nhất đang xét 
	for (int i = 0; i < N; i++) neighbor_candidate[i] = sol[i];

	bool stop = false; // Điều kiện dừng

	int t = (int)sqrt(N); // Tabu tenure

	int best_keepping = 0; // Số lần không thay đổi best solution

	int stopping_turn = 1000; // Nếu best_keeping == stopping_turn thì stop = true

	while(!stop) {

		int city1, city2;
		int cmp = INT_MAX;

		for(int i = 1; i < N; i++) {
			for(int j = 1; j < N; j++) {

				if(i == j) continue;

				int* tmp_sol = new int [MAX]; // Solution hiện tại đang xét đến 
				for(int i = 0; i < N; i++) {
					tmp_sol[i] = best_candidate[i];
				}

				swap(tmp_sol[i], tmp_sol[j]); // Đảo vị trí hai thành phố 

				// Kiểm tra có tabu không
				if(tabu_list[i] == 0 && tabu_list[j] == 0) {
					
					// Nếu không tabu thì xét xem đây có phải phương án tốt nhất trong láng giềng
					if(fitness(tmp_sol) < cmp) {

						// Cập nhật giá trị 
						for (int i = 0; i < N; i++) neighbor_candidate[i] = tmp_sol[i];
						cmp = fitness(neighbor_candidate);
						city1 = i;
						city2 = j;
					}
				}

				// Nếu có tabu
				else {

					// Kiểm tra xem nó có đang hơn best không
					if(fitness(tmp_sol) < best_val
						&& fitness(tmp_sol) < cmp) {

						// Cập nhật giá trị
						for (int i = 0; i < N; i++) neighbor_candidate[i] = tmp_sol[i];
						cmp = fitness(neighbor_candidate);
						city1 = i;
						city2 = j;
					}

					else { 
						delete[] tmp_sol; 
						continue;
					}
				}

				// Sau khi xét xong thì ứng cử tốt nhất sẽ là neighbor_candidate, các city sẽ đổi là city1 và city2
			delete [] tmp_sol;
			}
		}

		// Cập nhật giá trị
		for (int i = 0; i < N; i++) best_candidate[i] = neighbor_candidate[i];
		if(fitness(best_candidate) < best_val) {

			for (int i = 0; i < N; i++) best_sol[i] = best_candidate[i];
			best_val = fitness(best_sol);
			best_keepping = -1;
		}

		// Cập nhật tabu list
		if(tabu_list[city1] == 0 && tabu_list[city2] == 0) {
			for(int i = 0; i < N; i++) {
				if(tabu_list[i] > 0) tabu_list[i]--;
			}
			tabu_list[city1] += t;
		}

		else {
			for(int i = 0; i < N; i++) {
				tabu_list[i] = 0;
			}
			tabu_list[city1] += t;
		}
		
		// cout << best_val << endl;
		// for(int i = 0; i < n; i++) {
		// 	cout << best_sol[i] << " ";
		// }
		// cout << endl;

		//Kiểm tra stop_condition
		best_keepping++;
		if(best_keepping == stopping_turn) {
			stop = true;
			// cout << "Best value: " << best_val << endl;
			// cout << "Route: ";
			// for (int i = 0; i < N; i++)
			// 	cout << best_sol[i] << " ";
		}
		// getch();
	}

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
	// delete[] sol;
	// for (int i = 0; i < MAX; i++)
	// 	delete[] graph[i];
	// delete[] graph;
}
