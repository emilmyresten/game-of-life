#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/*
 1. Any live cell with fewer than two live neighbors dies, as if by underpopulation.
 2. Any live cell with two or three live neighbors lives on to the next generation.
 3. Any live cell with more than three live neighbors dies, as if by overpopulation.
 4. Any dead cell with exactly three live neighbors becomes a live cell, as if by reproduction.
 */

#define rows 25
#define cols 25

int get_live_neighbors(char* board, int row, int col) {
	int live_neighbors = 0;
	for (int r = -1; r <= 1; r++) {
		for (int c = -1; c <= 1; c++) {
			if ((c == 0 && r == 0) ||
			    (row+r < 0) || 
			    (row+r >= rows) || 
			    (col+c < 0) || 
			    (col+c >= cols)) {
				continue;
			}
			else if (board[(row + r) * cols + (col + c)] != ' ') {
				live_neighbors++;
			}
		}
	}
	return live_neighbors;
}

int rule_one(char cell, int live_neighbors) {
	return cell != ' ' && live_neighbors < 2;
}

int rule_two(char cell, int live_neighbors) {
	return cell != ' ' && live_neighbors == 2 || live_neighbors == 3;
}

int rule_three(char cell, int live_neighbors) {
	return cell != ' ' && live_neighbors > 3;
}

int rule_four(char cell, int live_neighbors) {
	return cell == ' ' && live_neighbors == 3;
}

char* initialize() {
	char* board = (char*) malloc(rows * cols * sizeof(char));
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			board[i * cols + j] = ' ';
		}
	}
	board[1 * cols + 1] = '*';
	board[3 * cols + 0] = '*';
	board[4 * cols + 4] = '*';
	board[5 * cols + 5] = '*';
	board[5 * cols + 4] = '*';
	board[4 * cols + 3] = '*';
	board[3 * cols + 3] = '*';
	board[2 * cols + 2] = '*';

	board[9 * cols + 9] = '*';
	board[9 * cols + 8] = '*';
	board[9 * cols + 7] = '*';
	board[9 * cols + 6] = '*';
	board[9 * cols + 5] = '*';

	board[7 * cols + 8] = '*';
	board[7 * cols + 7] = '*';
	board[7 * cols + 3] = '*';
	board[7 * cols + 5] = '*';
	board[7 * cols + 4] = '*';

	board[8 * cols + 3] = '*';
	board[8 * cols + 1] = '*';
	board[8 * cols + 9] = '*';
	board[8 * cols + 11] = '*';
	board[8 * cols + 13] = '*';

	board[6 * cols + 9] = '*';
	board[6 * cols + 2] = '*';
	board[6 * cols + 0] = '*';
	board[6 * cols + 13] = '*';
	board[6 * cols + 11] = '*';

	return board;
}

void print_board(char* board) {
	for(int r = 0; r < rows; r++) {
		char row[cols];
		for (int c = 0; c < cols; c++) {
			row[c] = board[r * cols + c];
		}
		printf("%s\n", row);
	}
}

char* tick(char* board) {
	int sz = rows * cols * sizeof(char);
	char* next_generation = (char*) malloc(sz);
	memcpy(next_generation, board, sz);
	for (int row = 0; row < rows; row++) {
		for (int col = 0; col < cols; col++) {
			int offset = row * cols + col;
			int live_neighbors = get_live_neighbors(board, row, col);
			char cell = board[offset];
			if (rule_one(cell, live_neighbors) || rule_three(cell, live_neighbors)) { 
				next_generation[offset] = ' ';  
			} else if (rule_two(cell, live_neighbors) || rule_four(cell, live_neighbors)) {
				next_generation[offset] = '*';
			}
		}
	}
	free(board);
	return next_generation;
}


int main(int argc, char** argv) {
	char* board = initialize();
   	int generation = 0;
	while(1) {
		printf("Generation %d\n", generation);
		print_board(board);
		printf("\n");

		board = tick(board);
		generation++;
		usleep(0.1 * 1000000);
	}
}
