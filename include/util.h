#pragma once
#ifndef UTILS_H
#define UTILS_H

#include <ncurses.h>
#include <iostream>
#include <fstream>
#include <sstream>

std::ofstream debugLog("debug.log");

void logDebug(const std::string &message) {
    debugLog << message << std::endl;
}

void deleteScreen(int start_y, int start_x, int height, int width)
{
    int y_bounds = start_y + height + 1;
    int x_bounds = start_x + width + 1;
    for(int i = start_y; i < y_bounds; i++){
        for(int j = start_x; j < x_bounds; j++){
            mvprintw(i, j, " ");
        }
    }

    refresh();
}

//find string in word with case ignore
bool containsWordIgnoreCase(std::string str, std::string word)
{
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
    std::transform(word.begin(), word.end(), word.begin(), ::tolower);

    size_t pos = str.find(word);

    return (pos != std::string::npos);
}

//find string in word
bool containsWord(const std::string str, const std::string word)
{
    return containsWordIgnoreCase(str, word);
}

// string to split using delimiter and place into a vector
void split_string(std::string string, std::vector<std::string> &stringList, char delimiter)
{
    std::istringstream streamstr(string);
    std::string string_candidate;
    while(std::getline(streamstr, string_candidate, delimiter))
    {
        stringList.push_back(string_candidate);
    }
}

#endif