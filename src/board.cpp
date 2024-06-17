#include <ncurses.h>
#include <algorithm>
#include <string>
#include "board.h"
#include "util.h"


void board::create_board(int origin_y, int origin_x)
{
    const int buffer_x = 1;

    // Used to create 4 vertical line with 7 "|" characters (top-to-bottom)
    // Starts at x = 0 and y = 0;
    for(int offset_y = 0; offset_y < 7; offset_y++){
        int new_y = origin_y + offset_y;

        for(int v_count = 0; v_count < 4; v_count++)
        {
            // Each vertical line is separated by 6 spaces
            int new_x = origin_x + (v_count * 6);
            move(new_y, new_x);
            printw("|");
        }                
        refresh();
    }

    // Total of 4 rows; each with 3 horizontal lines with 3 "-" characters(left-to-right)
    // Starts at x = 1 and y = 0 

    for(int offset_x = 0; offset_x < 5; offset_x++){
        int h_start = origin_x + offset_x;
        for(int h_count = 0; h_count < 3; h_count++)
        {
            int new_x = h_start + buffer_x + (h_count * 6);
            for(int y_offset = 0; y_offset < 4; y_offset++)
            {
                int new_y = origin_y + (y_offset * 2);
                move(new_y,new_x);
                printw("-");
            }        
        }
        refresh();
    }
}

void board::create_turn_board(int origin_y, int origin_x)
{
    mvprintw(origin_y - 1, origin_x + 5, "Turn Board");
    create_board(origin_y, origin_x);
    display_turns();
}

void board::init_font_color()
{
    start_color();

    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_BLUE, COLOR_BLACK);
    init_pair(4, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(5, COLOR_YELLOW, COLOR_BLACK);
}

// TO DO
void board::display_turns()
{
    std::map<std::string, std::pair<int,int>>::const_iterator turns_map_pos_itr;
    std::string strpos;
    std::pair<int,int> coordinates;
    int y_pos;
    int x_pos;
    int turns_left;
    for(turns_map_pos_itr = turns_map_pos.begin(); turns_map_pos_itr != turns_map_pos.end(); turns_map_pos_itr++)
    {
        strpos = turns_map_pos_itr -> first;
        coordinates = turns_map_pos_itr -> second;
        y_pos = coordinates.first;
        x_pos = coordinates.second;
        
        turns_left = get_turnsLeft(strpos);
        
        mvprintw(y_pos, x_pos, "%d", turns_left);
    }
}

void board::create_instructions()
{
    int y_pos = 0;
    for(int i = 0; i < game_instructions.size(); i++){
        mvprintw(y_pos, 0, game_instructions[i].c_str());
        y_pos++;
    }

    refresh();
    getch();

    clear();
}

void board::create_pieces(player cur_player)
{
    // std::string player_id = cur_player.get_player_num();
    std::string piece = cur_player.get_piece();
    std::map<std::string, int> elements = cur_player.get_elements();
    
    // cur_player.get_elem_count();
    display_pieces(piece, elements);

    refresh();
}

// show player available pieces
void board::display_pieces(std::string piece, std::map<std::string, int> elements)
{
    std::map<std::string, int>::iterator element_itr;

    // the color of the text(1 = red, 2 = green, 3 = blue)
    int color;
    int y_pos;

    // final x value after offsetting of x_origin
    int x_pos;

    // used to refresh the pieces
    deleteScreen(1, 28, 10, 20);

    for(element_itr = elements.begin(); element_itr != elements.end(); element_itr++)
    {
        std::string element_type = element_itr -> first;
        std::pair<int, int> element_col_pos = get_element_w_pos(element_type);
        color = element_col_pos.first;
        y_pos = element_col_pos.second;

        int element_count = element_itr -> second;
        for(int i = 0; i < element_count; i++){
            x_pos = piece_start_x + (i * 2);
            attron(COLOR_PAIR(color));
            mvprintw(y_pos, x_pos, piece.c_str());
        }
    }
    attrset(A_NORMAL);
}

