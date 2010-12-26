#include <screen/screen.h>
#include <terminal/terminal.h>
#include <stdlib.h>
#include <mm/virt_mm.h>
#include <lib/io.h>

//The VGA Frame buffer starts at 0xB8000
static uint16* video_memory_location = (uint16*) (KERNEL_START + 0xB8000);

static unsigned int cursor_x = 0;
static unsigned int cursor_y = 0;

static terminal_t* current_terminal = 0;

void text_mode_set_x(uint8 x) 
{
	cursor_x = x;
	if (current_terminal)
		current_terminal->m_cursorX = x;
}

void text_mode_set_y(uint8 y) 
{
	cursor_y = y;
	if (current_terminal)
		current_terminal->m_cursorY = y;
}

static const uint8 active_foreground_col = 15;
static const uint8 active_background_col = 0;

static void text_mode_move_cursor() 
{
    // The screen is 80 characters wide...
    uint16 cursorLocation = cursor_y * 80 + cursor_x;

    outb(0x3D4, 14);                  // Tell the VGA board we are setting the high cursor byte.
    outb(0x3D5, cursorLocation >> 8); // Send the high cursor byte.

    outb(0x3D4, 15);                  // Tell the VGA board we are setting the low cursor byte.
    outb(0x3D5, cursorLocation);      // Send the low cursor byte.

}

void text_mode_scroll_prec(uint16* video_memory, unsigned char bgc, unsigned char fgc, unsigned int* cx, unsigned int* cy)
{
    // Get a space character with the default colour attributes.
    uint8 attributeByte = (bgc << 4) | (fgc & 0x0F);
    uint16 blank = 0x20 /* space */ | (attributeByte << 8);

    // Row 25 is the end, this means we need to scroll up
    if(*cy >= 25)
    {
        // Move the current text chunk that makes up the screen
        // back in the buffer by a line
        int i;
        for (i = 0*80; i < 24*80; i++)
        {
            video_memory[i] = video_memory[i+80];
        }

        // The last line should now be blank. Do this by writing
        // 80 spaces to it.
        for (i = 24*80; i < 25*80; i++)
        {
            video_memory[i] = blank;
        }

        // The cursor should now be on the last line.
        *cy = 24;
    }
}

void text_mode_putc_prec(uint16* video_memory, uint8 bgc, uint8 fgc, unsigned int* cx, unsigned int* cy, char c)
{
    // The attribute byte is made up of two nibbles - the lower being the 
    // foreground colour, and the upper the background colour.
    uint8  attributeByte = (bgc << 4) | (fgc & 0x0F);

    // The attribute byte is the top 8 bits of the word we have to send to the
    // VGA board.
    uint16 attribute = attributeByte << 8;
    uint16 *location = 0;

    // Handle a backspace, by moving the cursor back one space
    if (c == 0x08 && *cx)
    {
        *cx = *cx - 1;
    }

    // Handle a tab by increasing the cursor's X, but only to a point
    // where it is divisible by 8.
    else if (c == 0x09)
    {
        *cx = (*cx + 8) & ~(8-1);
    }

    // Handle carriage return
    else if (c == '\r')
    {
        *cx = 0;
    }

    // Handle newline by moving cursor back to left and increasing the row
    else if (c == '\n')
    {
        *cx = 0;
        *cy = *cy + 1;
    }
    // Handle any other printable character.
    else if(c >= ' ')
    {
        location = video_memory + ( (*cy) * 80 + (*cx));
        *location = c | attribute;
        *cx = *cx + 1;
    }

    // Check if we need to insert a new line because we have reached the end
    // of the screen.
    if (*cx >= 80)
    {
        *cx = 0;
        *cy = *cy + 1;
    }

    text_mode_scroll_prec(video_memory, bgc, fgc, cx, cy);
}

void text_mode_clearscreen_prec(uint16* video_memory, uint8 bgc, uint8 fgc, unsigned int* cx, unsigned int* cy) 
{
    // Make an attribute byte for the default colours
    uint8 attributeByte = (bgc /*black*/ << 4) | (fgc /*white*/ & 0x0F);
    uint16 blank = 0x20 /* space */ | (attributeByte << 8);

    int i;
    for (i = 0; i < 80*25; i++)
    {
        video_memory[i] = blank;
    }

    // Move the hardware cursor back to the start.
    *cx = 0;
    *cy = 0;
}

void text_mode_clearscreen() 
{
    text_mode_clearscreen_prec(video_memory_location, active_background_col, active_foreground_col, &cursor_x, &cursor_y);
    text_mode_move_cursor();
}

void text_mode_set_fg_color(uint8 col) 
{
}

void text_mode_set_bg_color(uint8 col) 
{
}

void text_mode_putc(char c) 
{
    text_mode_putc_prec(video_memory_location, active_background_col, active_foreground_col, &cursor_x, &cursor_y, c);

    // Move the hardware cursor.
    text_mode_move_cursor();
}

void text_mode_hardwrite(const char * Array) 
{
	unsigned int i = 0;
	while (Array[i]) {
		text_mode_putc(Array[i++]);	
	}
}

void backup_terminal(terminal_t* term)
{
	memcpy(term->m_backupData, video_memory_location, sizeof(uint16) * term->m_width * term->m_height);
}

void text_mode_tputc(terminal_t* term, char c)
{
	text_mode_putc_prec(video_memory_location, term->m_backgroundColour, term->m_foregroundColour, &cursor_x, &cursor_y, c);
	term->m_cursorX = cursor_x;
	term->m_cursorY = cursor_y;

	text_mode_move_cursor();
}

void text_mode_tsetfg(terminal_t* term, uint8 col)
{
	text_mode_set_fg_color(col);
}

void text_mode_tsetbg(terminal_t* term, uint8 col)
{
	text_mode_set_bg_color(col);
}

void text_mode_tclear(terminal_t* term)
{
    text_mode_clearscreen_prec(video_memory_location, term->m_backgroundColour, term->m_foregroundColour, &cursor_x, &cursor_y);
    text_mode_move_cursor();
}

void text_mode_tup(terminal_t* term)
{
	cursor_x = term->m_cursorX;
	cursor_y = term->m_cursorY;

	text_mode_move_cursor();
}

void text_mode_sett(terminal_t* term)
{
	if (current_terminal != 0)
		backup_terminal(current_terminal);

	current_terminal = 0;
	term->m_used = 1;

	if (term->m_height != 25 || term->m_width != 80)
	{
		//Incompatable with this terminal
		current_terminal = 0;
		text_mode_clearscreen();
		return; //Crash
	}

	text_mode_clearscreen();
	
	//Restore from backed up data
	memcpy(video_memory_location, term->m_backupData, sizeof(uint16) * term->m_width * term->m_height);

	text_mode_tup(term);

	current_terminal = term;
}
