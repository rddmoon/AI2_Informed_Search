# 8 Puzzle Heuristic Function
  Pada penyelesaian 8 puzzle dengan heuristic ini digunakan graph, kita gambarkan searchingnya sebagai graph. Setiap state pada graph digambarkan sesuai konfigurasi puzzlenya kemudian di ekspansi memiliki 4 child atau successor, tiap child dibentuk state puzzle yang menyesuaikan gerakan dari menggeser angka ke arah space yang kosong (up, left, right, down) dari state parent. Kemudian tiap node child mengekspansi dengan cara yang sama.

## 8 Puzzle Heuristic 1
 Metode penyelesaian heuristic 1 adalah metode penyelesaian dengan menggunakan fungsi h1 dimana h1 merupakan jumlah kotak yang masih tidak berada pada letaknya sesuai goal state. Pada perhitungan h1 puzzle yang bernilai '0' atau space kosong tidak ikut dihitung walaupun jika sudah tepat letaknya. Penelusuran untuk tiap node menggunakan pertimbangan nilai h1, dimana nilai h1 yang lebih kecil lah yang dipilih karena dianggap lebih medekati goal state.

File:
- <a href ="https://github.com/rddmoon/AI2_Informed_Search/blob/master/8-puzzle-heuristic/8-puzzzle-h1.cpp">8-puzzle-h1.cpp</a>