//return element_color
std::pair<int,int> board::get_element_w_pos(std::string element_type)
{
    if(element_type == "f"){
        return std::make_pair(RED,2);
    }

    else if(element_type == "g"){
        return std::make_pair(GREEN,4);
    }

    else if(element_type == "w"){
        return std::make_pair(BLUE,6);
    }

    return std::make_pair(-1, -1);
}

// maps from element_type to color_pair
// "f" -> RED
int board::get_element_assoc(std::string element_type)
{
    bool is_fire;
    bool is_grass;
    bool is_water;
    if(element_type.size() < 2)
    {
        is_fire = element_type == "f";
        is_grass = element_type == "g";
        is_water = element_type == "w";
    }

    // else
    // {
    //     is_fire = containsWord(element_type, "fire");
    //     is_grass = containsWord(element_type, "grass");
    //     is_water = containsWord(element_type, "water");
    // }

    if(is_fire){
        return RED;
    }

    else if(is_grass){
        return GREEN;
    }

    else if(is_water){
        return BLUE;
    }

    return -1;
}

// maps from color_pair to element type
std::string board::get_element(int element_assoc)
{
    switch(element_assoc)
    {
        // returns nothing(no type element because character is ' ')
        case(0):
            return "n";
            break;
        // returns fire
        case(1):
            return "f";
            break;
        // returns grass
        case(2):
            return "g";
            break;
        // returns water
        case(3):
            return "w";
            break;
        // returns error
        default:
            return "e";
    }
}

// get the mapping for string to turns left before being able to place and position in availablePosition
// e.g. (tl -> {0, {1,3}))
std::pair<int,int> board::get_strpos_pair(std::string string_pos)
{
    return board_map.at(string_pos);
}

int board::get_turnsLeft(std::string string_pos)
{
    return turns_map_val.at(string_pos);
}

void board::update_turnsLeft(std::string string_pos, int turns, bool is_positive)
{
    int &turnsLeft = turns_map_val.at(string_pos);
    if(is_positive)
        turnsLeft += turns;
    else
        // So that turnsLeft never becomes zero
        if(turnsLeft > 0)
        {
            turnsLeft -= turns;
        }
    
    if(turnsLeft < 0)
    {
        std::string msg_err = "turns left at position " + string_pos + " is negative";
        logDebug(msg_err);
    }
}

void board::global_turns_decrement()
{
    std::map<std::string, int>::iterator turns_map_val_itr;
    for(turns_map_val_itr = turns_map_val.begin(); turns_map_val_itr != turns_map_val.end(); turns_map_val_itr++)
    {
        if(turns_map_val_itr -> second > 0)
        {
            turns_map_val_itr -> second--;
        }
    }
}


std::string board::get_player_choice(player& player_to_move, bool is_player1_turn)
{
    // used to display-options to both users
    mvprintw(turn_start_y,0, "Turn number %d", turn_number);
    display_options();
    std::string command;
    // -----------------------------------------------------------------------------
    // player 1 turn 
    if(is_player1_turn)
    {
        command = start_turn(player1_start_y, player_to_move, is_player1_turn);
    }

    // -----------------------------------------------------------------------------
    // player 2 turn
    else
    {
        command = start_turn(player2_start_y, player_to_move, is_player1_turn);

        turn_number ++;
    }
 
    return command;
}

