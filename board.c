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


void board_fill(struct board* board, int num_clues)
{
    for (int i = 0; i < num_clues; i++)
    {

    }
}


static bool check_has_numbers(bool has_numbers[], int length)
{
    for (int i = 0; i < length; i++)
    {
        if (has_numbers[i] == false)
        {
            return false;
        }
    }
    return true;
}

bool board_check_all(struct board* board)
{
    for (int i = 0; i < board->height; i++)
    {
        if (!board_check_row(board, i) || !board_check_col(board, i) || !board_check_box(board, i % board->box_width, i / board->box_height))
        {
            return false;
        }
    }
    return true;
}

bool board_check_row(struct board* board, int row)
{
    bool row_has_numbers[board->width];
    memset(row_has_numbers, false, sizeof(row_has_numbers));
    for (int i = 0; i < board->width; i++)
    {
        int cell_number = board_get_cell(board, i, row);
        if (cell_number == 0)
        {
            return false;
        }
        row_has_numbers[cell_number - 1] = true;
    }
    return check_has_numbers(row_has_numbers, board->width);
}

bool board_check_col(struct board* board, int col)
{
    bool col_has_numbers[board->height];
    memset(col_has_numbers, false, sizeof(col_has_numbers));
    for (int i = 0; i < board->height; i++)
    {
        int cell_number = board_get_cell(board, col, i);
        if (cell_number == 0)
        {
            return false;
        }
        col_has_numbers[cell_number - 1] = true;
    }
    return check_has_numbers(col_has_numbers, board->height);
}

bool board_check_box(struct board* board, int box_x, int box_y)
{
    bool box_has_numbers[board->width];
    memset(box_has_numbers, false, sizeof(box_has_numbers));
    for (int i = 0; i < board->width; i++)
    {
        int cell_number = board_get_box_cell(board, box_x, box_y, i % 3, box_y / 3);
        if (cell_number == 0)
        {
            return false;
        }
        box_has_numbers[board_get_cell(board, i, box_y) - 1] = true;
    }
    return check_has_numbers(box_has_numbers, board->width);    
}


static bool is_out_of_bounds(int col, int row, int bounds_x, int bounds_y)
{
    return col < 0 || col >= bounds_x || row < 0 || row >= bounds_y;
}

int board_get_cell(struct board* board, int col, int row)
{
    if (is_out_of_bounds(col, row, board->width, board->height))
    {
        fprintf(stderr, "invalid cell (%d, %d)\n", col, row);
        return -1;
    }
    return board->cells[row * board->width + col];
}

int board_get_box_cell(struct board* board, int box_x, int box_y, int box_col, int box_row)
{
    if (is_out_of_bounds(box_col, box_row, board->box_width, board->box_height))
    {
        fprintf(stderr, "invalid box cell (%d, %d)\n", box_col, box_row);
        return -1;
    }
    if (is_out_of_bounds(box_x, box_y, board->box_width, board->box_height))
    {
        fprintf(stderr, "invalid box (%d, %d)\n", box_col, box_row);
        return -1;
    }
    return board->cells[(box_y * board->box_width * board->width) + (box_x * board->box_width) + (box_row * board->width) + (box_col)];
}

void board_set_cell(struct board* board, int col, int row, int value)
{
    if (is_out_of_bounds(col, row, board->width, board->height))
    {
        fprintf(stderr, "invalid cell (%d, %d)\n", col, row);
        return;
    }
    board->cells[row * board->width + col] = value;
}

void board_set_box_cell(struct board* board, int box_x, int box_y, int box_col, int box_row, int value)
{
    if (is_out_of_bounds(box_col, box_row, board->box_width, board->box_height))
    {
        fprintf(stderr, "invalid box cell (%d, %d)\n", box_col, box_row);
        return;
    }
    if (is_out_of_bounds(box_x, box_y, board->box_width, board->box_height))
    {
        fprintf(stderr, "invalid box (%d, %d)\n", box_col, box_row);
        return;
    }
    board->cells[(box_y * board->box_width * board->width) + (box_x * board->box_width) + (box_row * board->width) + (box_col)] = value;
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


