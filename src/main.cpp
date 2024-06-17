#include <stdio.h>
#include <string>
#include <ncurses.h>
#include "board.h"

int main()
{
    // init screen and sets up screen
    initscr();

    board board_instance;
    player player1("O", "1");
    player player2("X", "2");
    
    bool is_player1_turn = true;
    bool win = false;
    std::string player_choice;


    while(true)
    {
        // display_turn_board
        board_instance.display_turns();

        // Decide if player1 or player 2 turn
        player& player_to_move = is_player1_turn ? player1 : player2;
        player_choice = board_instance.get_player_choice(player_to_move, is_player1_turn);

        if(player_choice == "quit" || player_choice == "q")
        {
            break;
        }

        board_instance.update_board(player_to_move, player_choice, is_player1_turn);
        player1.get_elem_count();

        win = board_instance.check_win();

        if(win)
        {
            board_instance.apply_win(player_to_move);
            break;
        }

        // subtract turn_board on a a basis
    }

    // deallocate our objects we instantiated
    // deallocates memory and ends ncurses
    endwin();
}