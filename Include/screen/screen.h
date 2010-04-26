#ifndef _SCREEN_DEF_H_
#define _SCREEN_DEF_H_
#include <types/int_types.h>

//Mode definitions
#define MODE_TEXTMODE 0

//Attribute definitions
#define SCREEN_MODE 0
#define SCREEN_WIDTH 1
#define SCREEN_HEIGHT 2

#define WHITE 15
#define BLACK 0
#define RED 4
#define BROWN 6
#define GREEN 2

void init_screen();
unsigned int get_attr(unsigned int ID);

void text_mode_clearscreen();
void text_mode_putc(char Character);
void text_mode_write(const char * Array);
void text_mode_set_fg_color(uint8 col);
void text_mode_set_x(uint8 x);
void text_mode_set_y(uint8 y);

#endif //_SCREEN_DEF_H_
