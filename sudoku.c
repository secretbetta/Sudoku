#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/**
 * A function that creates an empty puzzle
 */
void CreateSudokuPuzzle(int matrix[][9], int row, int col) {
	int r, c;
	for (r = 0; r < row; r++) {
		for (c = 0; c < col; c++) {
			matrix[r][c] = 0;
		}
	}
}

/**
 * Displays the puzzle that can be printed
 */
void Display(int matrix[][9], int row, int col) {
	int r, c, x;

	//Column Number
	printf(" C:");
	for (x = 0; x < 9; x++) {
		printf("%d  ", x);
		if (x > 0 && (x+1)%3 == 0) {
			printf(" ");
		}
	}
	printf("\n");

	for (r = 0; r < row; r++) {
		if (r == 0) { // Seperator for each section
			printf("R+");
			for (x = 0; x < 9*3+2; x++) {
				printf("-");
			}
			printf("+");
			printf("\n");
		}

		// Row number, box with numbers, and vertical seperators
		printf("%d|", r);
		for (c = 0; c < col; c++) {
			if (matrix[r][c] != 0) {
				printf("[%d]", matrix[r][c]);
			} else {
				printf("[ ]");
			}
			if ((c+1)%3 == 0) {
				printf("|");
			}
		}
		printf("\n");

		// Horizontal Seperators
		if (r > 0 && (r+1)%3 == 0) {
			printf(" +");
			for (x = 0; x < 9*3+2; x++) {
				printf("-");
			}
			printf("+");
			printf("\n");
		}
	}

	printf("\n");
}

/**
 * Sets the given square to the given value as an initial value 
 * that cannot be changed by the puzzle solver
 */
void addInitial(int matrix[][9], int row, int col, int value) {
	matrix[row][col] = value;
}

/**
 * Sets the given square to the given value; 
 * the value can be changed later by another call to addGuess
 */
void addGuess(int matrix[][9], int row, int col, int value) {
	matrix[row][col] = value;
}

/**
 * Returns true if the values in the puzzle do not violate 
 * the restrictions
 */
int checkPuzzle(int matrix[][9]) {
	int valid = 1;
	int c, r, x, boxc, boxr;
	int bag[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};

	// Columns check
	for (r = 0; r < 9; r++) {
		for (c = 0; c < 9; c++) {
			if (bag[matrix[r][c] - 1] != 1) {
				bag[matrix[r][c] - 1] = 1;
			} else if (matrix[r][c] != 0) {
				return 0;
			}
		}
		for (x = 0; x < 9; x++) {
			bag[x] = 0;
		}
	}

	// Rows check
	for (c = 0; c < 9; c++) {
		for (r = 0; r < 9; r++) {
			if (bag[matrix[r][c] - 1] != 1) {
				bag[matrix[r][c] - 1] = 1;
			} else if (matrix[r][c] != 0) {
				return 0;
			}
		}
		for (x = 0; x < 9; x++) {
			bag[x] = 0;
		}
	}

	// Boxes check
	for (boxr = 0; boxr < 9; boxr+=3) {
		for (boxc = 0; boxc < 9; boxc+=3) {
			for (r = boxr; r < boxr + 3; r++) {
				for (c = boxc; c < boxc + 3; c++) {
					if (bag[matrix[r][c] - 1] == 0) {
						bag[matrix[r][c] - 1] = 1;
					} else if (matrix[r][c] != 0) {
						return 0;
					}
				}
			}
			for (x = 0; x < 9; x++) {
					bag[x] = 0;
			}
		}
	}
	return 1;
}

/**
 * Returns the value in the given square
 */
int getValueIn(int matrix[][9], int row, int col) {
	return matrix[row][col];
}

/**
 * Returns true if every square has a value
 */
int isFull(int matrix[][9], int row, int col) {
	int r, c;
	for (r = 0; r < row; r++) {
		for (c = 0; c < col; c++) {
			if (matrix[r][c] == 0)
				return 0;
		}
	}
	return 1;
}

/**
 * Changes all of the nonpermanent squares back to blanks (0)
 */
void reset(int matrix[][9], int start[][9]) {
	int r, c;
	for (r = 0; r < 9; r++) {
		for (c = 0; c < 9; c++) {
			if (start[r][c] == 0) {
				matrix[r][c] = 0;
			}
		}
	}

}


/**
 * Returns a one-dimensional array of nine booleans, each of which 
 * corresponds to a digit and is true if the digit can be placed in 
 * the given square without violating the restrictions
 */
