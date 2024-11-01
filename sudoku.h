#ifndef SUDOKU_H
#define SUDOKU_H

#include <stdlib.h>
#include <stdio.h>
#include <curses.h>
#include <time.h>
#include "board.h"
#include "cli.h"

void sudoku_run(int board_box_size, int num_clues);

#endif 