## Penjelasan
Class dari node
```
struct node{
    vector<int> initial;
    int g_cost, h_cost, f_cost;
    node* parent;

    node(){
        g_cost = 0;
        h_cost = 0;
        f_cost = 0;
        parent = NULL;
    }
};
```
Operator overloading untuk priority queue
```
struct comp{
    bool operator()(const node* lhs, const node* rhs) const{
        return lhs->f_cost > rhs->f_cost;
    }
};
```
Global variabel yang diperlukan, N merupakan banyaknya kotak pada puzzle ini, yaitu 9 termasuk space kosong.
```
int N = 9;
vector<int> goal;
vector<node*> visited;
priority_queue<node*, vector<node*>, comp> to_be_explored;
```
Fungsi print merupakan fungsi untuk menampilkan state board saat ini.
```
void print(node* n){
    int dim = sqrt(N);
    int i, j, k = 0;
    for(i = 0; i < dim; ++i){
        for(j = 0; j < dim; ++j){
        	printf("%d ", n->initial[k++]);
		}
        printf("\n");
    }
    printf("\n");
}
```
Fungsi path_solution merupakan fungsi untuk menampilkan path untuk solusi. Di sinilah fungsi print sebelumnya dipanggil.
```
void path_solution(node* n){
    node* temp = n;
    int i;
    vector<node*> totalpath;	//it's a list for containing all the nodes in the path from the root to the goal
    while(temp != NULL){
        totalpath.push_back(temp);
        temp = temp->parent;
    }

    int sz = totalpath.size();
    //print number of moves required
    printf("Moves: %d\n", sz-1);

    //show the moves one by one
    for(i = sz-1; i >= 0; --i){
    	print(totalpath[i]);
	}
}
```
Fungsi di bawah ini adalah fungsi untuk menghitung banyaknya kotak pada 8 puzzle yang tidak sesuai letaknya dengan goal state. Fungsi mengembalikan jumlah h1 pada state yang dihitung h1 nya.
```
int calculate_misplaced_tiles(node* n){
    int cnt = 0;
    int i;
    for(i = 0; i < N; ++i){
    	if(goal[i] != n->initial[i]){
    		++cnt;
		}
	}
    return cnt;
}
```
Fungsi di bawah merupakan fungsi untuk mengisi node child dengan state yang sesuai.
```
node* create_successor(node* state, int pos1, int pos2){
    node* new_state = new node();
    new_state->initial = state->initial;
    swap(new_state->initial[pos1], new_state->initial[pos2]);
    new_state->h_cost = calculate_misplaced_tiles(new_state);
    new_state->g_cost = state->g_cost+1;
    new_state->f_cost = new_state->h_cost + new_state->g_cost;
    new_state->parent = state;

    return new_state;
}
```
Fungsi di bawah ini merupakan fungsi untuk membuat node-node child dari node state saat ini.
```
vector<node*> get_successor(node* n){
    int pos, row, col, dim;
    for(int i = 0; i < N; ++i){
        if(n->initial[i] == 0){
            pos = i;
            break;
        }
    }
    dim = sqrt(N);
    row = pos / dim;
    col = pos % dim;

    vector<node*> successors;

    if(col != 0){
    	successors.push_back(create_successor(n, pos, pos-1));
	}
    if(col != dim-1){
    	successors.push_back(create_successor(n, pos, pos+1));
	}
    if(row != 0){
    	successors.push_back(create_successor(n, pos, pos-dim));
	}
    if(row != dim-1){
    	successors.push_back(create_successor(n, pos, pos+dim));
	}
    return successors;
}
```
Fungsi di bawah merupaakn fungsi untuk mengecek apakah state saat ini adala goal state
```
bool is_Goal(node* n){
    return (n->h_cost == 0) ? 1 : 0;
}
```
Fungsi check_visited merupakan fungsi untuk mengecek apakah node sudah pernah di datangi sebelumnya.
```
bool check_visited(node* n){
    int sz = visited.size(), i, j;
    for(i = 0; i < sz; ++i){
        for(j = 0; j < N; ++j){
            if(n->initial[j] != visited[i]->initial[j]){
            	break;
			} 
        }
        if(j == N){
        	return 1;
		}   
    }
    return 0;
}
```
Fungsi di bawah merupakan fungsi untuk menyelesaikan 8 puzzlenya.
```
void Asearch(node* n){
    n->h_cost = calculate_misplaced_tiles(n);
    n->f_cost = n->h_cost;
    n->parent = NULL;
    to_be_explored.push(n);

    bool flag;
    int tentative_g_cost, sz, i, k;
    node *current, *temp;
    vector<node*> current_successors;
    priority_queue<node*, vector<node*>, comp> pq;

    while(!to_be_explored.empty()){
        current = to_be_explored.top();
        to_be_explored.pop();
        visited.push_back(current);

        if(is_Goal(current)){
            path_solution(current);
            return;
        }

        current_successors.clear();
        current_successors = get_successor(current);

        sz = current_successors.size();
        for(i = 0; i < sz; ++i){
            if(check_visited(current_successors[i])){
            	continue;
			}
            tentative_g_cost = current->g_cost+1;

            while(!pq.empty()){
            	pq.pop();
			}
            while(!to_be_explored.empty()){
                temp = to_be_explored.top();
                to_be_explored.pop();

                flag = 0;
                for(k = 0; k < N; ++k){
                	if(current_successors[i]->initial[k] != temp->initial[k]){
                		break;
					} 
				}
                if(k == N){
                	flag = 1;
				}
                if(flag && tentative_g_cost < temp->g_cost){
                    temp->parent = current;
                    temp->g_cost = tentative_g_cost;
                    temp->f_cost = temp->g_cost + temp->h_cost;
                }
                pq.push(temp);
            }
            if(!flag){
            	pq.push(current_successors[i]);
			} 
            to_be_explored = pq;
        }
    }

    return;
}
```
Fungsi main berisi definisi untuk goal state dan menampilkannya ke layar, meminta input untuk initial state, dan memanggil fungsi untuk menyelesaikan 8 puzzle.
```
int main(){
    int i, x;
    node* newnode = new node();
    
    goal.push_back(1);
    goal.push_back(2);
    goal.push_back(3);
    goal.push_back(4);
    goal.push_back(5);
    goal.push_back(6);
    goal.push_back(7);
    goal.push_back(0);
    goal.push_back(8);
    
    printf("Goal State:\n");
    for(i = 0; i < 3; ++i){
        printf("%d ", goal[i]);
    }
    printf("\n");
    for(i = 3; i < 6; ++i){
        printf("%d ", goal[i]);
    }
    printf("\n");
    for(i = 6; i < N; ++i){
        printf("%d ", goal[i]);
    }
    printf("\n");
	printf("Input initial state:\n");
    for(i = 0; i < N; ++i){
        scanf("%d", &x);
        newnode->initial.push_back(x);
    }

    Asearch(newnode);
}
```

## 8 Puzzle Heuristic 2
  Metode penyelesaian heuristic 2 adalah metode penyelesaian dengan menggunakan fungsi h2 dimana h2 merupakan total penjumlahan dari jarak atau pergeseran tiap kotak dari letaknya yang sesuai dengan goal state (space kosong '0' tidak dihitung). Maka untuk tiap satu state memiliki nilai h2 tersendiri. Penelusuran untuk tiap node menggunakan pertimbangan nilai h2, dimana nilai h2 yang lebih kecil lah yang dipilih karena dianggap lebih medekati goal state.

File:
-<a href="https://github.com/rddmoon/AI2_Informed_Search/blob/master/8-puzzle-heuristic/8-puzzle-h2.cpp">8-puzzle-h2.cpp</a>

