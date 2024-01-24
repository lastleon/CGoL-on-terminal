#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <termios.h>

#include "conways_gol.h"
#include "board.h"
#include "utils.h"

/* original terminal settings */
struct termios orig_term;
board_config config;

int main(int argc, char *argv[])
{
        int i;
        board b;

        /***** INIT *****/

        /* signalhandler */
        signal(SIGABRT, signalhandler);
        signal(SIGINT, signalhandler);
        signal(SIGTERM, signalhandler);

        srand(time(NULL));

        /* sets terminal to custom raw mode */
        enable_raw_mode();
        atexit(reset_terminal);

        config = init_config();

        /* command-line argument handling */
        if (argc > 1) {
                /* if first argument is not a flag, the first two arguments must be dimensions of the board */
                if (argv[1][0] != '-') {
                        if (argc >= 3 && is_p_int(argv[1]) && is_p_int(argv[2])) {
                                config->rows = min(MAX_BOARD_SIZE, atoi(argv[1]));
                                config->cols = min(MAX_BOARD_SIZE, atoi(argv[2]));
                                i = 3;
                        } else {
                                fprintf(stderr, "If the first argument is not a flag the first two arguments must be positive integers, representing the rows and columns of the board respectively.\n");
                                exit(EXIT_FAILURE);
                        }
                } else {
                        i = 1;
                }
                /* other flags are parsed */
                for (; i<argc; i++) {
                        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
                                if (argc > 2) {
                                        printf("To see the help page, please use '-h'/'--help' alone.\n");
                                } else {
                                        print_help();
                                        exit(EXIT_SUCCESS);
                                }
                        } else if (strcmp(argv[i], "-r") == 0 || strcmp(argv[i], "--random") == 0) {
                                config->mode = RANDOM;

                        } else if (strcmp(argv[i], "-c") == 0 || strcmp(argv[i], "--custom") == 0) {
                                config->mode = CUSTOM;

                        } else if (strcmp(argv[i], "-x") == 0) {
                                config->skin = XES;

                        } else if (strcmp(argv[i], "-b") == 0) {
                                config->skin = BRACKETS;

                        } else {
                                if (argv[i][0] == '-') {
                                        fprintf(stderr, "You used the following unsupported flag: '%s'\nPlease use '-h' for instructions on how to use this utility.\n", argv[i]);
                                        exit(EXIT_FAILURE);
                                } else {
                                        fprintf(stderr, "The argument '%s' is illegal in this position.\nPlease use '-h' for instructions on how to use this utility.\n", argv[i]);
                                        exit(EXIT_FAILURE);
                                }
                        }
                }
        }

        b = init_board(config);
        if (!b) {
                exit(EXIT_FAILURE);
        }

        /***** MAIN LOOP *****/

        while (1) {
                update(b);
                draw(b);
                usleep(100*1000);
        }

        return 0;
}



/* editor functions */

void enable_raw_mode(void)
{
        struct termios raw;
        tcgetattr(STDIN_FILENO, &orig_term);

        raw = orig_term; /* this is a COPY, raw does not point to orig_term */

        raw.c_lflag &= ~(ECHO | ICANON);

        tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

void reset_terminal(void)
{
        tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_term);
}

void signalhandler(int signum)
{
        reset_terminal();
        exit(1);
}


/* helper functions */

void print_help(void)
{
	printf("\nThis program runs Conway's Game of Life on the commandline.\n\n");
	printf("USAGE:\n\t./cgol {rows} {cols} [-h] [-r | -c] [-x | -b]\n\n");
	printf("PARAMETERS:\n");
	printf("\trows:\tNumber of rows the board will have. Must be positive integer. Default: %d\n", DEFAULT_ROWS);
	printf("\tcols:\tNumber of columns the board will have. Must be positive integer. Default: %d\n", DEFAULT_COLS);
	printf("\nOPTIONS:\n");
	printf("\t-h:\tShow this help page.\n");
	printf("\t-r:\tStart game with random initialization. (default)\n");
	printf("\t-c:\tStart game with custom initialization. You will be able to edit the board.\n");
	printf("\t-x:\tBoard will have skin 'X' (default)\n");
	printf("\t-b:\tBoard will have skin 'brackets'\n\n");
}

int is_p_int(char *s)
{
        char c;
        int i = 0;

        /* is positive */
        int p = 0;

        while ((c = s[i++]) != '\0') {
                if (!isdigit(c))
                        return 0;
                if ( c != '0')
                        p = 1;
        }
        /* if string is empty, p will be zero => returns 0 on empty string */
        return p;
}

int min(int a, int b)
{
        if (a < b) 
                return a;
        return b;
}

int max(int a, int b)
{
        if (a > b) 
                return a;
        return b;
}
