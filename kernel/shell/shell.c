#include "headers/commands.h"
#include "headers/interpreter.h"
#include "headers/util.h"
#include "../headers/print.h"
#include "../drivers/ps2.h"
#include "../fs/vfs.h"

#define SHELL_BUFFER_SIZE 128

static char shell_buffer[SHELL_BUFFER_SIZE];

// --- command history ---------------------------------------------
#define HISTORY_SIZE 16

static char history[HISTORY_SIZE][SHELL_BUFFER_SIZE];
static unsigned int history_count = 0; // entries stored so far (caps at HISTORY_SIZE)
static unsigned int history_next  = 0; // next ring-buffer slot to write

// -1 = not currently browsing history (on the "live" line)
// 0..history_count-1 = how far back we've scrolled (0 = most recent)
static int history_browse_index = -1;

// what the user had typed before they started pressing Up, so Down
// can bring them back to it
static char saved_current[SHELL_BUFFER_SIZE];

static void history_add(const char* cmd)
{
    if (cmd[0] == '\0') {
        return; // don't store empty lines
    }

    str_copy(history[history_next], cmd);

    history_next = (history_next + 1) % HISTORY_SIZE;
    if (history_count < HISTORY_SIZE) {
        history_count++;
    }
}

// back = 0 -> most recently run command, 1 -> the one before that, etc.
// returns NULL if there's nothing that far back.
static const char* history_get(unsigned int back)
{
    if (back >= history_count) {
        return (const char*)0;
    }

    unsigned int idx = (history_next + HISTORY_SIZE - 1 - back) % HISTORY_SIZE;
    return history[idx];
}

// erases `old_len` characters before the cursor, writes new_content
// in their place (both on screen and into shell_buffer), and returns
// the new line length
static unsigned int redraw_line(unsigned int old_len, const char* new_content)
{
    for (unsigned int i = 0; i < old_len; i++) {
        print_char('\b');
    }

    unsigned int i = 0;
    while (new_content[i] != '\0' && i < SHELL_BUFFER_SIZE - 1) {
        shell_buffer[i] = new_content[i];
        print_char(new_content[i]);
        i++;
    }
    shell_buffer[i] = '\0';

    return i;
}

static void print_prompt(void)
{
    print_text("\nzSlash ");
    vfs_print_cwd_path();
    print_text("> ");
}



// shell main loop - never returns
void shell_start()
{
    unsigned int idx = 0;

    print_text("\nWelcome to zSlash.\nType 'help' for a list of commands.\n");
    print_prompt();

    while (1) {
        char c = key_buffer_pop();

        if (c == 0) {
            // nothing typed yet - sleep until the next interrupt
            // (keyboard IRQ will wake us right back up here)
            asm volatile("hlt");
            continue;
        }

        if (c == KEY_ARROW_UP) {
            if (history_browse_index == -1) {
                // just starting to browse - stash the in-progress line
                shell_buffer[idx] = '\0';
                str_copy(saved_current, shell_buffer);
            }

            if ((unsigned int)(history_browse_index + 1) < history_count) {
                history_browse_index++;
                const char* entry = history_get((unsigned int)history_browse_index);
                idx = redraw_line(idx, entry);
            }
            continue;
        }

        if (c == KEY_ARROW_DOWN) {
            if (history_browse_index > 0) {
                history_browse_index--;
                const char* entry = history_get((unsigned int)history_browse_index);
                idx = redraw_line(idx, entry);
            } else if (history_browse_index == 0) {
                history_browse_index = -1;
                idx = redraw_line(idx, saved_current);
            }
            continue;
        }

        if (c == '\n') {
            print_char('\n');
            shell_buffer[idx] = '\0';
            history_add(shell_buffer);
            history_browse_index = -1;
            interpret_command(shell_buffer);
            idx = 0;
            print_prompt();
            continue;
        }

        if (c == '\b') {
            if (idx > 0) {
                idx--;
                print_char('\b');
            }
            continue;
        }

        // drop input past buffer capacity instead of overflowing
        if (idx < SHELL_BUFFER_SIZE - 1) {
            shell_buffer[idx++] = c;
            print_char(c);
        }
    }
}



// command: help
void command_help(void)
{
    print_text(
        "Available commands:\n"
        "help                       - show this list\n"
        "clear                      - clear the screen\n"
        "about                      - information about AlderKernel\n"
        "echo <text>                - print text\n"
        "uname [-a|-s|-r|-m]        - show system info\n"
        "req-syscallop <number>     - request a syscall operation (stub)\n"
        "memtest                    - run a simple memory test\n"
        "ls [path]                  - list a directory\n"
        "cd [path]                  - change directory\n"
        "pwd                        - print working directory\n"
        "cat <file>                 - print a file's contents\n"
        "halt                       - stop the CPU\n"
        "\n"
        "Press Up/Down to recall previous commands.\n"
    );
}



// command: clear
void command_clear(void)
{
    clear_screen(0);
}



// command: about
void command_about(void)
{
    print_text(
        "AlderKernel\n"
        "A small hobby operating system kernel.\n"
        "Shell: zSlash\n"
        "Made by loren-wastaken\n"
    );
}



// command: echo
void command_echo(char* args)
{
    print_text(args);
    print_text("\n");
}



// command: halt
void command_halt(void)
{
    print_text("CPU halted.\n");

    asm volatile("cli");

    while (1) {
        asm volatile("hlt");
    }
}
