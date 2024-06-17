#include <iostream>
#include <fstream>
#include <sstream>
#include "player.h"


std::ofstream debugLogPlayer("debug_player.log");

void logDebugPlayer(const std::string &message) {
    debugLogPlayer << message << std::endl;
}

std::string player::get_player_num()
{
    return player_id;
}

std::string player::get_piece()
{
    return piece;
}

std::map<std::string, int> player::get_elements()
{
    return elements;
}

void player::remove_element(std::string elem_to_remove)
{
    std::map<std::string,int>::iterator element_count_itr;

    for(element_count_itr = elements.begin(); element_count_itr != elements.end(); element_count_itr++)
    {
        std::string elem_to_compare = element_count_itr -> first;
        if(elem_to_remove == elem_to_compare)
        {
            // reduce the element associated with it
            element_count_itr -> second --;
        }
    }
}

void player::get_elem_count()
{
    std::map<std::string,int>::iterator element_count_itr;
    int elem_x_count;
    for(element_count_itr = elements.begin(); element_count_itr != elements.end(); element_count_itr++)
    {
        std::string element = element_count_itr -> first;
        std::string element_count = std::to_string(element_count_itr -> second);
        logDebugPlayer("Player " + player_id + " " + element + " element is at: " + element_count);
    }
}
int player::get_elem_x_count(std::string type)
{
    std::map<std::string,int>::iterator element_count_itr;
    int elem_x_count;
    for(element_count_itr = elements.begin(); element_count_itr != elements.end(); element_count_itr++)
    {
        std::string elem_to_compare = element_count_itr -> first;
        if(type == elem_to_compare)
        {
            elem_x_count = element_count_itr -> second;
        }
    }

    return elem_x_count;
}