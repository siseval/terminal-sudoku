#ifndef MENU_H
#define MENU_H

#include <curses.h>
#include <stdlib.h>
#include <string.h>

#define K_ENTER 10

struct cli_button 
{
    char *text;
};

struct cli_menu 
{
    char* top_text;
    char *left;
    char *right;
    int selected_col;
    int normal_col;
    bool bold;     
    bool has_selected;
    int num_buttons;
    int selected;
    struct cli_button buttons[20];
};

struct cli_button cli_menu_get_button(struct cli_menu menu, int index);

void cli_menu_button_draw(struct cli_button button, struct cli_menu menu, int col);
void cli_menu_draw_buttons(struct cli_menu menu, const int gaps[]);
int cli_menu_display(struct cli_menu *menu, int gaps[], int dy, bool clear_screen);

void cli_menu_display_labels(struct cli_menu menu, int gaps[], int dy, bool clear_screen);

int cli_menu_get_height(struct cli_menu menu, int gaps[]);

int cli_menu_handle_input(struct cli_menu *menu);

int cli_get_cur_x();
int cli_get_cur_y();
int cli_get_scrw();
int cli_get_scrh();
void cli_move_center_v(int dy);
void cli_move_center_h(int dx);

#endif