int * getAllowedValues(int matrix[][9], int row, int col) {
	int r, c, x;
	int *isAvail = (int *) malloc(sizeof(int) * 9);
	
	if (matrix[row][col] == 0) {
		for (x = 0; x < 9; x++) {
			isAvail[x] = 0;
		}

		for (x = 0; x < 9; x++) {
			if (isAvail[matrix[row][x] - 1] == 0 && matrix[row][x] > 0) {
				isAvail[matrix[row][x] - 1] = 1;
			}
			if (isAvail[matrix[x][col] - 1] == 0 && matrix[x][col] > 0) {
				isAvail[matrix[x][col] - 1] = 1;
			}
		}

		for (r = row/3 * 3; r < row/3 * 3 + 3; r++) {
			for (c = col/3 * 3; c < col/3 * 3 + 3; c++) {
				if (isAvail[matrix[r][c] - 1] == 0 && matrix[r][c] > 0) {
					isAvail[matrix[r][c] - 1] = 1;
				}
			}
		}
	} else {
		for (x = 0; x < 9; x++) {
			isAvail[x] = 1;
		}
	}
	return isAvail;
}

// Extra Credit: Create Full Sudoku Game
/**
 * Fills diagonal boxes with a bag of random numbers 1-9
 */
void Diagonal(int board[][9], int start[][9], int row, int col) {
	int r, c, x, y, index1, index2, temp;
	int bag[9] = {1,2,3,4,5,6,7,8,9};

	for (x = 0; x < 9; x+=3) {
		for (y = 0; y < 11; y++) {
			index1 = rand()%9;
			index2 = rand()%9;
			temp = bag[index1];
			bag[index1] = bag[index2];
			bag[index2] = temp;
		}
		for (r = 0; r < 3; r++) {
			for (c = 0; c < 3; c++) {
				board[r+x][c+x] = bag[r*3+c]; 
				start[r+x][c+x] = 1;
			}
		}
		temp = 0;
	}
}

/**
 * Find index in box section with least available values
 */ 
void FindMinAllowed(int board[][9], int coordmin[], int row, int col) {
	int *available;
	int x, r, c;
	int count;
	int min = 9;

	for (r = 0; r < 3; r++) {
		for (c = 0; c < 3; c++) {
			available = getAllowedValues(board, row+r, col+c);
			count = 0;
			for (x = 0; x < 9; x++) {
				if (available[x] == 0) {
					count++;
				}
			}
			if (count < min && count > 0) {
				min = count;
				coordmin[0] = r+row;
				coordmin[1] = c+col;
			}
		}
	}
}

/**
 * Fills boxes with "safe" values
 */
void Box(int board[][9], int start[][9], int row, int col) {
	int bag[9] = {1,2,3,4,5,6,7,8,9};
	int pick[9] = {0,0,0,0,0,0,0,0,0};

	int x, index, n;
	int coordmin[2];
	int *available;

	for (index = 0; index < 9; index++) {
		FindMinAllowed(board, coordmin, row, col);
		available = getAllowedValues(board, coordmin[0], coordmin[1]);

		n = 0;
		for (x = 0; x < 9; x++) {
			if (available[x] == 0) {
				pick[n] = x+1;
				n++;
			}
		}

		if (n >= 1) {
			board[coordmin[0]][coordmin[1]] = pick[rand()%n];
		}
	}
}

/**
 * Algorithm to generate a filled Sudoku Board and then removes random boxes to create a puzzle
 * It resets itself until a valid sudoku puzzle is generated.
 * 
 * See Diagonal() and Box()
 */
void generator(int board[][9], int start[][9], int row, int col, int difficulty) {
	CreateSudokuPuzzle(board, row, col);
	CreateSudokuPuzzle(start, row, col);

	// Fill Diagonal Boxes
	Diagonal(board, start, row, col);

	// Fill remaining boxes
	int r, c;
	int invalid = 1;

	while (invalid) {
		for (r = 0; r < 9; r+=3) {
			for (c = 0; c < 9; c+=3) {
				if (board[r][c] == 0) { 
					Box(board, start, r, c);
				}
			}
		}
		if (isFull(board, row, col)) {
			invalid = 0;
		} else {
			reset(board, start);
		}
	}

	// Remove random boxes
	for (r = 0; r < 9; r++) {
		for (c = 0; c < 9; c++) {
			if (rand()%100 < difficulty) {
				board[r][c] = 0;
				start[r][c] = 0;
			} else {
				start[r][c] = 1;
			}
		}
	}
}

/** 
 * Creates Random Puzzle for user to play
 * 
 * Has a win condition if creative mode isn't on (Board must be valid & board must be full)
 * 8/9 Options:
 * 		Add initial value (only in creative mode)
 * 		Add a guess
 * 		Look at box
 * 		See all allowed values in box
 * 		Check if board is filled
 * 		Display Board
 * 		Resets Board to Starting values
 * 		Checks if board is valid
 *		Exits game
 */
