//--------------------------------------------------
// INCLUDE LIBRARIES
//--------------------------------------------------
#include "word_game.h"
#include "string.h"

//--------------------------------------------------
// my_getchar
//--------------------------------------------------
char my_get_char() {
    // 1. We create the output variable with the value the user has input by keyboard.
    char input_character = (char) getchar();

    // 2. We discard any extra character the user has input by keyboard.
    boolean character_received = False;
    char dummy_character;
    while (character_received == False) {
        dummy_character = (char) getchar();
        if (dummy_character == '\n') {
            character_received = True;
        }
    }

    // 3. We return the output variable.
    return input_character;
}

//--------------------------------------------------
// ask_for_new_command
//--------------------------------------------------
char ask_for_new_command() {
    // 1. We create the output variable with an initial value.
    char input_command = ' ';

    // 2. While we have not received a valid command.
    boolean valid_command = False;
    while (valid_command == False) {
        // 2.1 We ask the user for a new command.
        printf("Please type in a valid command here >>>");
        // 2.2. We read the command entered by the user.
        input_command = my_get_char();
        // 2.3. We check if the command is correct.
        if ((input_command >= 'a' && input_command <= 'z') || (input_command == '>' || input_command == '<')) {
            valid_command = True;
        }
    }

    // 3. We return the output variable.
    return input_command;
}

//--------------------------------------------------
// process_movement
//--------------------------------------------------
void process_movement(char current_word[NUM_LETTERS], char** letter_ptr_ptr, int* num_movs_ptr, char command) {
    // 1. We process the different commands.

    // 1.1. If it is a '<':
    if (command == '<') {
        // 1.1.1. If we are not in the left border.
        if (*letter_ptr_ptr > &current_word[0]) {
            // I. We move the pointer to the left.
            *letter_ptr_ptr -= 1;
            // II. We update the number of movements.
            *num_movs_ptr += 1;
        }
    }

    // 1.2. If it is a '>':
    else if (command == '>') {
        // 1.2.1. If we are not in the right border.
        if (*letter_ptr_ptr < &current_word[NUM_LETTERS - 1]) {
            // I. We move the pointer to the right.
            *letter_ptr_ptr += 1;
            // II. We update the number of movements.
            *num_movs_ptr += 1;
        }
    }

    // 1.3. If it is a letter:
    else if (command >= 'a' && command <= 'z') {
        // 1.3.1. If the letter is different from the current one.
        if (command != **letter_ptr_ptr) {
            // I. We modify the letter.
            **letter_ptr_ptr = command;
            // II. We update the number of movements.
            *num_movs_ptr += 1;
        }
    }
}

//--------------------------------------------------
// print_status
//--------------------------------------------------
void print_status(char current_word[NUM_LETTERS], char target_word[NUM_LETTERS], char* letter_ptr, int num_movs) {
    // 1. We clean the screen.
    printf("\n\n\n");

    // 2. We print the status.
    printf("- - - - - GAME STATUS - - - - -");

    // 3. We print the target word.
    int target_word_index = 0;
    int target_word_size = 4;
    printf("\nTarget = ");
    while (target_word_index < target_word_size) {
        printf("%c", target_word[target_word_index]);
        target_word_index++;
    }

    printf("\n\n");

    // 4. We print the current word.
    int current_word_index = 0;
    int current_word_size = 4;
    while (current_word_index < current_word_size) {
        printf("%c", current_word[current_word_index]);
        current_word_index++;
    }

    // 5. We print the caret associated with the pointer.
    int spaces_number = (int) (letter_ptr - &current_word[0]);
    printf("\n");
    for (int space_index = 0; space_index < spaces_number; space_index++) {
        printf(" ");
    }
    printf("^");

    // 6. We print the number_of_movements.
    printf("\nNumber of movements: %d \n", num_movs);

    // 7. We print a last line.
    printf("- - - - - - - - - - - - - - - - \n");
}

//--------------------------------------------------
// is_game_over
//--------------------------------------------------
int is_game_over(char current_word[NUM_LETTERS], char target_word[NUM_LETTERS], int num_movs, int max_movs) {
    // 1. We create the output variable with an initial value.
    int game_status = 0;

    // 2. Game is over if current_word matches target_word.
    // 2.1. By default we assume current_word and target_word are equal.
    if (strcmp(current_word, target_word) == 0) {
        // 2.2. In case they are equal we set the end of the game.
        game_status = 1;
    }

    if (num_movs < max_movs) {
        // 2.3. We traverse them to check if they are actually equal.
        int letter_index = 0;
        int word_size = 4;
        while (letter_index < word_size) {
            if (current_word[letter_index] != target_word[letter_index]) {
                game_status = 0;
                break;
            }

            letter_index++;
        }
    }

    // 3. Game is over as we have done the maximum number of movements being allowed.
    if (num_movs == max_movs) {
        game_status = -1;
    }

    // 4. We return the output variable.
    return game_status;
}

//--------------------------------------------------
// user_game_word
//--------------------------------------------------
void user_game_word(char current_word[NUM_LETTERS], char target_word[NUM_LETTERS]) {
    // 1. We initialise the pointer to the first position of the array.
    char* letter_pointer = &current_word[0];
    char** letter_pointer_pointer = &letter_pointer;

    // 2. We initialise the num_movs and max_movs.
    int num_movs = 0;
    int* num_movs_pointer = &num_movs;
    int max_movs = NUM_LETTERS * 2;

    // 3. We initialise the game to not to be over.
    int game_over = is_game_over(current_word, target_word, num_movs, max_movs);

    // 4. We print the initial status of the game.
    print_status(current_word, target_word, letter_pointer, num_movs);

    // 5. We perform the main loop.
    while (game_over == 0) {
        // 5.1. We ask for a new command.
        printf("NEW MOVEMENT: Enter a valid command by keyword: \nValid commands: >\t<\t'a'--'z' \n");
        char user_command = ask_for_new_command();

        // 5.2. We process the command.
        process_movement(current_word, letter_pointer_pointer, num_movs_pointer, user_command);

        // 5.3. We print the current status.
        print_status(current_word, target_word, letter_pointer, num_movs);

        // 5.4. We determine if the game is over.
        game_over = is_game_over(current_word, target_word, num_movs, max_movs);
    }

    // 6. Once the game is finished, we display the outcome of it.
    if (game_over == 1) {
        printf("\nCongratulations: you reached the target word after %d movements.", num_movs);
    }
    else {
        printf("\nGame Over: you did not reach the target word after the maximum number of movements, try again.");
    }
}
