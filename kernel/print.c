#include "headers/print.h"
#include "headers/colors.h"
#include "headers/screen.h"

// keep track of where we are typing on the screen
static unsigned int cursor_index = 0;

void print_text(const char* str) {
    unsigned short* buffer = VIDEO_MEMORY;

    for (int i = 0; str[i] != '\0'; i++) {
        // handle newline char
        if (str[i] == '\n') {
            // move cursor to the start of the next row
            cursor_index = ((cursor_index / SCREEN_WIDTH) + 1) * SCREEN_WIDTH;
            continue;
        }

        // combine char byte and color byte into 16 bits
        buffer[cursor_index] = str[i] | (WHITE << 8);
        cursor_index++;

        // prevent writing past the screen boundary (SCREEN_HEIGHT * SCREEN_WIDTH = 2000)
        if (cursor_index >= 2000) {
            cursor_index = 0; // wrap around to top 4 now
        }
    }
}