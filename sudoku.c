#include "sudoku.h"
#include <curses.h>


static void init_curses();
static void main_loop(struct board* board);
static void handle_input(struct board* board);
static void end_game(int board_box_size);

int main()
{
    init_curses();        
    srand(time(NULL));

    sudoku_run(3);

    endwin();
}

static void init_curses(void)
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

void sudoku_run(int board_box_size)
{
    struct board* board = board_create(board_box_size);
    main_loop(board);
}

static void main_loop(struct board* board)
{
    board_generate_puzzle(board, 15);
    board_print(board);
    while (true)
    {
        handle_input(board);
        board_print(board);
        if (board_is_solved(board))
        {
            end_game(board->box_width);
            board_destroy(board);
            return;
        }
    }
}

static void end_game(int board_box_size)
{
    clear(); 

    int num_labels = 1;
    struct cli_button time_label = { "Time: 0:00:00" };
    struct cli_menu end_screen = { "=: You Win :=", "- ", " -", 1, 1, true, false, num_labels, 0, time_label };

    int label_gaps[] = { 3 };
    
    cli_menu_display_labels(end_screen, label_gaps, -cli_menu_get_height(end_screen, label_gaps), true);

    int num_buttons = 3;
    struct cli_button restart_button = { "Restart" };
    struct cli_button menu_button = { "Main Menu" };
    struct cli_button quit_button = { "Quit" };
    struct cli_menu end_menu = { "", ":: ", " ::", 2, 1, true, false, num_buttons, 0, restart_button, menu_button, quit_button };

    int menu_gaps[] = { 3, 2, 2 };

    int selection = cli_menu_run(&end_menu, menu_gaps, 3, false);

    if (selection == 2)
    {
        exit(0);
    }

    selection == 0 ? sudoku_run(board_box_size) : sudoku_run(board_box_size);
}

static void handle_input(struct board* board)
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