void CreateRandomPuzzle(int matrix[][9], int start[][9], int row, int col, int difficulty, int creative) {
	int *available;
	int r, c, x;
	int input = 0;
	int val;

	CreateSudokuPuzzle(matrix, row, col);
	CreateSudokuPuzzle(start, row, col);

	if (creative == 0) {
		printf("Difficulty: %d\n", difficulty);
		generator(matrix, start, row, col, difficulty);
	}

	while (input != 8) {
		if (isFull(matrix, row, col) && checkPuzzle(matrix) == 1 && creative == 0) {
			printf("Congratulations! You finished the puzzle.\n\n");
			Display(matrix, row, col);
			printf("\n");
			break;
		}

		if (creative == 0) {
			printf("What would you like to do? (AddGuess: 1, GetValueInBox: 2, GetAllowedValues: 3, IsFull: 4, Display: 5, Reset: 6, Check Valid Board: 7, Exit: 8)\n");
		} else {
			printf("What would you like to do? (AddInitial: 0, AddGuess: 1, GetValueInBox: 2, GetAllowedValues: 3, IsFull: 4, Display: 5, Reset: 6, Check Puzzle if Valid: 7, Exit: 8)\n");
		}

		scanf("%d", &input);
		printf("\n");

		if (creative == 1 && input == 0) {
			printf("Where would you like to add the initial value?\n");
			printf("Row: ");
			scanf("%d", &r);
			printf("Column: ");
			scanf("%d", &c);
			if (r >= 9 || r < 0 || c >= 9 || c < 0) {
				printf("Cannot add value there.\n");
			} else {
				printf("Value: ");
				scanf("%d", &val);
				if (val < 0 || val >= 10) {
					printf("Invalid Value\n");
				} else {
					start[r][c] = 1;
					matrix[r][c] = val;
				}
			}
			printf("\n");
		}

		switch(input) {
			case 1:
				printf("Where would you like to add the guess?\n");
				printf("Row: ");
				scanf("%d", &r);
				printf("Column: ");
				scanf("%d", &c);
				if (r >= 9 || r < 0 || c >= 9 || c < 0 ||start[r][c] == 1) {
					printf("Cannot add guess there.\n");
				} else {
					printf("Guess: ");
					scanf("%d", &val);
					if (val < 0 || val >= 10) {
						printf("Invalid Guess\n");
					} else {
						matrix[r][c] = val;
					}
				}
				printf("\n");
				break;
			case 2:
				printf("What box would you like to see?\n");
				printf("Row: ");
				scanf("%d", &r);
				printf("Column: ");
				scanf("%d", &c);
				if (r >= 9 || r < 0 || c >= 9 || c < 0) {
					printf("Cannot look there.\n");
				} else {
					printf("Value: %d\n", getValueIn(matrix, r, c));
				}
				printf("\n");
				break;
			case 3:
				printf("Where would you like to see allowed values?\n");
				printf("Row: ");
				scanf("%d", &r);
				printf("Column: ");
				scanf("%d", &c);
				if (r >= 9 || r < 0 || c >= 9 || c < 0) {
					printf("Cannot look there.\n");
				} else {
					available = getAllowedValues(matrix, r, c);
					printf("Allowed Values: ");
					for (x = 0; x < 9; x++) {
						if (available[x] == 0) {
							printf("%d ", x+1);
						}
					}
					printf("\n");
				}
				printf("\n");
				break;
			case 4:
				if (isFull(matrix, row, col) == 1) {
					printf("Is full.\n");
				} else {
					printf("Is not full.\n");
				}
				printf("\n");
				break;
			case 5:
				Display(matrix, row, col);
				printf("\n");
				break;
			case 6:
				reset(matrix, start);
				printf("\n");
				break;
			case 7:
				if (checkPuzzle(matrix) == 1) {
					printf("Board is Valid.\n");
				} else {
					printf("Board is Invalid.\n");
				}
				printf("\n");
				break;
		}
	}
}

/**
 * Runner Method
 */
int main() {
	int input = 0;
	int difficulty;
	int row = 9;
	int col = 9;
	int board[row][col];
	int start[row][col];

	srand(time(0));

	while (input < 3) {
		printf("--Main Menu--\n");
		printf("What would you like to do?\n");
		printf("1. Start from scratch\n");
		printf("2. Play a random puzzle\n");
		printf("3. Exit\n");

		scanf("%d", &input);
		printf("\n");

		switch(input) {
			case 1:
				CreateRandomPuzzle(board, start, row, col, difficulty, 1);
				break;
			case 2:
				printf("What difficulty? (33 minimum = easy, 65 max = hard)\n");
				scanf("%d", &difficulty);
				printf("\n");

				if (difficulty < 33) {
					difficulty = 33;
				} else if (difficulty > 65) {
					difficulty = 65;
				}

				CreateRandomPuzzle(board, start, row, col, difficulty, 0);
				break;

			reset(board, start);
			printf("\n");
		}
	}
}