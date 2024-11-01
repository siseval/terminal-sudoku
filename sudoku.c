#include "sudoku.h"
#include <curses.h>


static void init_curses();
static void main_loop(struct board* board);
static void handle_input(struct board* board);
static void end_game(int board_box_size, int num_clues);
static void main_menu();
static void quit();

int main()
{
    init_curses();        
    srand(time(NULL));

    main_menu();

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

void sudoku_run(int board_box_size, int num_clues)
{
    struct board* board = board_create(board_box_size);
    board_generate_puzzle(board, num_clues);
    main_loop(board);
}

static void quit(void)
{
    nocbreak();
    endwin();
    exit(0);
}

static void main_loop(struct board* board)
{
    board_print(board);
    while (true)
    {
        handle_input(board);
        board_print(board);
        if (board_is_solved(board))
        {
            end_game(board->box_width, board->num_clues);
            board_destroy(board);
            return;
        }
    }
}

static void main_menu()
{
    struct cli_button small_button = {"Small (4x4)"};
    struct cli_button standard_button = {"Standard (9x9)"};
    struct cli_button quit_button = {"Quit"};

    struct cli_menu size_menu = {"=: Select Board Size :=", ":: ", " ::", 2, 1, true, false, 3, 0, small_button, standard_button, quit_button};

    int size_menu_gaps[] = { 4, 2, 3};

    int size_selection = cli_menu_run(&size_menu, size_menu_gaps, -2, true);

    if (size_selection == 2)
    {
        quit();
    }

    char easy_button_str[32];
    char medium_button_str[32];
    char hard_button_str[32];

    int num_clues_map[2][3] = { { 8, 6, 4 }, { 40, 35, 30 } };

    sprintf(easy_button_str, "Easy (%d clues)", num_clues_map[size_selection][0]);
    sprintf(medium_button_str, "Medium (%d clues)", num_clues_map[size_selection][1]);
    sprintf(hard_button_str, "Hard (%d clues)", num_clues_map[size_selection][2]);

    struct cli_button easy_button = { "" };
    struct cli_button medium_button = { "" };
    struct cli_button hard_button = { "" };
    struct cli_button return_button = { "Return" };

    strcpy(easy_button.text, easy_button_str);
    strcpy(medium_button.text, medium_button_str);
    strcpy(hard_button.text, hard_button_str);

    struct cli_menu difficulty_menu = {"=: Select Difficulty :=", ":: ", " ::", 2, 1, true, false, 4, 0, easy_button, medium_button, hard_button, return_button};

    int difficulty_menu_gaps[] = { 4, 2, 2, 3};

    int difficulty_selection = cli_menu_run(&difficulty_menu, difficulty_menu_gaps, 0, true);

    if (difficulty_selection == 3)
    {
        main_menu();
        return;
    }

    sudoku_run(2 + size_selection, num_clues_map[size_selection][difficulty_selection]);
}

static void end_game(int board_box_size, int num_clues)
{
    clear(); 

    int num_labels = 1;
    struct cli_button time_label = { "Time: 0:00:00" };
    struct cli_menu end_screen = { "=: You Win :=", "- ", " -", 1, 1, true, false, num_labels, 0, time_label };

    int label_gaps[] = { 4 };
    
    cli_menu_display_labels(end_screen, label_gaps, -5, true);

    int num_buttons = 3;
    struct cli_button restart_button = { "Restart" };
    struct cli_button menu_button = { "Main Menu" };
    struct cli_button quit_button = { "Quit" };
    struct cli_menu end_menu = { "", ":: ", " ::", 2, 1, true, false, num_buttons, 0, restart_button, menu_button, quit_button };

    int menu_gaps[] = { 3, 2, 2 };

    int selection = cli_menu_run(&end_menu, menu_gaps, 2, false);

    if (selection == 2)
    {
        quit();
    }

    selection == 0 ? sudoku_run(board_box_size, num_clues) : main_menu();
}

static void handle_input(struct board* board)
{
    char input = getch();
    switch (input)
    {
        case 'q':
            board_destroy(board);
            main_menu();
            break; 
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

