//------------------------------------
//	Include Libraries 
//------------------------------------
#include "mine_ship_game.h"

//------------------------------------
//	Hide warnings
//------------------------------------
#pragma clang diagnostic push
#pragma clang diagnostic pop
#pragma ide diagnostic ignored "cert-msc30-c"

//------------------------------------
//	01. FUNCTION create_new_game
//------------------------------------
game* create_new_game(char* p1, char* p2, int board_size, int percentage_mines) {
	// 1. We declare the variable to be returned -> the struct game to be played.
	game* res = NULL;

	// 2. We create memory space for it with malloc.
	res = (game*)malloc(1 * sizeof(game));

	// 3. We set the mode based on the names of the players.
	/*
	Human vs. Human --> mode = 0
	Human vs. Computer --> mode = 1
	Computer vs. Human --> mode = 2
	Computer vs. Computer --> mode = 3
	*/
	int mode = 0;
	if (strcmp(p1, "Computer") == 0)
		mode = mode + 2;
	if (strcmp(p2, "Computer") == 0)
		mode = mode + 1;
	(*res).mode = mode;

	// 4. We set the status to be 1; (i.e., player1 moves first)
	(*res).status = 1;

	// 5. We initialise the board_size.
	(*res).board_size = board_size;

	// 6. We initialise num_mines.
	float num_mines = (board_size * board_size) * (percentage_mines / 100.0);
	(*res).num_mines = (int) num_mines;
	
	// 7. We initialise p1_name.
	(*res).p1_name = (char*)malloc((strlen(p1) + 1) * sizeof(char));
	strcpy((*res).p1_name, p1);

	// 8. We initialise p2_name.
	(*res).p2_name = (char*)malloc((strlen(p1) + 1) * sizeof(char));
	strcpy((*res).p2_name, p2);

	// 9. We initialise p1_mines_uncovered.
	(*res).p1_mines_uncovered = (*res).num_mines;

	// 10. We initialise p1_mines_uncovered.
	(*res).p2_mines_uncovered = (*res).num_mines;

	// 11. We initialise p1_board.

	// 11.1. We make room for the array pointers.
	(*res).p1_board = (board_cell**) malloc(board_size * sizeof(board_cell*));

	// 11.2. We make room for the matrix, and adjust the pointers.
	(*res).p1_board[0] = (board_cell*)malloc((board_size * board_size) * sizeof(board_cell));
	for (int i = 0; i < board_size; i++)
		(*res).p1_board[i] = (*res).p1_board[0] + (board_size * i);

	// 11.3. We initialise the cells.
	for (int i = 0; i < board_size; i++) {
		for (int j = 0; j < board_size; j++) {
			(*res).p1_board[i][j].value = 0;
			(*res).p1_board[i][j].visible = False;
		}
	}

	// 12. We initialise p2_board.

	// 12.1. We make room for the array pointers.
	(*res).p2_board = (board_cell**) malloc(board_size * sizeof(board_cell*));

	// 12.2. We make room for the matrix, and adjust the pointers.
	(*res).p2_board[0] = (board_cell*)malloc((board_size * board_size) * sizeof(board_cell));
	for (int i = 0; i < board_size; i++)
		(*res).p2_board[i] = (*res).p2_board[0] + (board_size * i);

	// 12.3. We initialise the cells.
	for (int i = 0; i < board_size; i++) {
		for (int j = 0; j < board_size; j++) {
			(*res).p2_board[i][j].value = 0;
			(*res).p2_board[i][j].visible = False;
		}
	}

	// 13. We return res.
	return res;
}

//--------------------------------------------------
// 02. gen_num
//--------------------------------------------------
int gen_num(int lb, int ub) {
	// 1. We create the variable to be returned -> the random value being generated.
	int res = 0;

	// 2. We assign num to a random value in the desired range.
	res = (rand() % (ub - lb)) + lb;

	// 3. We return the output variable.
	return res;
}

//--------------------------------------------------
// 03. FUNCTION my_getchar
//--------------------------------------------------
char my_get_char() {
	// 1. We create the variable to be returned -> the char typed on keyboard by the user.
	char res;

	// 2. We create an extra variable to consume any other characters entered until a return is pressed.
	boolean line_consumed = False;
	char dummy_char;

	// 3. We consume the first char entered; (i.e., the one we are interested in)
	res = getchar();

	// 4. While still there are remaining characters.
	while (line_consumed == False) {

		// 4.1. We consume the next character.
		dummy_char = getchar();

		// 4.2. If the new character is at the end of line one, we can ensure we have consumed the entire line.
		if (dummy_char == '\n') {
            line_consumed = True;
        }

	}

	// 5. We return res.
	return res;
}

