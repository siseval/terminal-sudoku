#include "board.h"

struct board* board_create(int box_dimensions)
{
    struct board* board = calloc(1, sizeof(struct board));

    board->box_width = box_dimensions;
    board->box_height = box_dimensions;

    board->width = box_dimensions * box_dimensions;
    board->height = box_dimensions * box_dimensions;

    board->cells = calloc(1, sizeof(int) * board->width * board->height);

    return board;
}

void board_destroy(struct board* board)
{
    free(board->cells);
    free(board);
}


int board_get_cell(struct board *board, int row, int col)
{
    if (row < 0 || row >= board->width || col < 0 || col >= board->height)
    {
        fprintf(stderr, "invalid cell (%d, %d)\n", row, col);
        return -1;
    }
    return board->cells[row * board->width + col];
}

void board_set_cell(struct board *board, int row, int col, int value)
{
    if (row < 0 || row >= board->width || col < 0 || col >= board->height)
    {
        fprintf(stderr, "invalid cell (%d, %d)\n", row, col);
    }
    board->cells[row * board->width + col] = value;
}


void print_hor_line(struct board* board, bool is_hor_edge)
{
    move_center_h(-((board->width * 4 + 1) / 2));
    for (int i = 0; i < board->width; i++)
    {
        bool is_vert_edge = i % board->box_width == 0;
        if (is_hor_edge || is_vert_edge)
        { 
            attron(A_BOLD); 
        }

        char edge_symbols[2][2] = { {' ', '|'}, { '-', '+' } };
        printw("%c", edge_symbols[is_hor_edge][is_vert_edge]);

        if (!is_hor_edge)
        {
            attroff(A_BOLD);
        }
        printw("---");
    }
    attron(A_BOLD);
    printw(is_hor_edge ? "+\n" : "|\n");
    attroff(A_BOLD);
}

void print_row(struct board* board, int row)
{
    move_center_h(-((board->width * 4 + 1) / 2));
    for (int i = 0; i < board->width; i++)
    {
        if (i % board->box_width == 0) 
        {
            attron(A_BOLD);
        }
        printw("| ");
        attroff(A_BOLD);
        printw("%d ", board_get_cell(board, i, row));
    }
    attron(A_BOLD);
    printw("|\n");
    attroff(A_BOLD);
}

void board_print(struct board* board)
{
    clear();

    attron(COLOR_PAIR(1));
    move_center_v(-((board->height * 2 + 1) / 2));
    for (int i = 0; i < board->height; i++)
    {
        print_hor_line(board, i % board->box_height == 0);
        print_row(board, i);
    }
    print_hor_line(board, true);
}