void board::display_options()
{
    std::vector<std::string>::const_iterator first_options_itr;
    std::vector<std::string>::const_iterator second_options_itr;
    std::vector<std::string>::const_iterator correct_commands_itr;

    int y_offset = 0;
    int y_pos;
    std::string pos;
    
    //display first option
    attrset(COLOR_PAIR(5));
    mvprintw(option_start_y - 1, input_start_x, "Player first options are: ");
    attrset(COLOR_PAIR(4));
    for(first_options_itr = player_first_options.begin(); first_options_itr != player_first_options.end(); first_options_itr++)
    {
        y_pos = option_start_y + y_offset;
        std::string first_option = *first_options_itr;
        mvprintw(y_pos,input_start_x, first_option.c_str());
        y_offset += 1;
    }
    
    //display second option
    attrset(COLOR_PAIR(5));
    y_offset += 1;
    mvprintw(option_start_y + y_offset,input_start_x, "Player second options are: ");
    y_offset += 1;
    for(second_options_itr = player_second_options.begin(); second_options_itr != player_second_options.end(); second_options_itr++)
    {
        y_pos = option_start_y + y_offset;
        std::string second_option = *second_options_itr;
        int color = get_element_assoc(second_option);
        attron(COLOR_PAIR(color));
        mvprintw(y_pos, input_start_x, second_option.c_str());
        y_offset += 1;
    }

    //display example list of correct commands
    attrset(COLOR_PAIR(5));
    y_offset += 1;
    mvprintw(option_start_y + y_offset,input_start_x, "Example of correct commands are: ");
    y_offset += 1;
    attrset(COLOR_PAIR(4));
    for(correct_commands_itr = example_commands.begin(); correct_commands_itr != example_commands.end(); correct_commands_itr++)
    {
        y_pos = option_start_y + y_offset;
        std::string example_arg = *correct_commands_itr;
        mvprintw(y_pos,input_start_x, example_arg.c_str());
        y_offset += 1;
    }

    attrset(A_NORMAL);

    refresh();
}


// Two phases to this: 
//  check if choice is in option
//  check if choice can be placed due to typing advantages
bool board::check_choice(int y_pos, player player_to_move, std::string choice, bool is_player1_turn)
{
    // vector for our arugments
    std::vector<std::string> arguments;
    std::string stringpos;
    std::string type;

    bool quit = false;

    // check if choice is quit or q
    if(choice != "quit" && choice != "q")
    {
        split_string(choice, arguments, ' ');
        if(arguments.size() != 2)
        {
            gen_error(y_pos, 0, "Invalid command");
            return false;
        }
        // Split our arguments and place them into the vector
        // check if need to convert arguments
        convert_arguments(arguments);

        // obtain argument
        stringpos = arguments[0];
        type = arguments[1];
    }

    else
    {
        arguments.push_back(choice);
        convert_arguments(arguments);
        stringpos = arguments[0];
        quit = true;
    }
    

    // find for choice in valid_player_options
    bool in_option = std::find(valid_first_arg.begin(), valid_first_arg.end(), stringpos) != valid_first_arg.end();
    if(!(in_option)){
        gen_error(y_pos, 0, "Invalid first argument");
        return false;
    }

    // if quit is signaled by the user, then quit the game
    if(quit)
    {
        return true;
    }

    // check if type is in the list of availabel commands 
    in_option = std::find(valid_second_arg.begin(), valid_second_arg.end(), type) != valid_second_arg.end();
    if(!(in_option)){
        gen_error(y_pos, 0, "Invalid second argument");
        return false;
    }


    // check if player has any pieces of that element left
    std::map<std::string,int>::iterator element_count_itr;
    std::map<std::string,int> player_element_count = player_to_move.get_elements();
    for(element_count_itr = player_element_count.begin(); element_count_itr != player_element_count.end(); element_count_itr++){
        std::string elem_to_compare = element_count_itr -> first;
        if(type == elem_to_compare)
        {
            int type_count = element_count_itr -> second;
            if(type_count == 0)
            {
                gen_error(y_pos, 0, "Player does not have enough piece of that type");
                return false;
            }
        }
    }

    int turns_left = get_turnsLeft(stringpos);


    if(turns_left > 0)
    {
        std::string turns_left_err = "Turns left is not 0 yet!. Turns left for pos " + stringpos + ": " + std::to_string(turns_left); 
        gen_error(y_pos, 0, turns_left_err);
        return false;
    }

    bool is_possible = true;

    // Check board for character and color/type at that position
    std::pair<char, int> character_and_type = get_char_and_color(stringpos);
    
    // Convert the char into a string
    std::string board_char(1, character_and_type.first); 
    int board_type_i = character_and_type.second;
    // obtain element/type
    std::string board_type = get_element(board_type_i);
    
    std::string user_char = player_to_move.get_piece();
    std::string user_type = type;
    
    is_possible = check_type_adv(y_pos, user_char, user_type, board_char, board_type);
    if(!is_possible){
        return false; 
    }

    return true;
}

