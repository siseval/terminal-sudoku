#ifndef MENU_H
#define MENU_H

#include <curses.h>
#include <stdlib.h>
#include <string.h>

#define K_ENTER 10

struct cli_button 
{
    char text[32];
};

struct cli_menu 
{
    char top_text[32];
    char left[32];
    char right[32];
    int selected_col;
    int normal_col;
    bool bold;     
    bool has_selected;
    int num_buttons;
    int selected;
    struct cli_button buttons[20];
};

struct cli_button cli_menu_get_button(const struct cli_menu menu, const int index);

void cli_menu_button_draw(const struct cli_button button, const struct cli_menu menu, const int col);
void cli_menu_draw_buttons(const struct cli_menu menu, const int gaps[]);
int cli_menu_run(struct cli_menu *menu, const int gaps[], const int dy, const bool clear_screen);

void cli_menu_display_labels(const struct cli_menu menu, const int gaps[], const int dy, const bool clear_screen);

int cli_menu_get_height(const struct cli_menu menu, const int gaps[]);

int cli_menu_handle_input(struct cli_menu *menu);

int cli_get_cur_x();
int cli_get_cur_y();
int cli_get_scrw();
int cli_get_scrh();
void cli_move_center_v(int dy);
void cli_move_center_h(int dx);

#endif
