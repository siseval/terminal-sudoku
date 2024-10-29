#ifndef MENU_H
#define MENU_H

#include <curses.h>
#include <stdlib.h>
#include <string.h>

#define K_ENTER 10

struct button 
{
    char *text;
};

struct menu 
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
    struct button buttons[20];
};

struct button get_button(struct menu m, int i);

void draw_button(struct button b, struct menu m, int col);
void draw_buttons(struct menu m, const int gaps[]);
int do_menu(struct menu *m, int gaps[], int dy, bool clear_screen);

void print_as_labels(struct menu m, int gaps[], int dy, bool clear_screen);

int get_height(struct menu m, int gaps[]);

int menu_input(struct menu *m);

int get_cur_x();
int get_cur_y();
int get_scrw();
int get_scrh();
void move_center_v(int dy);
void move_center_h(int dx);

#endif
