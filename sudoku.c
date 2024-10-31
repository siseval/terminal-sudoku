#include "sudoku.h"
#include "board.h"

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
    main_loop(board);
}

void main_loop(struct board* board)
{
    board_generate_puzzle(board, 81);
    board_print(board);
    while (true)
    {
        handle_input(board);
        board_print(board);
    }
}

void handle_input(struct board* board)
{
    switch (getch())
    {
        case 'h':
        case 'a':

            break;
        case 'l':
        case 'd':
            break;
        case 'k':
        case 'w':
            break;
        case 'j':
        case 's':
            break;
    }
}