//-------------------------------------
//	04. FUNCTION place_mines_on_board
//-------------------------------------
void place_mines_on_board(board_cell** board, int board_size, int num_mines) {
    // We initialise the counter to be equal to 0.
    int counter = 0;

    // 1. While there are more mines to be generated:
    while (counter < num_mines) {

		// 1.1. We randomly select a cell. (row and column)
		int row_number = gen_num(0, board_size);
		int column_number = gen_num(0, board_size);

		// 1.2. If the cell is not already occupied with a mine: (with value != 9)
		if ((*(&board[row_number][column_number])).value != 9) {

			// 1.2.1. We occupy it with a mine.
            (*(&board[row_number][column_number])).value = 9;

			// 1.2.2. We increase by 1 all its non-mine neighbours.

			/*
			 * We make sure that we are not in the top row or any other row
			 * which lies beyond the upper bound of the board.
			 */
			if (row_number - 1 >= 0) {
			    /*
			     * We make sure that we are not in the left-most column or any
			     * other column which lies beyond the left bound of the board.
			     * We also ensure that the left up cell does not contain a mine.
			     */
                // 1.2.2.1. Left Up neighbour.
                if (column_number - 1 >= 0 && (*(&board[row_number - 1][column_number - 1])).value != 9) {
                    // We increase the left up cell's value by 1.
                    (*(&board[row_number - 1][column_number - 1])).value++;
                }

                // 1.2.2.2. Up neighbour.
                // If the upper cell does not contain a mine:
                if ((*(&board[row_number - 1][column_number])).value != 9) {
                    // We increase the upper cell's value by 1.
                    (*(&board[row_number - 1][column_number])).value++;
                }

                /*
			     * We make sure that we are not in the right-most column or any
			     * other column which lies beyond the right bound of the board.
			     * We also ensure that the right up cell does not contain a mine.
			     */
                // 1.2.2.3. Right Up neighbour.
                if (column_number + 1 < board_size && (*(&board[row_number - 1][column_number + 1])).value != 9) {
                    // We increase the right up cell's value by 1.
                    (*(&board[row_number - 1][column_number + 1])).value++;
                }
			}

            /*
             * We make sure that we are not in the left-most column or any
             * other column which lies beyond the left bound of the board.
             * We also ensure that the cell on the left does not contain a mine.
             */
			// 1.2.2.4. Left neighbour.
			if (column_number - 1 >= 0 && (*(&board[row_number][column_number - 1])).value != 9) {
			    // We increase the left cell's value by 1.
                (*(&board[row_number][column_number - 1])).value++;
			}

            /*
             * We make sure that we are not in the right-most column or any
             * other column which lies beyond the right bound of the board.
             * We also ensure that the cell on the right does not contain a mine.
             */
			// 1.2.2.5. Right neighbour.
            if (column_number + 1 < board_size && (*(&board[row_number][column_number + 1])).value != 9) {
                // We increase the right cell's value by 1.
                (*(&board[row_number][column_number + 1])).value++;
            }

            /*
			 * We make sure that we are not in the bottom row or any other row
			 * which lies beyond the lower bound of the board.
			 */
            if (row_number + 1 < board_size) {
                /*
                 * We make sure that we are not in the left-most column or any
                 * other column which lies beyond the left bound of the board.
                 * We also ensure that the left bottom cell does not contain a mine.
                 */
                // 1.2.2.6. Left Bottom neighbour.
                if (column_number - 1 >= 0 && (*(&board[row_number + 1][column_number - 1])).value != 9) {
                    // We increase the left bottom cell's value by 1.
                    (*(&board[row_number + 1][column_number - 1])).value++;
                }

                // 1.2.2.7. Bottom neighbour.
                // If the lower cell does not contain a mine:
                if ((*(&board[row_number + 1][column_number])).value != 9) {
                    // We increase the lower cell's value by 1.
                    (*(&board[row_number + 1][column_number])).value++;
                }

                /*
                 * We make sure that we are not in the right-most column or any
                 * other column which lies beyond the right bound of the board.
                 * We also ensure that the right bottom cell does not contain a mine.
                 */
                // 1.2.2.8. Right Bottom neighbour.
                if (column_number + 1 < board_size && (*(&board[row_number + 1][column_number + 1])).value != 9) {
                    // We increase the right bottom cell's value by 1.
                    (*(&board[row_number + 1][column_number + 1])).value++;
                }
            }

			// 1.2.3. We decrease num_mines.
			num_mines--;
		}
	}
}

