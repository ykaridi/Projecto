cmake_minimum_required(VERSION 3.13)
project(ProjectoHW3 C)

set(CMAKE_C_STANDARD 99)

add_executable(ProjectoHW3 main.c soduku_game.c soduku_game.h soduku_solver.c soduku_solver.h main_aux.c main_aux.h utils.c utils.h)