void board::convert_arguments(std::vector<std::string> &argument_list)
{
    std::string stringpos = argument_list[0];
    if(stringpos.length() > 2)
    {
        // do conversion for first argument
        // top-left -> tl
        std::map<std::string, std::string>::const_iterator first_arg_map_itr;
        for(first_arg_map_itr = first_arg_map.begin(); first_arg_map_itr != first_arg_map.end(); first_arg_map_itr++)
        {
            // check if first argument is in the list of valid arguments
            std::string word = first_arg_map_itr->first;    
            if(stringpos == word)
            {
                // get value associated with key
                std::string updated_arg = first_arg_map_itr -> second;
                argument_list[0] = updated_arg;
                break;
            }
        }
    }

    // check if argument list is just one character only("quit or q")
    if(argument_list.size() < 2)
    {
        return;
    }

    std::string type = argument_list[1];
    if(type.length() > 1)
    {
        // do conversion for second argument
        // water -> w
        std::map<std::string, std::string>::const_iterator second_arg_map_itr;
        for(second_arg_map_itr = second_arg_map.begin(); second_arg_map_itr != second_arg_map.end(); second_arg_map_itr++)
        {
            // check if second argument is in the list of valid arguments
            std::string word = second_arg_map_itr->first;
            if(type == word)
            {
                // get value associated with the key
                std::string updated_arg = second_arg_map_itr -> second; 
                argument_list[1] = updated_arg;
                break;
            }
        }
    }
}

void board::gen_error(int y_pos, int x_pos, std::string err_msg)
{
    attron(COLOR_PAIR(1));
    // delete user input
    deleteScreen(y_pos,x_pos,0,70);
    // delete prior error messages
    deleteScreen(y_pos + 1,x_pos,0,70);
    
    mvprintw(y_pos + 1,0, err_msg.c_str());
    attrset(A_NORMAL);
}

char board::get_char(const std::string& pos) const {
    std::pair<int,int> coordinates = const_cast<board*>(this)->get_strpos_pair(pos);
    int pos_y = coordinates.first;
    int pos_x = coordinates.second;

    chtype chAttr = mvinch(pos_y, pos_x);
    return chAttr & A_CHARTEXT;
}

std::pair<char,int> board::get_char_and_color(std::string string_pos)
{
    std::pair<int,int> coordinates = get_strpos_pair(string_pos);

    int y_pos = coordinates.first; 
    int x_pos = coordinates.second;
    chtype chAttr = mvinch(y_pos,x_pos);

    char ch = chAttr & A_CHARTEXT;
    int attr = chAttr & A_ATTRIBUTES;
    // extract pair number associated with character
    int color_pair = PAIR_NUMBER(attr);

    return std::make_pair(chAttr,color_pair);
}

// NEED TO FIX FOR BOTH
bool board::check_type_adv(int y_pos, std::string user_char, std::string user_type, std::string board_char, std::string board_type)
{
    bool valid = true;
    // logDebug(user_type);
    // logDebug(board_type);
    if(user_char == board_char)
    {
        gen_error(y_pos, 0, "You cannot take over your own piece!!");
        return false;
    }

    // if user_type is fire
    if(user_type == "f")
    {
        if(board_type == "w")
        {
            valid = false;
            gen_error(y_pos, 0, "fire type can only beat grass type and none");
        }
    }

    // if user_type is grass
    else if(user_type == "g")
    {
        if(board_type == "f")
        {
            valid = false;
            gen_error(y_pos, 0, "grass type can only beat water type and none");
        }
    }

    // if user_type is water
    else if(user_type == "w")
    {
        if(board_type == "g")
        {
            valid = false;
            gen_error(y_pos, 0, "water type can only beat fire type and none");
        }
    }

    return valid;
}