## Penjelasan
Ini merupakan class node.
```
struct Node { 
	Node* parent; 
	int mat[N][N]; 
	int x, y; 
	int cost; 
	int level; 
}; 
```
Fungsi printMatrix merupakan fungsi untuk menampilkan state board ke layar.
```
int printMatrix(int mat[N][N]) { 
	int i, j;
	for (i = 0; i < N; i++) { 
		for (j = 0; j < N; j++) 
			printf("%d ", mat[i][j]); 
		printf("\n"); 
	} 
} 
```
Fungsi di bawah ini merupakan fungsi untuk membuat node baru.
```
Node* newNode(int mat[N][N], int x, int y, int newX, int newY, int level, Node* parent) { 
	Node* node = new Node; 

	node->parent = parent; 
	memcpy(node->mat, mat, sizeof node->mat); 

	swap(node->mat[x][y], node->mat[newX][newY]); 

	node->cost = INT_MAX; 
	node->level = level; 

	node->x = newX; 
	node->y = newY; 

	return node; 
} 
```
Fungsi calculateCost merupakan fungsi untuk menghitung h2 sesuai yang telah dijelaskan sebelumnya.
```
int calculateCost(int initial[N][N], int final[N][N]) { 
	int i, j, count = 0; 
	for (i = 0; i < N; i++){
		for (j = 0; j < N; j++){
			if (initial[i][j] && initial[i][j] != final[i][j]){
				count++;
			}	
		}
	}
	return count; 
} 
```
Fungsi di bawah ini merupakan fungsi untuk membatasi pergeseran space kosong agar tidak keluar batas.
```
int isSafe(int x, int y) { 
	return (x >= 0 && x < N && y >= 0 && y < N); 
} 
```
Fungsi di bawah ini merupakan fungsi untuk melacak path dari initial ke goal state dan memanggil fungsi printMatrix untuk menampilkan state tahapan ke layar.
```
void printPath(Node* root) { 
	if (root == NULL) {
		return;
	} 
	printPath(root->parent); 
	printMatrix(root->mat); 

	printf("\n"); 
} 
```
Fungsi solve merupakan fungsi inti untuk menyelesaikan 8 puzzle dengan initial state yang diberikan. Fungsi mengeksplorasi node-node dengan pertimbangan nilai h2 nya. Fungsi berjalan kurang lebih sama sesuai dengan yang telah dijelaskan di atas. 
```
void solve(int initial[N][N], int x, int y, int final[N][N]) { 
	priority_queue<Node*, std::vector<Node*>, comp> pq; 

	Node* root = newNode(initial, x, y, x, y, 0, NULL); 
	root->cost = calculateCost(initial, final); 

	pq.push(root); 

	while (!pq.empty()) { 
		Node* min = pq.top(); 

		pq.pop(); 

		if (min->cost == 0) { 
			printPath(min); 
			return; 
		} 
		int i;
		for (i = 0; i < 4; i++) { 
			if (isSafe(min->x + row[i], min->y + col[i])){ 
				Node* child = newNode(min->mat, min->x, 
							min->y, min->x + row[i], 
							min->y + col[i], 
							min->level + 1, min); 
				child->cost = calculateCost(child->mat, final); 

				pq.push(child); 
			} 
		} 
	} 
}
```
Pada fungsi main, didefinisikan nilai untuk goal state dan menampilkannya ke layar, meminta input untuk initial state, dan terakhir memanggil fungsi solve untuk menyelesaikan puzzle yang diberikan.
```
int main() { 
	int i, j;
	int initial[N][N];
	int goal[N][N] = { {1, 2, 3}, {4, 5, 6}, {7, 0, 8} }; 
	printf("Goal State:\n");
    for(i = 0; i < N; ++i){
    	for(j = 0; j < N; j++){
    		printf("%d ", goal[i][j]);
		}
        printf("\n");
    }
    printf("\n");
	printf("Input initial state:\n");
    for(i = 0; i < N; ++i){
        for(j = 0; j < N; j++){
    		scanf("%d", &initial[i][j]);
		}
    }
    printf("\n");
	int x = 0, y = 2; 
	
	printf("Solution:\n");
	solve(initial, x, y, goal); 

	return 0; 
} 
```

# 8 Queen Hill CLimb
  Metode hill climb merupakan metode local search & optimation. Berbeda dengan informed maupun uninformed search dimana dicari jalur atau path dari initial state untuk menuju goal state, di sini hanya dibutuhkan kondisi final goal state nya. Goal state pada hill climb merupakan posisi dari 8 ratu pada board dengan jumlah bidak tidak saling menyerang seminimal mungkin, biasa disebut global maxima. 
  
  Hill climb memilih state berdasarkan nilai objektif pada board. Pada awalnya, akan dicek tetangga-tetangga dari state saat ini. Kemudian, dipilih state yang memiliki kualitas terbaik dan pindah ke state tersebut, dilakukan seperti itu terus menerus hingga tetangga-tetangga dari state saat ini memiliki kualitas lebih buruk daripada state saat ini. Terkahir, mengembalikan state saat ini sebagai solusi. Dalam menjalankan metode hill climb dapat ditemui masalah seperti terjebak pada local maxima, dimana state board selanjutnya yang terbaik bernilai lebih rendah daripada state saat ini. 
  
  Program ini nantinya akan melakukan random untuk menentukan initial state, kemudian menggunakan hill climb untuk menemukan solusi dari goal state dan menampilkannya ke layar. Tidak ada input yang diminta, user hanya perlu menekan tombol enter terus menerus selama program menampilkan jalannya hill climb hingga program menampilkan solusi.

