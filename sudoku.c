#include "sudoku.h"

int main()
{
    init_curses();        
    srand(time(NULL));

    run();

    endwin();
}

void init_curses()
{
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    timeout(REFRESH_MS);

    start_color();
    use_default_colors();
    init_pair(1, COLOR_WHITE, -1);
}

void run()
{
    struct board* board = board_create(3);
    run_loop(board);
}

void run_loop(struct board* board)
{
    int i = 0;
    while (true)
    {
        handle_input(board);
        board_set_box_cell(board, 0, 0, i % 3, i / 3, i % 4 + 1);
        if (i >= 8 && board_check_box(board, 0, 0))
        {
            exit(0);
        }
        // board_set_box_cell(board, 1, 1, 1, 2, rand() % 9 + 1);
        board_print(board);
        i++;
    }
}

void handle_input(struct board* board)
{
    getch();
}