std::string board::start_turn(int y_origin, player player, bool is_player1_turn)
{
    // Used to refresh the screen for any user input and user pieces
    if(is_player1_turn)
    {
        // player_1 input starts from y = 32
        deleteScreen(player1_start_y,0,10,70);
    }

    std::string player_text = is_player1_turn ? "Player 1" : "Player 2"; 

    char command[80];
    bool valid_choice = true;
    int y_offset = 0;

    std::string player_turn_txt = player_text + " Turn";
    mvprintw(y_origin + y_offset,0, player_turn_txt.c_str());
    mvprintw(0,27, player_text.c_str());
    
    // player.get_elem_count();
    create_pieces(player);

    
    y_offset = 2;
    mvprintw(y_origin + y_offset, 0, "Enter a choice: ");
    getstr(command);        // Get a string from the user

    valid_choice = check_choice(y_origin+y_offset, player, command, is_player1_turn);

    // checking if command given
    while(!valid_choice){
        deleteScreen(y_origin + y_offset,0,0,70);
        mvprintw(y_origin + y_offset,0, "Kindly enter a valid command: ");
        getstr(command);        // Get a string from the user

        valid_choice = check_choice(y_origin+y_offset, player, command, is_player1_turn);
        refresh();
    }
    
    std::string text_entered = player_text + " entered " + command;

    deleteScreen(y_origin + y_offset,0,3,70);
    mvprintw(y_origin + y_offset, 0, text_entered.c_str());

    return std::string(command);
}


void board::update_board(player &player_to_move, std::string command, bool &is_player1_turn)
{
    std::vector<std::string> arguments;
    std::string stringpos;
    std::string type;
    
    // split the commands into position(string) and type
    split_string(command, arguments, ' ');
    stringpos = arguments[0];
    type = arguments[1];

    // get coordinates from the string position
    // tl -> (1,3)
    int color_pair = get_element_assoc(type);
    std::pair<int,int> coordinates = get_strpos_pair(stringpos);

    // update board first

    int y_pos = coordinates.first;
    int x_pos = coordinates.second;

    // get player piece
    std::string piece = player_to_move.get_piece();

    // place the piece on the board
    attron(COLOR_PAIR(color_pair));
    mvprintw(y_pos, x_pos, piece.c_str());
    attrset(A_NORMAL);

    // remove the players piece from the board
    player_to_move.remove_element(type);

    // global decrement for every stringpos first
    global_turns_decrement();

    // increment turn by 2 for that stringpos
    int turns_increase = 2;
    update_turnsLeft(stringpos, turns_increase, true);

    // end the players turn
    if(is_player1_turn)
        is_player1_turn = false;
    else
        is_player1_turn = true;
} 


bool board::check_win()
{
    std::vector<std::vector<std::string>>::const_iterator win_conditions_itr;
    for(win_conditions_itr = win_conditions.begin(); win_conditions_itr != win_conditions.end(); win_conditions_itr++)
    {
        // checks for one of the many win conditions
        if(check_three(*win_conditions_itr))
        {
            return true;
        }
    }

    return false;
}


bool board::check_three(const std::vector<std::string>& win_cond) const
{
    char first_ch = get_char(win_cond[0]);
    char second_ch = get_char(win_cond[1]);
    char third_ch = get_char(win_cond[2]);

    return (first_ch == second_ch) && (second_ch == third_ch) && (first_ch != ' ');
}

void board::apply_win(player player_win)
{
    std::string player_id = player_win.get_player_num();
    std::string win_text = "Player " + player_id + " wins!!!"; 
    attron(COLOR_PAIR(5));
    mvprintw(win_text_start_y, 0, win_text.c_str());
    mvprintw(win_text_start_y + 1, 0, "Press enter to exit..");
    attrset(A_NORMAL);
    getch();
}