//----------------------------------------
//	05. FUNCTION display_board_content
//----------------------------------------
void display_board_content(board_cell** board, int board_size, int mines_uncovered) {
	// 1. We print the flag of board content.
    printf("\t------------------\n \t BOARD CONTENT \n \t------------------\n\n");

	// 2. We print the column names, using the value 'A' as a reference.

	// 2.1. We print a row of lines.
	// We print a tab whitespace character.
	printf("\t");
	// We initialise the column's index to be equal to 0.
    int column_index_1 = 0;
    // While column index is lesser than or equal to the board's size: (its width)
    while (column_index_1 <= board_size) {
        // If the column index reaches the edge of the board:
        if (column_index_1 == board_size) {
            // We print the closing vertical bar.
            printf("|");
        }
        // Otherwise, we do the following:
        else {
            // We print the following repeating pattern.
            printf("|---");
        }
        // We increase the column's index by 1 with each iteration.
        column_index_1++;
    }

	// 2.2. We print a row with the column names.
	// We declare letter A as a reference.
    char letter = 'A';
    // We print the opening vertical bar.
    printf("\n\t|");
    // In the for loop we are going through the letters indicating columns of the board.
    for (int letter_index = 0; letter_index < board_size; letter_index++) {
        // We print each single letter for all the columns.
        printf(" %c |", letter);
        // We go through the alphabet incrementing letter by 1 with each iteration.
        letter++;
    }

	// 2.3. We print a row of lines.
	// We print a newline character with a tab.
    printf("\n\t");
    // In the for loop we are going through the columns until we reach the board's edge.
    for (int column_index_2 = 0; column_index_2 <= board_size; column_index_2++) {
        if (column_index_2 == board_size) {
            printf("|");
        }
        else {
            printf("|---");
        }
    }

	// 3. We print the rows of the board, using 1 as a reference.
    int row_number = 1;

	// 3.1. We traverse each row.
    for (int row = 0; row < board_size; row++) {

        // 3.1.1. We print the row_number.
        printf("\n%d\t|", row_number);

        // 3.1.2. We traverse each column.
        for (int column = 0; column < board_size; column++) {

            // 3.1.2.1. If visible, we print each cell content.
            if ((*(&board[row][column])).visible == True) {
                // If a particular cell contains a mine:
                if ((*(&board[row][column])).value == 9) {
                    // We print '*' for this position.
                    printf(" * |");
                }
                else {
                    // We print the board_cell value (number of surrounding mine neighbours) for a position with no mine.
                    printf(" %d |", (*(&board[row][column])).value);
                }

            }

            // 3.1.2.2. If non-visible, we print ' '.
            else {
                printf("   |");
            }
        }

        // 3.2.2. We print a row of lines.
        printf("\n\t");
        int column_index_3 = 0;
        while (column_index_3 <= board_size) {
            if (column_index_3 == board_size) {
                printf("|");
            }
            else {
                printf("|---");
            }

            column_index_3++;
        }

        // 3.2.3. We increase the row number.
        row_number++;
    }

    // We print a newline character.
    printf("\n");

	// 4. We print the number of mines uncovered.
    printf("\nMines Uncovered = %d\n", mines_uncovered);
}

