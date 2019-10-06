// Hide a warning regarding unused macro inspection.
#pragma ide diagnostic ignored "OCUnusedMacroInspection"

//------------------------------------
//	Define 
//------------------------------------
#define _CRT_SECURE_NO_WARNINGS

//------------------------------------
//	Include Libraries 
//------------------------------------
#include <time.h>
#include "mine_ship_game.h"
#include <string.h>

//------------------------------------
//	Hide other warnings
//------------------------------------
#pragma clang diagnostic push
#pragma clang diagnostic pop
#pragma clang diagnostic ignored "-Wunknown-pragmas"
#pragma ide diagnostic ignored "cert-msc32-c"
#pragma ide diagnostic ignored "cert-err34-c"

//------------------------------------
//	FUNCTION main 
//------------------------------------
int main(int argc, char* argv[]) {
	// 1. We set the seed for generating random numbers (in case the "Computer" acts as one of the players).
	srand((unsigned int) time(NULL));

	// 2. We parse the input arguments to the program.
	
	// 2.1. The first argument is the name of the first player.
	char* player1_name = (char*)malloc(strlen(argv[1]) + 1 * sizeof(char));
	strcpy(player1_name, argv[1]);

	// 2.2. The second argument is the name of the second player.
	char* player2_name = (char*)malloc(strlen(argv[2]) + 1 * sizeof(char));
	strcpy(player2_name, argv[2]);

	// 2.3. The third argument is the size of the board.
	int board_size = atoi(argv[3]);

	// 2.4. The fourth argument is the percentage of mines on the board.
	int percentage_mines = atoi(argv[4]);

	// 3. We call to the main function of the game.
	play_game(player1_name, player2_name, board_size, percentage_mines);

	// 4. We finish the application returning 0.
	return 0;
}
