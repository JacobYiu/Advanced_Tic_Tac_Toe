cmake_minimum_required(VERSION 3.10)
project(Tic_Tac_Toe VERSION 1.0 DESCRIPTION "tic tac toe on C++")

set(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_STANDARD_REQUIRED True)

# Include header files
include_directories($(PROJECT_SOURCE_DIR)/include)

# Add the executable
add_executable(${PROJECT_NAME} src/main.cpp src/board.cpp src/player.cpp)

find_package(Curses REQUIRED)
if(CURSES_FOUND)
    include_directories(${CURSES_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} ${CURSES_LIBRARIES})
endif()