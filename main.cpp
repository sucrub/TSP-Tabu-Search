/*
	Cho số thành phố ra ngoài
	Khởi tạo mảng động mặc định ngay từ đầu
	Đọc file data
*/

#include <bits/stdc++.h>
using namespace std;

#define MAX 1000

// Ma trận khoảng cách giữa các thành phố
int **graph;

// Mảng lời giải là thứ tự di chuyển các thành phố
int *sol = new int[MAX];

/*
	Khởi tạo mảng hai chiều random
	Mảng chứa khoảng cách di chuyển từ thành phố i đến j
*/
void make_graph() {

	// Khởi tạo mảng động hai chiều
	graph = new int *[MAX];
	int *temp = new int[MAX * MAX];
	for (int i = 0; i < MAX; i++) {
		graph[i] = temp;
		temp += MAX;
	}

	// n là số thành phố
	int n = 7;

	// Nhập số
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			cin >> graph[i][j];
}

/*
	Hàm tính giá trị quãng đường đi theo thứ tự thành phố trong sol
	Tham số chiếu vào:
		sol: Thứ tự quãng đường đi
		graph: Ma trận khoảng cách giữa các thành phố
		n: Số thành phố
*/
int fitness(int *sol, int **graph, int n) {

	int path = 0; // độ dài quãng đường

	for (int i = 0; i < n - 1; i++)
	{
		path = path + graph[i][i + 1];
	}

	path += graph[sol[n - 1]][0]; // quay lại thành phố đầu
	return path;
}

