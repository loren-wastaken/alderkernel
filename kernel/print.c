#include "headers/print.h"
#include "headers/colors.h"
#include "headers/screen.h"
#include "headers/io.h"

#define SCREEN_SIZE (SCREEN_WIDTH * SCREEN_HEIGHT)

// keep track of where we are typing on the screen
static unsigned int cursor_index = 0;

// VGA text-mode CRT controller ports
#define CRT_CTRL_PORT 0x3D4
#define CRT_DATA_PORT 0x3D5

// move the blinking hardware cursor to match cursor_index
static void update_cursor(void)
{
    outb(CRT_CTRL_PORT, 14);
    outb(CRT_DATA_PORT, (cursor_index >> 8) & 0xFF);
    outb(CRT_CTRL_PORT, 15);
    outb(CRT_DATA_PORT, cursor_index & 0xFF);
}

// shift every row up by one line, blank the new last line, and pull
// the cursor back onto that last line - called instead of wrapping
// back to index 0 whenever we run off the bottom of the screen
static void scroll_screen(void)
{
    unsigned short* buffer = VIDEO_MEMORY;
    unsigned short blank = ' ' | (WHITE << 8);

    for (unsigned int i = 0; i < SCREEN_SIZE - SCREEN_WIDTH; i++) {
        buffer[i] = buffer[i + SCREEN_WIDTH];
    }

    for (unsigned int i = SCREEN_SIZE - SCREEN_WIDTH; i < SCREEN_SIZE; i++) {
        buffer[i] = blank;
    }

    cursor_index -= SCREEN_WIDTH;
}

// call after cursor_index may have run past the end of the screen
static void advance_cursor(void)
{
    while (cursor_index >= SCREEN_SIZE) {
        scroll_screen();
    }
}

void print_text(const char* str) {
    unsigned short* buffer = VIDEO_MEMORY;

    for (int i = 0; str[i] != '\0'; i++) {
        // handle newline char
        if (str[i] == '\n') {
            cursor_index = ((cursor_index / SCREEN_WIDTH) + 1) * SCREEN_WIDTH;
            advance_cursor();
            continue;
        }

        // combine char byte and color byte into 16 bits
        buffer[cursor_index] = str[i] | (WHITE << 8);
        cursor_index++;

        advance_cursor();
    }

    update_cursor();
}

void print_char(char c)
{
    unsigned short* buffer = VIDEO_MEMORY;

    if (c == '\n') {
        cursor_index = ((cursor_index / SCREEN_WIDTH) + 1) * SCREEN_WIDTH;
        advance_cursor();
        update_cursor();
        return;
    }

    if (c == '\b') {
        if (cursor_index > 0) {
            cursor_index--;
            buffer[cursor_index] = ' ' | (WHITE << 8);
        }
        update_cursor();
        return;
    }

    buffer[cursor_index] = c | (WHITE << 8);
    cursor_index++;

    advance_cursor();
    update_cursor();
}

// print an unsigned integer in decimal
void print_uint(unsigned int n)
{
    char buf[11];
    int i = 10;
    buf[10] = '\0';

    if (n == 0) {
        print_char('0');
        return;
    }

    while (n > 0 && i > 0) {
        i--;
        buf[i] = '0' + (n % 10);
        n /= 10;
    }

    print_text(&buf[i]);
}

// print an unsigned integer as 0xAABBCCDD
void print_hex(unsigned int n)
{
    const char* hex_digits = "0123456789ABCDEF";
    char buf[9];
    buf[8] = '\0';

    for (int i = 7; i >= 0; i--) {
        buf[i] = hex_digits[n & 0xF];
        n >>= 4;
    }

    print_text("0x");
    print_text(buf);
}

void clear_screen_reset_cursor(void)
{
    cursor_index = 0;
    update_cursor();
}
