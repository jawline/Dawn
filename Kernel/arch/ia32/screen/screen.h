#ifndef _SCREEN_DEF_H_
#define _SCREEN_DEF_H_
#include <types/stdint.h>

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

/**
 * Initialize the screen if one exists
 */
void initializeScreen();

/**
 * Get the current screen mode
 */
unsigned int getScreenMode();

/**
 * Get a screen attribute
 */
unsigned int getScreenAttribute(unsigned int ID)

void text_mode_clearscreen();
void text_mode_putc(char Character);
void text_mode_write(const char * Array);
void text_mode_set_fg_color(uint8_t col);
void text_mode_set_x(uint8_t x);
void text_mode_set_y(uint8_t y);

#endif //_SCREEN_DEF_H_