/*
	Hàm khởi tạo kết quả đầu tiên
	Tham số chiếu vào:
		graph: Ma trận khoảng cách giữa các thành phố
		n: Số thành phố
*/
int *first_sol(int **graph, int n) {

	srand((int)time(0));
	int r;
	// Kiểm tra thành phố đã đi qua chưa
	map<int, bool> vis;

	int *tmp = new int[MAX];
	tmp[0] = 0;

	// Random thành phố (?)
	for (int i = 1; i < n; i++) {
		do {
			r = 1 + rand() % ((n - 1) + 1 - 1);
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
void tabuSearch(int **graph, int n) {

	int tabu_list[n]; // Tabu list

	sol = first_sol(graph, n); // Khởi tạo solution đầu tiên

	int best_val = fitness(sol, graph, n); // Value cho solution tốt nhất

	int *best_sol = sol; // Solution tốt nhất

	int *best_candidate = sol; // Trường hợp tốt nhất từ vòng lặp trước

	int *neighbor_candidate = sol; // Miền láng giềng tốt nhất đang xét 

	bool stop = false; // Điều kiện dừng

	int t = sqrt(n); // Tabu tenure

	int best_keepping = 0; // Số lần không thay đổi best solution

	int stopping_turn = 500; // Nếu best_keeping == stopping_turn thì stop = true

	while (!stop) {

		int city1, city2;
		int cmp = INT_MAX;

		// tim kiem cac gia tri lan can tot nhat
		for (int i = 1; i < n; i++)
			for (int j = 1; j < n; j++) {

				if (i == j)
					continue;
				
				int *tmp_sol = best_candidate; // Solution hiện tại đang xét đến 
				swap(tmp_sol[i], tmp_sol[j]); // Đảo vị trí hai thành phố 

				// Nếu 1 trong hai thành phố đang trong trạng thái Tabu
				if (tabu_list[i] > 0 || tabu_list[j] > 0) { 
					// Kiểm tra kết quả đấy có tốt hơn best solution không? Nếu có thì là aspiration condition
					// Bỏ qua việc đang là trạng thái tabu và xét duyệt kết quả này
					if (fitness(tmp_sol, graph, n) < fitness(best_sol, graph, n)) {

						best_sol = tmp_sol;
						best_val = fitness(tmp_sol, graph, n);
						best_keepping = 0;
						for (int i = 0; i < n; i++)
							tabu_list[i] = 0;
						break;
					}
					else
						continue;
				}
				if (fitness(tmp_sol, graph, n) < cmp) {

					neighbor_candidate = tmp_sol;
					cmp = fitness(neighbor_candidate, graph, n);
					city1 = i;
					city2 = j;
				}
			}
		best_candidate = neighbor_candidate;
		// neu lan can tim duoc chua thuoc tabu list thi dua vao
		if (tabu_list[city1] == 0 && tabu_list[city2] == 0) {

			tabu_list[city1] = t;
		}
		// neu gia tri tim thay tot hon gia tri tot nhat hien c� th� g�n lai cac gia tri
		if (fitness(best_candidate, graph, n) < fitness(best_sol, graph, n)) {

			best_sol = best_candidate;
			best_val = fitness(best_candidate, graph, n);
			best_keepping = 0;
		}
		// giam cac luot lap trong day tabu
		for (int i = 0; i < n; i++) {

			if (tabu_list[i] > 0)
				tabu_list[i]--;
		}
		// kiem tra dk dung: so lan ko doi gia tri tot nhat = stopping_turn
		if (best_keepping == stopping_turn)
			stop = true;
		best_keepping += 1;
	}
	cout << best_val << endl;
	for (int i = 0; i < n; i++)
		cout << best_sol[i] << " ";
}

void test(int **graph, int n) {

	int tabu_list[n]; // Tabu list
	memset(tabu_list, n, 0);

	sol = first_sol(graph, n); // Khởi tạo solution đầu tiên

	int best_val = fitness(sol, graph, n); // Value cho solution tốt nhất

	int *best_sol = sol; // Solution tốt nhất

	int *best_candidate = sol; // Trường hợp tốt nhất từ vòng lặp trước

	int *neighbor_candidate = sol; // Miền láng giềng tốt nhất đang xét 

	bool stop = false; // Điều kiện dừng

	int t = sqrt(n); // Tabu tenure

	int best_keepping = 0; // Số lần không thay đổi best solution

	int stopping_turn = 500; // Nếu best_keeping == stopping_turn thì stop = true

	while(!stop) {

		int city1, city2;
		int cmp = INT_MAX;

		for(int i = 0; i < n; i++) {
			for(int j = 0; j < n; j++) {

				if(i == j) continue;

				int *tmp_sol = best_candidate; // Solution hiện tại đang xét đến 
				swap(tmp_sol[i], tmp_sol[j]); // Đảo vị trí hai thành phố 

				// Kiểm tra có tabu không
				if(tabu_list[i] == 0 && tabu_list[j] == 0) {
					
					// Nếu không tabu thì xét xem đây có phải phương án tốt nhất trong láng giềng
					if(fitness(tmp_sol, graph, n) < cmp) {

						// Cập nhật giá trị 
						neighbor_candidate = tmp_sol;
						cmp = fitness(neighbor_candidate, graph, n);
						city1 = i;
						city2 = j;
					}
				}

				// Nếu có tabu
				else {

					// Kiểm tra xem nó có đang hơn best không
					if(fitness(tmp_sol, graph, n) < best_val) {

						// Cập nhật giá trị
						neighbor_candidate = tmp_sol;
						cmp = fitness(neighbor_candidate, graph, n);
						city1 = i;
						city2 = j;
					}
				}

				// Sau khi xét xong thì ứng cử tốt nhất sẽ là neighbor_candidate, các city sẽ đổi là city1 và city2

			}
		}

		// Cập nhật giá trị
		best_candidate = neighbor_candidate;
		if(fitness(best_candidate, graph, n) < best_val) {

			best_sol = best_candidate;
			best_val = fitness(best_candidate, graph, n);
		}

		// Cập nhật tabu list
		for(int i = 0; i < n; i++) {
			if(tabu_list[i] > 0) tabu_list[i]--;
		}

		tabu_list[city1] += t;
		tabu_list[city2] += t;
		
	}
}

int main() {
	make_graph();
	tabuSearch(graph, 7);
	delete[] sol;
	for (int i = 0; i < MAX; i++)
		delete[] graph[i];
	delete[] graph;
}