//---------------------------------------
//	06. FUNCTION user_get_movement_index
//---------------------------------------
int user_get_movement_index(int status,
							int board_size,
							board_cell** p1_board,
							board_cell** p2_board) {
	// 1. We create the variable to be returned -> position on the board the user wants to uncover.
	int res = -1;

	// 2. While we have not received a valid position:
    while (res == -1) {

        // 2.1. We print a message asking for a new movement.
        printf("\nPlease do your next movement by uncovering one of the positions on the board.");

        // 2.2. We collect them from the keyboard.
        printf("\nEnter a row number: ");
        char row_number = my_get_char();
        printf("\nEnter a column letter: ");
        char column_letter = my_get_char();

        /*
         * I subtract the given characters from row number and column letter
         * given by the user in order to get the correct position of a cell
         * being uncovered.
         */
        int row_index = row_number - '1';
        int column_index = column_letter - 'A';

        // 2.3. If the position is not valid, we inform the user.
        if ((row_index < 0 || row_index > board_size) || (column_index < 0 || column_index > board_size)) {
            printf("\nThe indicated position does not exist, please try again.\n");
        }

        // 2.4. If the position is valid.
        else {
            // If player one moves next:
            if (status == 1) {

                // 2.4.1. If the position is occupied (visible == True), we inform the user.
                if ((*(&p2_board[row_index][column_index])).visible == True) {
                    printf("The indicated position is already occupied, please try again.\n");
                }

                // 2.4.2. If the position was not occupied (visible == False), we return the position board[row][column] in integer format.
                else {
                    // The variable 'res' takes the index value of a cell which we would count from the beginning of the board.
                    res = (row_index * board_size) + column_index;
                }
            }
            // If player two moves next:
            else if (status == 2) {
                if ((*(&p1_board[row_index][column_index])).visible == True) {
                    printf("The indicated position is already occupied, please try again.\n");
                }
                else {
                    res = (row_index * board_size) + column_index;
                }
            }
        }
    }

	// 3. We return res.
	return res;
}

//-------------------------------------------
//	07. FUNCTION computer_get_movement_index
//-------------------------------------------
int computer_get_movement_index(int status,
								int board_size,
								board_cell** p1_board,
								board_cell** p2_board) {
	// 1. We create the variable to be returned -> position in the board the computer wants to uncover.
	int res = -1;

	// 2. While we have not received a valid position:
    while (res == -1) {

        // 2.1. We generate row and column randomly.
        int row_index = gen_num(0, board_size);
        int column_index = gen_num(0, board_size);

        // 2.2. If the position was not occupied (visible == False), we return the position board[row][column] in integer format.
        // If computer 1 moves next:
        if (status == 1) {
            if ((*(&p2_board[row_index][column_index])).visible == False) {
                res = (row_index * board_size) + column_index;
            }
        }
        // If computer 2 moves next:
        else if (status == 2) {
            if ((*(&p1_board[row_index][column_index])).visible == False) {
                res = (row_index * board_size) + column_index;
            }
        }
    }

	// 3. We return res.
	return res;
}

//---------------------------------------
//	08. FUNCTION get_next_movement_index
//---------------------------------------
int get_next_movement_index(int status,
							char* p1_name,
							char* p2_name,
							int board_size,
							board_cell** p1_board,
							board_cell** p2_board) {
	// 1. We create the variable to be returned -> position in the board the user or the computer wants to uncover.
	int res = -1;

	// 2. Depending on status, we decide who moves next.

	// 2.1. If status is 1, then we check p1_name to determine whether the player must move as a user or as a computer.
    if (status == 1) {
        // We display who does the next movement as the player 1.
        printf("%s has to do the next movement.\n", p1_name);
        // If player 1 is not a computer:
        if (strcmp(p1_name, "Computer") != 0) {

            // 2.1.1. One way or another, we collect the movement made by the user.
            res = user_get_movement_index(status, board_size, p1_board, p2_board);
        }
        // Otherwise if player 1 is a computer:
        else {
            // We collect the movement made by the computer.
            res = computer_get_movement_index(status, board_size, p1_board, p2_board);
        }
    }

	// 2.2. If status is 2, then we check p2_name to determine whether the player must move as a user or as a computer.
    else if (status == 2) {
        // We display who does the next movement as the player 2.
        printf("%s has to do the next movement.\n", p2_name);
        // If player 2 is not a computer:
        if (strcmp(p2_name, "Computer") != 0) {

            // 2.2.1. One way or another, we collect the movement made by the user.
            res = user_get_movement_index(status, board_size, p1_board, p2_board);
        }
        // Otherwise if player 2 is a computer:
        else {
            res = computer_get_movement_index(status, board_size, p1_board, p2_board);
        }
    }

	// 3. We return res.
	return res;
}

