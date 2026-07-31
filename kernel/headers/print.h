#ifndef PRINT_H
#define PRINT_H

#include "colors.h"
#include "screen.h"

#define VIDEO_MEMORY ((unsigned short*)0xB8000)

void print_text(const char* str);
void clear_screen(unsigned char color);

#endif
