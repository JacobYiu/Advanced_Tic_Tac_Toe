#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <map>
#include <string>
#include "player.h"

#define RED 1
#define GREEN 2
#define BLUE 3


class board{
    public:
        void create_board(int origin_y, int origin_x);
        void create_turn_board(int origin_y, int origin_x);
        void display_turns();
        void init_font_color();
        // void create_players();
        void create_pieces(player cur_player);
        void display_pieces(std::string piece, std::map<std::string, int> element_count);
        void create_instructions();
        
        std::string get_element_type();
        std::pair<int,int> get_element_w_pos(std::string element_type);
        int get_element_assoc(std::string element_type);
        std::string get_element(int element_assoc);
        std::pair<int,int> get_strpos_pair(std::string string_pos);
        char get_char(const std::string& pos) const;
        std::pair<char,int> get_char_and_color(std::string mapped_pos);

        int get_turnsLeft(std::string string_pos);
        void update_turnsLeft(std::string string_pos, int turns, bool is_positive);
        void global_turns_decrement();

        std::string get_player_choice(player &player_to_move, bool is_player1_turn);

        void display_options();
        bool check_choice(int y_pos, player player_to_move, std::string choice, bool is_player1_turn);
        void gen_error(int y_pos, int x_pos, std::string err_msg);

        void convert_arguments(std::vector<std::string> &argument_list);

        bool check_type_adv(int y_pos, std::string user_char, std::string user_type, std::string board_char, std::string board_type);

        std::string start_turn(int y_origin, player player, bool is_player1_turn);

        void update_board(player &player_to_move, std::string player_choice, bool &is_player1_turn);

        bool check_win();
        bool check_three(const std::vector<std::string>& condition) const;

        void apply_win(player player_win);

        board():
            // used to initialize consts
            game_instructions{
                "This is called pokemon typing tictactoe. Just like tictactoe, it is a 2-player turn-based game. However there is a twist to it",
                "The rules are as the following:",
                " 1. Each player has a total of 9 pieces. There are 3 types of pieces:",
                "    - Water, Fire, Grass Pieces",
                "    - Water beats Fire, Fire beats Grass, and Grass beats Water",
                "     - If there is Water_O, then only thing that can beat it or take its 'place' is Grass_X",
                "    - Water does not beat Water, Fire does not beat Fire, and Grass does not beat Grass",
                "     - If there is Water_O, then Water_X and Fire_X cannot beat it and DOES not take its 'place'",

                " 2. There are a total of 3 Grass, 3 Fire and 3 Water Pieces",
                "    - Fire represents red",
                "    - Grass represents green",
                "    - Water represents blue",
                " 3. The game will end if either player is able to obtain a 3-straight or 3-diagonal of ANY of its pieces",
                " Good luck and enjoy the game!",
                " Press enter to start game"
            },
            board_map{
                {"tl", {1, 3}}, {"tm", {1, 9}}, {"tr", {1, 15}},
                {"ml", {3, 3}}, {"mm", {3, 9}}, {"mr", {3, 15}},
                {"bl", {5, 3}}, {"bm", {5, 9}}, {"br", {5, 15}}
            },

            turns_map_val{
                {"tl", 0}, {"tm", 0}, {"tr", 0},
                {"ml", 0}, {"mm", 0}, {"mr", 0},
                {"bl", 0}, {"bm", 0}, {"br", 0}
            },

            turns_map_pos{
                {"tl", {2, 53}}, {"tm", {2, 59}}, {"tr", {2, 65}},
                {"ml", {4, 53}}, {"mm", {4, 59}}, {"mr", {4, 65}},
                {"bl", {6, 53}}, {"bm", {6, 59}}, {"br", {6, 65}}
            },

            // first argument options
            player_first_options{
                "top-left(tl)", "top-middle(tm)", "top-right(tr)",
                "middle-left(ml)", "middle-middle(mm)", "middle-right(mr)",
                "bottom-left(bl)", "bottom-middle(bm)", "bottom-right(br)",
                "quit(q)"
            },

            // second argument options
            player_second_options{
                "fire(f)", "grass(g)", "water(w)" 
            },

            // final result could look like :
            example_commands{
                "top-left f",
                "tr water",
                "bm g"   
            },

            valid_first_arg{
                "top-left", "tl", "top-middle", "tm", "top-right", "tr",
                "middle-left", "ml", "middle-middle", "mm", "middle-right", "mr",
                "bottom-left", "bl", "bottom-middle", "bm", "bottom-right", "br",
                "quit", "q"
            },

            valid_second_arg{
                "fire", "f",
                "grass", "g",
                "water", "w"
            },

            first_arg_map{
                {"top-left", "tl"},
                {"top-middle", "tm"},
                {"top-right", "tr"},
                {"middle-left", "ml"},
                {"middle-middle", "mm"},
                {"middle-right", "mr"},
                {"bottom-left", "bl"},
                {"bottom-middle", "bm"},
                {"bottom-right", "br"},
                {"quit", "q"}
            },

            second_arg_map{
                {"fire", "f"},
                {"grass", "g"},
                {"water", "w"}
            },

            win_conditions{
                // Horizontal win conditions
                {"tl", "tm", "tr"},
                {"ml", "mm", "mr"},
                {"bl", "bm", "br"},
                // Vertical win conditions
                {"tl", "ml", "bl"},
                {"tm", "mm", "bm"},
                {"tr", "mr", "br"},
                // Diagonal win conditions
                {"tl", "mm", "br"},
                {"tr", "mm", "bl"}
            }

            // use to define the main body of the constructor
            {
                create_instructions();
                init_font_color();
                // create board
                create_board(board_origin_y, board_origin_x);
                // create turn board to count turns
                create_turn_board(turn_board_origin_y, turn_board_origin_x);
            };

    
    private:
        // Board field is from:
        //   x = 0 and y = 0 to
        //   x = 20 and y = 6
        int board_origin_x = 0; 
        int board_origin_y = 0;

        // Turn Board field is from:
        //   x = 50 and y = 1 to
        //   x = 70 and y = 7
        int turn_board_origin_x = 50;
        int turn_board_origin_y = 1;

        // create pieces of a player
        // Pieces start: x = 28 and y = 2
        // Pieces end  : x = 32 and y = 6 
        int piece_start_x = 28;

        int turn_number = 0;

        // Input field is from:
        //   x = 0 and y = 8 to
        //   x = 0 and y = 38
        int input_start_x = 0;
        int turn_start_y = 8;
        int option_start_y = 10;
        int player1_start_y = 32;
        int player2_start_y = 36;
        int win_text_start_y = 42;

        const std::vector<std::string> game_instructions;
        const std::vector<std::string> player_first_options;
        const std::vector<std::string> player_second_options;
        const std::vector<std::string> example_commands;
        const std::vector<std::string> valid_first_arg;
        const std::vector<std::string> valid_second_arg;
        const std::vector<std::vector<std::string>> win_conditions;

        const std::map<std::string, std::string> first_arg_map;
        const std::map<std::string, std::string> second_arg_map;
        const std::map<std::string, std::pair<int, int>> board_map;
        const std::map<std::string, std::pair<int, int>> turns_map_pos;

        std::map<std::string, int> turns_map_val;
};

#endif