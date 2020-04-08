#include<bits/stdc++.h>
using namespace std;

#define N 8

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

void copy(int (&board1)[N], int (&board2)[N]){
	int i;
	
	for(i = 0; i < 8; ++i){
		board1[i] = board2[i];
	}
}

	
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
