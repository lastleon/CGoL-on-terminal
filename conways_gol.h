#ifndef CONWAY_H
#define CONWAY_H

/* MISC SHORTCUTS */
#define cursup "\033[A"
#define cursdown "\033[B"
#define cursright "\033[C"
#define cursleft "\033[D"
#define clearline "\033[K"

#define ESCAPE 27
#define ENTER 10

#define ANSI_COLOR_RED     "\x1b[31m"
/*#define ANSI_COLOR_RED_FULL "\x1b[31;41m"*/
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_RESET   "\x1b[0m"

#define ANSI_SLOW_BLINK "\x1b[5m"
#define ANSI_COLOR_INVERT "\033[7m"

#define DISABLE_CURSOR_BLINK "\033[?12l"
#define ENABLE_CURSOR_BLINK "\033[?12h"

#define MAX_BOARD_SIZE 1000

void signalhandler(int siging);

/* helper functions */
void print_help();
int is_p_int(char* s);
int min(int a, int b);
int max(int a, int b);


#endif