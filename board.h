#ifndef BOARD_H
#define BOARD_H

#define DEFAULT_ROWS 30
#define DEFAULT_COLS 30

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "conways_gol.h"

typedef enum {RANDOM, CUSTOM} board_mode;
typedef enum {XES, BRACKETS} board_skin;

typedef int ** board_state;

typedef struct _config {
    int rows;
    int cols;

    board_mode mode;
    board_skin skin;
} * board_config;

typedef struct _board {
    board_state state;
    board_state next_state;

    board_config config;
} * board;


/* config functions */
board_config init_config(void);

/* board state functions */
board init_board(board_config config);

board_state random_board(int rows, int cols);
board_state board_editor(int rows, int cols, board_skin skin);
board_state zeros_board(int rows, int cols);

/** board printing **/
void draw(board b);

/* normal board printing */
static void skin_print(unsigned int a, board_skin skin);
static void skin_print_curs(unsigned int a, int x, int y, board_skin skin);
static void draw_edit_board(board_state state, int rows, int cols, int x, int y, board_skin skin);

void update(board b);


int alive(board b, int i, int j);
int live_neighbours(board b, int i, int j);

/* editor functions */
void enable_raw_mode(void);
void reset_terminal(void);


#endif