File:
- <a href ="https://github.com/rddmoon/AI2_Informed_Search/blob/master/8-queen-hillclimb/8-queen-hillclimb.cpp">8-queen-hillclimb.cpp</a>

# Penjelasan
Fungsi calculate merupakan fungsi untuk menghitung nilai objektif dari board untuk menentukan kualitasnya. Pada fungsi dilakukan cek apabila ada ratu yang saling menyerang dan menghitung jumlah banyaknya ratu yang saling serang. Fungsi mengembaikan nilai 28 dikurangi jumlah ratu saling serang. Di sini di set best value dari board adalah 28, sehingga nilai lebih rendah dari 28 dan semakin rendah semakin jelek kualitas state board.
```
int calculate(int (&board)[N]){
	
	int cnt = 0;
	int i, j;
	for(i = 0; i < 8; ++i){
		for(j = i+1; j < 8; ++j){
			if(board[i] == board[j]){
				++cnt;
			}
			else if(abs(i-j) == abs(board[i]-board[j])){
				++cnt;
			}
		}
	}
	return 28-cnt;			
}
```
Fungsi print merupakan fungsi untuk menampilkan isi board ke layar
```
void print(int (&board)[N]){
	
	int grid[10][10];
	int i, j;
	memset(grid, 0, sizeof grid);
	
	for(i = 0; i < 8; ++i){
		grid[ board[i] ][i] = 1;
	}
	
	for(i = 0; i < 8; ++i){
		
		for(j = 0; j < 8; ++j){
			printf("%d ", grid[i][j]);
		}
		printf("\n");
	}
	return;
}
```
Fungsi copy merupakan fungsi untuk menyalin isi board ke board lain.
```
void copy(int (&board1)[N], int (&board2)[N]){
	int i;
	
	for(i = 0; i < 8; ++i){
		board1[i] = board2[i];
	}
}
```
Fungsi hillclimb menjalankan metode hill climb seperti yang sudah dijelaskan di atas. Pada bagian ```if(bestValue <= boardValue)``` merupakan kondisi jika terjebak pada local maxima, program akan menampilkan state terbaik ke layar dan keluar dari fungsi. Sehingga kemudian program akan melakukan restart dari posisi acak hingga goal state ditemukan.
```
int hillclimb(int (&board)[N]){
	
	int bestBoard[N], bestValue, boardValue;
	copy(bestBoard, board);
	
	int cnt = 0;
	
	while(1){
		int currBoard[N], currValue;
		copy(currBoard, board);
		boardValue = bestValue = calculate(board);
		
		if(bestValue == 28){
			cin.get();
			printf("Global maxima. After %d moves\n", cnt);
			print(bestBoard);
			return 1;
		}
		++cnt;		
		for(int i = 0; i < 8; ++i){
			int temp = currBoard[i];
			
			for(int j = 0; j < 8; ++j){
				if(j == board[i]){
					continue;
				}
				currBoard[i] = j;
				currValue = calculate(currBoard);
				
				if(currValue > bestValue){
					
					bestValue = currValue;
					copy(bestBoard, currBoard);
				}
			}
			currBoard[i] = temp;
		}
		if(bestValue <= boardValue){
			cin.get();
			printf("Local maxima. After %d moves\n", cnt);
			print(bestBoard);
			return 0;
		}
		copy(board, bestBoard);
	}
}
```
Pada fungsi main melakukan random untuk initial state, melakukan looping dan menampilkan berapa banyak iterasi yang telah dilakukan hingga menemukan global maxima, dan memanggil fungsi hillclimb.
```
int main(){
	
	srand(time(NULL));
	bool success = 0;
	int cnt = 1;
	int board[N];
	int i;
	
	printf ( "Repeatedly press [Enter] to continue . . .\n" );
	while(1){
		for(i = 0; i < 8; ++i){
			board[i] = rand() % 8;
		}
		cin.get();				
		printf("___Iteration: %d___\n", cnt);
		printf("\nInitial:\n");
		print(board);
		success = hillclimb(board);
		++cnt;
		if(success == 1){
			break;
		}	
	}
	return 0;
}
```
