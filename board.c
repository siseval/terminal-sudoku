#include "board.h"

struct board* board_create(const int box_dimensions)
{
    struct board* board = calloc(1, sizeof(struct board));

    board->box_width = box_dimensions;
    board->box_height = box_dimensions;

    board->width = box_dimensions * box_dimensions;
    board->height = box_dimensions * box_dimensions;

    board->col_statuses = calloc(1, sizeof(int) * board->height);
    board->row_statuses = calloc(1, sizeof(int) * board->width);
    board->box_statuses = calloc(1, sizeof(int) * board->box_width * board->box_height);

    board->cursor_pos[0] = 0;
    board->cursor_pos[1] = 0;

    board->cells = calloc(1, sizeof(int) * board->width * board->height);

    return board;
}

void board_destroy(struct board* board) 
{
    free(board->cells);
    free(board->col_statuses);
    free(board->row_statuses);
    free(board->box_statuses);
    free(board);
}


void board_generate_puzzle(struct board* board, const int num_clues)
{

    int nums_tried[board->width * board->height];
    memset(nums_tried, 0, sizeof(int) * board->width * board->height);

    int check_order[board->width];
    for (int i = 1; i <= board->width; i++)
    {
        check_order[i - 1] = i;
    }

    int cell_index = 0;
    while (cell_index < board->width * board->height)
    {
        for (int i = 0; i < board->width * 3; i++)
        {
            int pos1 = rand() % board->width;
            int pos2 = rand() % board->width;
            int buf = check_order[pos1];
            check_order[pos1] = check_order[pos2];
            check_order[pos2] = buf;
        }

        bool all_tried = nums_tried[cell_index] == board->width;

        if (all_tried)
        {
            nums_tried[cell_index] = 0;
            board->cells[cell_index] = 0;
            if (cell_index > 0)
            {
                cell_index--;
            }
            continue;
        }

        int candidate_num = check_order[nums_tried[cell_index]];

        board->cells[cell_index] = candidate_num;
        nums_tried[cell_index]++;

        bool failed = board_cell_lowest_status(board, cell_index % board->width, cell_index / board->height) == -1;
        if (failed)
        {
            continue;
        }

        cell_index++;
    }

    for (int i = 0; i < board->width * board->height - num_clues; i++)
    {
        int cell_index = -1; 
        while (cell_index == -1)
        {
            int candidate_index = rand() % (board->width * board->height);
            cell_index = board->cells[candidate_index] != 0 ? candidate_index : -1;
        }
        board->cells[cell_index] = 0;
    }
}


int board_highest_status(struct board* board)
{
    board_update_statuses(board);
    int highest = -1;
    for (int i = 0; i < board->width; i++)
    {
        int col_status = board->col_statuses[i];
        int row_status = board->row_statuses[i];
        int box_status = board->box_statuses[i];

        if (col_status == 1 || row_status == 1 || box_status == 1)
        {
            return 1;
        }

        if (col_status > highest)
        {
            highest = col_status;
        }
        if (row_status > highest)
        {
            highest = row_status;
        }
        if (box_status > highest)
        {
            highest = box_status;
        }
    }
    return highest;
}

int board_lowest_status(struct board* board)
{
    board_update_statuses(board);
    int lowest = 1;
    for (int i = 0; i < board->width; i++)
    {
        int col_status = board->col_statuses[i];
        int row_status = board->row_statuses[i];
        int box_status = board->box_statuses[i];

        if (col_status == -1 || row_status == -1 || box_status == -1)
        {
            return -1;
        }

        if (col_status < lowest)
        {
            lowest = col_status;
        }
        if (row_status < lowest)
        {
            lowest = row_status;
        }
        if (box_status < lowest)
        {
            lowest = box_status;
        }
    }
    return lowest;
}


void board_update_statuses(struct board* board)
{
    for (int i = 0; i < board->height; i++)
    {
        board->col_statuses[i] = board_col_status(board, i);
        board->row_statuses[i] = board_row_status(board, i);
        board->box_statuses[i] = board_box_status(board, i % 3, i / 3);
    }
}


int board_cell_lowest_status(const struct board* board, const int col, const int row)
{
    int lowest = 1;
    int col_status = board_col_status(board, col);
    int row_status = board_row_status(board, row);
    int box_status = board_box_status(board, col / board->box_width, row / board->box_height);
    if (col_status < lowest) { lowest = col_status; }
    if (row_status < lowest) { lowest = row_status; }
    if (box_status < lowest) { lowest = box_status; }
    return lowest;

}

int board_col_status(const struct board* board, const int col)
{
    bool col_has_numbers[board->width + 1];
    memset(col_has_numbers, false, sizeof(col_has_numbers));
    for (int i = 0; i < board->width; i++)
    {
        int cell_number = board_get_cell(board, col, i);
        if (cell_number != 0 && col_has_numbers[cell_number])
        {
            return -1;
        }
        col_has_numbers[cell_number] = true;
    }
    return !col_has_numbers[0];
}

int board_row_status(const struct board* board, const int row)
{
    bool row_has_numbers[board->height + 1];
    memset(row_has_numbers, false, sizeof(row_has_numbers));
    for (int i = 0; i < board->height; i++)
    {
        int cell_number = board_get_cell(board, i, row);
        if (cell_number != 0 && row_has_numbers[cell_number])
        {
            return -1;
        }
        row_has_numbers[cell_number] = true;
    }
    return !row_has_numbers[0];
}

int board_box_status(const struct board* board, const int box_x, const int box_y)
{
    bool box_has_numbers[board->width + 1];
    memset(box_has_numbers, false, sizeof(box_has_numbers));
    for (int i = 0; i < board->width; i++)
    {
        int cell_number = board_get_box_cell(board, box_x, box_y, i % board->box_width, i / board->box_height);
        if (cell_number != 0 && box_has_numbers[cell_number])
        {
            return -1;
        }
        box_has_numbers[board_get_box_cell(board, box_x, box_y, i % board->box_width, i / board->box_height)] = true;
    }
    return !box_has_numbers[0];
}


static bool is_out_of_bounds(const int col, const int row, const int bounds_x, const int bounds_y)
{
    return col < 0 || col >= bounds_x || row < 0 || row >= bounds_y;
}

int board_get_cell(const struct board* board, const int col, const int row)
{
    if (is_out_of_bounds(col, row, board->width, board->height))
    {
        fprintf(stderr, "invalid cell (%d, %d)\n", col, row);
        return -1;
    }
    return board->cells[row * board->width + col];
}

int board_get_box_cell(const struct board* board, const int box_x, const int box_y, const int box_col, const int box_row)
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

void board_set_cell(struct board* board, const int col, const int row, const int value)
{
    if (is_out_of_bounds(col, row, board->width, board->height))
    {
        fprintf(stderr, "invalid cell (%d, %d)\n", col, row);
        return;
    }
    board->cells[row * board->width + col] = value;
}

void board_set_box_cell(struct board* board, const int box_x, const int box_y, const int box_col, const int box_row, const int value)
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


static void print_hor_line(const struct board* board, const bool is_hor_edge)
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

static void print_row(const struct board* board, const int row)
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
        int cell_number = board_get_cell(board, i, row);
        cell_number != 0 ? printw("%d ", cell_number) : printw("  ");
    }
    attron(A_BOLD);
    printw("|\n");
    attroff(A_BOLD);
}

void board_print(const struct board* board)
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
