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

    int* col_statuses;
    int* row_statuses;
    int* box_statuses;

    int* clues_positions;
    int num_clues;

    int cursor_pos[2];

    int* cells;
};

struct board* board_create(const int box_dimensions);
void board_destroy(struct board* board);

void board_generate_puzzle(struct board* board, const int num_clues);
void board_solve_puzzle(struct board* board);

bool board_is_solved(struct board* board);
void board_update_statuses(struct board* board);
int board_col_status(const struct board* board, const int col);
int board_row_status(const struct board* board, const int row);
int board_box_status(const struct board* board, const int box_x, const int box_y);
int board_cell_lowest_status(const struct board* board, const int col, const int row);

int board_get_cell(const struct board* board, const int row, const int col);
int board_get_box_cell(const struct board* board, const int box_x, const int box_y, const int box_col, const int box_row);
void board_set_cell(struct board* board, const int row, const int col, const int value);
void board_set_box_cell(struct board* board, const int box_x, const int box_y, const int box_col, const int box_row, const int value);

bool board_try_set_cell(struct board* board, const int col, const int row, const int value);

void board_move_cursor(struct board* board, const int dx, const int dy);

void board_print(const struct board* board);

#endif
