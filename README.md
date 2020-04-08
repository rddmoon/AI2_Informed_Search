# 8 Puzzle Heuristic


File:

# Penjelasan

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