//------------------------------------
//	09. FUNCTION process_movement
//------------------------------------
void process_movement(int position_to_uncover,
		              int* status,
		              int board_size,
		              board_cell** p1_board,
		              board_cell** p2_board,
		              int* p1_mines_uncovered,
		              int* p2_mines_uncovered) {

	// 1. We get the concrete row and column from position_to_uncover.
    int row_index = position_to_uncover / board_size;
    int column_index = position_to_uncover % board_size;

	// 2. Depending on the status, we uncover the position in one player or another.

	// 2.1. If status == 1 (player 1 moves next):
    if (*status == 1) {

        // 2.1.1. We uncover the position in the p2_board.
        (*(&p2_board[row_index][column_index])).visible = True;

        // 2.1.2. If the position was a mine, we decrease the number of mines to hit.
        if ((*(&p2_board[row_index][column_index])).value == 9) {
            *p1_mines_uncovered -= 1;
        }

        // 2.1.3. If the number of p1_mines_uncovered is 0, p1_name has won the game and we update status accordingly.
        if (*p1_mines_uncovered == 0) {
            *status = 3;
        }
    }

	// 2.2. If status == 2 (player 2 moves next):
    else if (*status == 2) {

        // 2.2.1. We uncover the position in the p1_board.
        (*(&p1_board[row_index][column_index])).visible = True;

        // 2.1.2. If the position was a mine, we decrease the number of mines to hit.
        if ((*(&p1_board[row_index][column_index])).value == 9) {
            *p2_mines_uncovered -= 1;
        }

        // 2.2.3. If the number of p2_mines_uncovered is 0, p2_name has won the game and we update status accordingly.
        if (*p2_mines_uncovered == 0) {
            *status = 4;
        }
    }
}

//------------------------------------
//	10. FUNCTION play_game
//------------------------------------
void play_game(char* p1, char* p2, int board_size, int percentage_mines) {
	// 1. We create a variable pointer game* g and call to the function.
	// create_new_game so as to initialise it.
	game* g = create_new_game(p1, p2, board_size, percentage_mines);

	// 2. We initialise the mines in p1_board and p2_board.
	place_mines_on_board((*g).p1_board, (*g).board_size, (*g).num_mines);
	place_mines_on_board((*g).p2_board, (*g).board_size, (*g).num_mines);

	// 3. Game loop: while the game is on (i.e., Game Status = 1 or Game Status = 2)
	while ((*g).status < 3) {

		// 3.1. We display_board_content before the move.
		if ((*g).status == 1) {
			printf("\nGame is on. Player %s moves next.\n\n", (*g).p1_name);
			display_board_content((*g).p2_board, (*g).board_size, (*g).p1_mines_uncovered);
		}
		else {
			printf("\nGame is on. Player %s moves next.\n\n", (*g).p2_name);
			display_board_content((*g).p1_board, (*g).board_size, (*g).p2_mines_uncovered);
		}

		// 3.2. We get and process the next movement.
		int mov = get_next_movement_index((*g).status, (*g).p1_name, (*g).p2_name, (*g).board_size, (*g).p1_board, (*g).p2_board);
		process_movement(mov, &(*g).status, (*g).board_size, (*g).p1_board, (*g).p2_board, &(*g).p1_mines_uncovered, &(*g).p2_mines_uncovered);

		// 3.3. We display_board_content after the move.
		if (((*g).status == 1) || ((*g).status == 3)) {
			printf("\nGame is on. Player %s had just moved.\n\n", (*g).p1_name);
			display_board_content((*g).p2_board, (*g).board_size, (*g).p1_mines_uncovered);
			printf("Press a key to continue\n");
			my_get_char();
		}
		else {
			printf("\nGame is on. Player %s had just moved.\n\n", (*g).p2_name);
			display_board_content((*g).p1_board, (*g).board_size, (*g).p2_mines_uncovered);
			printf("Press a key to continue\n");
			my_get_char();
		}

		// 3.4. If status is < 3, we swap players.
		if ((*g).status < 3) {
            if ((*g).status == 1) {
                (*g).status = 2;
            }
            else {
                (*g).status = 1;
            }
        }
	}

	// 4. We display the end of the game.
	if ((*g).status == 3) {
		printf("\n\n\n------------------------------\nPLAYER %s HAS WON!\n------------------------------\n", (*g).p1_name);
		display_board_content((*g).p2_board, (*g).board_size, (*g).p1_mines_uncovered);
	}
	else {
		printf("\n\n\n------------------------------\nPLAYER %s HAS WON!\n------------------------------\n", (*g).p2_name);
		display_board_content((*g).p1_board, (*g).board_size, (*g).p2_mines_uncovered);
	}
}
