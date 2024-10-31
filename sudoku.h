#ifndef SUDOKU_H
#define SUDOKU_H

#include <stdlib.h>
#include <stdio.h>
#include <curses.h>
#include <time.h>
#include "board.h"

void init_curses();

void run();
void main_loop(struct board* board);

void handle_input(struct board* board);

#endif 
