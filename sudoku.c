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
    init_pair(2, COLOR_YELLOW, -1);
}

void run()
{
    struct board* board = board_create(3);
    main_loop(board);
}

void main_loop(struct board* board)
{
    board_generate_puzzle(board, 21);
    board_print(board);
    while (true)
    {
        handle_input(board);
        board_print(board);
    }
}

void handle_input(struct board* board)
{
    char input = getch();
    switch (input)
    {
        case 'h':
        case 'a':
            board_move_cursor(board, -1, 0);
            break;
        case 'l':
        case 'd':
            board_move_cursor(board, 1, 0);
            break;
        case 'k':
        case 'w':
            board_move_cursor(board, 0, -1);
            break;
        case 'j':
        case 's':
            board_move_cursor(board, 0, 1);
            break;
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            board_try_set_cell(board, board->cursor_pos[0], board->cursor_pos[1], input - '0');
            break;
        case ' ':
            board_try_set_cell(board, board->cursor_pos[0], board->cursor_pos[1], 0);
            break;

    }
}

