#ifndef PLAYER_H
#define PLAYER_H

#include <stdio.h>
#include <string>
#include <map>

class player{
    public:  
        // std::map<std::string, std::pair<int, int>> element_locations = {
        //     {"Water_O 1", {0, 20}}, {"Water_O 2", {0, 22}}, {"Water_O 3", {0, 24}},
        //     {"Water_X 1", {0, 26}}, {"Water_X 2", {0, 28}}, {"Water_X 3", {0, 30}},
        //     {"Fire_O 1", {1, 20}}, {"Fire_O 2", {1, 22}}, {"Fire_O 3", {1, 24}},
        //     {"Fire_X 1", {1, 26}}, {"Fire_X 2", {1, 28}}, {"Fire_X 3", {1, 30}},   
        //     {"Grass_O 1", {2, 20}}, {"Grass_O 2", {2, 22}}, {"Grass_O 3", {2, 24}},
        //     {"Grass_X 1", {2, 26}}, {"Grass_X 2", {2, 28}}, {"Grass_X 3", {2, 30}},                 
        // };
        player(std::string piece_, std::string player_id_){
            piece = piece_;
            player_id = player_id_;
            elements = 
            {
                {"f", 3},
                {"g", 3},
                {"w", 3},
            };
        }

        // Player 1(1) or Player 2(2)
        std::string get_player_num();
        std::string get_piece();
        std::map<std::string, int> get_elements();
        void get_elem_count();
        int get_elem_x_count(std::string type);

        void remove_element(std::string type);


    private:
        std::string player_id;
        std::string piece;  
        std::map<std::string, int> elements;
        
}; 

#endif