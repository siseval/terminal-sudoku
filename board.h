#ifndef BOARD_H
#define BOARD_H

#include <stdlib.h>
#include <stdio.h>
#include <curses.h>
#include "cli.h"
#define REFRESH_MS 500

struct board
{
    int box_width;
    int box_height;
    int width;
    int height;
    int* cells;
};

struct board* board_create(int box_dimensions);
void board_destroy(struct board* board);

void board_fill(struct board* board, int num_clues);

bool board_check_all(struct board* board);
bool board_check_row(struct board* board, int row);
bool board_check_col(struct board* board, int col);
bool board_check_box(struct board* board, int box_x, int box_y);

int board_get_cell(struct board* board, int row, int col);
int board_get_box_cell(struct board* board, int box_x, int box_y, int box_col, int box_row);
void board_set_cell(struct board* board, int row, int col, int value);
void board_set_box_cell(struct board* board, int box_x, int box_y, int box_col, int box_row, int value);

void board_print(struct board* board);

#endif
