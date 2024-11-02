#include "cli.h"

void cli_menu_button_draw(const struct cli_button button, const struct cli_menu menu, const int col)
{
    attron(COLOR_PAIR(col));
  
    cli_move_center_h(-((int)(strlen(button.text) + strlen(menu.left) + strlen(menu.right)) / 2));
    printw("%s%s%s\n\r", menu.left, button.text, menu.right);

    attroff(COLOR_PAIR(col));
}

void cli_menu_draw_buttons(const struct cli_menu menu, const int gaps[])
{
    for (int i = 0; i < menu.num_buttons; i++) 
    {
        const struct cli_button b = menu.buttons[i];
        for (int j = 0; j < gaps[i]; j++) 
        {
            printw("\n");
        }
        cli_menu_button_draw(b, menu, menu.selected == i ? menu.selected_col : menu.normal_col);
    }
}

int cli_menu_run(struct cli_menu *menu, const int gaps[], const int dy, const bool clear_screen)
{
    menu->bold ? attron(A_BOLD) : attroff(A_BOLD);
    int selection = menu->selected;
    while (!menu->has_selected)
    {
        if (clear_screen)
        {
            clear();
        }
        cli_move_center_v(-(cli_menu_get_height(*menu, gaps) / 2) + dy);
        cli_move_center_h(-(strlen(menu->top_text) / 2));

        printw("%s", menu->top_text);
        cli_menu_draw_buttons(*menu, gaps);

        selection = cli_menu_handle_input(menu);
    }
    attroff(A_BOLD);
    return selection;
}

void cli_menu_display_labels(const struct cli_menu menu, const int gaps[], const int dy, const bool clear_screen)
{
    menu.bold ? attron(A_BOLD) : attroff(A_BOLD);

    if (clear_screen)
    {
        clear();
    }

    cli_move_center_v(-(cli_menu_get_height(menu, gaps) / 2) + dy);
    cli_move_center_h(-(strlen(menu.top_text) / 2));

    printw("%s", menu.top_text);
    cli_menu_draw_buttons(menu, gaps);
}

int cli_menu_get_height(const struct cli_menu m, const int gaps[])
{
    int height = m.num_buttons + 1;
    for (int i = 0; i < m.num_buttons; i++)
    {
        height += gaps[i];
    }
    return height;
}

struct cli_button cli_menu_get_button(const struct cli_menu menu, const int index) 
{
    return menu.buttons[index]; 
}

int cli_menu_handle_input(struct cli_menu *menu) 
{
    switch (getch()) 
    {
    case 'k':
    case 'w':
        menu->selected -= menu->selected <= 0 ? 0 : 1;
        break;

    case 'j':
    case 's':
        menu->selected += menu->selected >= menu->num_buttons - 1 ? 0 : 1;
        break;

    case ' ':
    case 'f':
    case K_ENTER:
        menu->has_selected = true;
        break;
    }
    return menu->selected;
}

int cli_get_cur_x()
{
    int y, x;
    getyx(stdscr, y, x);
    return x;
}

int cli_get_cur_y(void)
{
    int y, x;
    getyx(stdscr, y, x);
    return y;
}

int cli_get_scrw(void)
{
    int scrh, scrw;
    getmaxyx(stdscr, scrh, scrw);
    return scrw;
}

int cli_get_scrh(void)
{
    int scrh, scrw;
    getmaxyx(stdscr, scrh, scrw);
    return scrh;
}

void cli_move_center_v(const int dy)
{
    move(cli_get_scrh() / 2 + dy, cli_get_cur_x());
}

void cli_move_center_h(const int dx)
{
    move(cli_get_cur_y(), cli_get_scrw() / 2 + dx);
}
