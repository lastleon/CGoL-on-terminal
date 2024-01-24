#include "board.h"
#include "utils.h"

/* board functions */

board init_board(board_config config)
{
        NULLCHECK(config);

        board b = malloc(sizeof(board));
        NULLCHECK(b);

        switch (config->mode) {
                default:
                case RANDOM:
                        b->state = random_board(config->rows, config->cols);
                        break;
                case CUSTOM:
                        b->state = board_editor(config->rows, config->cols, config->skin);
                        break;
        }

        b->next_state = zeros_board(config->rows, config->cols);
        b->config = config;

        /* if state-init fails, the allocated board must be cleaned up */
        if (!(b->state) || !(b->next_state)) {
                /* doesn't matter which one failed */
                free(b->state);
                free(b->next_state);
                free(b);
                return NULL;
        }

        return b;
}

board_config init_config(void)
{
        board_config config = malloc(sizeof(board_config));
        NULLCHECK(config);

        config->rows = DEFAULT_ROWS;
        config->cols = DEFAULT_COLS;
        config->mode = RANDOM;
        config->skin = XES;

        return config;
}

board_state random_board(int rows, int cols)
{
        int i, j;
        board_state state;

        state = malloc(rows * sizeof(int*));
        NULLCHECK(state);

        for (i=0; i<rows; i++) {
                state[i] = malloc(cols * sizeof(int));

                /* if any allocation fails, all of the previous ones have to be cleaned up */
                if (!state[i]) {
                        for (; i>=0; i--) {
                                free(state[i]);
                        }
                        return NULL;
                }

                for (j=0; j<cols; j++) {
                        state[i][j] = rand() % 100 >= 50 ? 1 : 0;
                }
        }

        return state;
}

board_state board_editor(int rows, int cols, board_skin skin)
{
        char c;
        int status = 0;

        int curs_pos_i = 3;
        int curs_pos_j = 3;

        board_state state = zeros_board(rows, cols);
        NULLCHECK(state);

        draw_edit_board(state, rows, cols, curs_pos_i, curs_pos_j, skin);
        
        /* event handling and drawing */
        while (read(STDIN_FILENO, &c, 1) == 1 && c != 'q') {
                /* handle events */
                if (c == ESCAPE) {
                        status = read(STDIN_FILENO, &c, 1);
                        /* is escape-sequence */
                        if (status != 0 && c == '[') {
                                read(STDIN_FILENO, &c, 1);
                                switch (c) {
                                        case 'A': /* up arrow */
                                                curs_pos_i = max(0, curs_pos_i-1);
                                                break;
                                        case 'B': /* down arrow */
                                                curs_pos_i = min(rows-1, curs_pos_i+1);
                                                break;
                                        case 'C': /* right arrow */
                                                curs_pos_j = min(cols-1, curs_pos_j+1);
                                                break;
                                        case 'D': /* left arrow */
                                                curs_pos_j = max(0, curs_pos_j-1);
                                                break;
                                        default:
                                                fflush(stdin);
                                }
                        }

                } else if (c == ENTER) {
                        break;
                } else if (c == ' ') {
                        state[curs_pos_i][curs_pos_j] = !state[curs_pos_i][curs_pos_j];
                }

                draw_edit_board(state, rows, cols, curs_pos_i, curs_pos_j, skin);
        }

        return state;
}

board_state zeros_board(int rows, int cols)
{
        board_state state;
        int i;

        state = malloc(rows * sizeof(int*));
        NULLCHECK(state);

        for (i=0; i<cols; i++) {
                state[i] = calloc(cols, sizeof(int));

                /* if any allocation fails every allocation before that has to be freed */
                if (!(state[i])) {
                        for (; i>=0; i--) {
                                free(state[i]);
                        }
                        return NULL;
                }
        }

        return state;
}

void print_board(board b)
{
        int i, j;

        printf("\n\n");

        for (i=0; i < b->config->rows; i++) {
                printf("\t");
                for (j=0; j < b->config->cols; j++) {
                        skin_print((b->state)[i][j], b->config->skin);
                }
                printf("\n");
        }

        printf("\n\n");
}

void skin_print(unsigned int a, board_skin skin)
{
        char *s1;
        char *s2;
        switch (skin) {
                default:
                case XES:
                        printf("%s ", a ? ANSI_COLOR_RED"X"ANSI_COLOR_RESET : "_");
                        break;
                case BRACKETS:
                        s1 = a ? ANSI_COLOR_RED"["ANSI_COLOR_RESET : ".";
                        s2 = a ? ANSI_COLOR_RED"]"ANSI_COLOR_RESET : ".";
                        printf("%s%s", s1, s2);
                        break;
        }
}

void skin_print_curs(unsigned int a, int x, int y, board_skin skin)
{
        char *s1;
        char *s2;
        switch (skin) {
                default:
                case XES:
                        printf("%s ", a ? ANSI_COLOR_INVERT ANSI_COLOR_RED"X"ANSI_COLOR_RESET : ANSI_COLOR_INVERT "_" ANSI_COLOR_RESET);
                        break;
                case BRACKETS:
                        s1 = a ? ANSI_COLOR_INVERT ANSI_COLOR_RED "[" ANSI_COLOR_RESET : ANSI_COLOR_INVERT "." ANSI_COLOR_RESET;
                        s2 = a ? ANSI_COLOR_INVERT ANSI_COLOR_RED "]" ANSI_COLOR_RESET : ANSI_COLOR_INVERT "." ANSI_COLOR_RESET;
                        printf("%s%s", s1, s2);
                        break;
        }
}

void draw(board b)
{
        int i, j;

        printf("\n\n");

        for (i=0; i < b->config->rows; i++) {
                printf(clearline);
                printf("\t");
                for (j=0; j < b->config->cols; j++) {
                        skin_print((b->state)[i][j], b->config->skin);
                }
                printf("\n");
        }

        printf("\n\n");

        /* move cursor up again */
        for (i=0; i < b->config->rows+4; i++) {
                printf(cursup);
        }
}

void draw_edit_board(board_state state, int rows, int cols, int x, int y, board_skin skin)
{
        int i, j;

        printf("\n\n");

        for (i=0; i<rows; i++) {
                printf(clearline);
                printf("\t");
                for (j=0; j<cols; j++) {
                        if (i == x && j == y) {
                                skin_print_curs(state[i][j], x, y, skin);
                        } else {
                                skin_print(state[i][j], skin);
                        }
                }
                printf("\n");
        }

        printf("\n\n");

        /* move cursor up again */
        for (i=0; i<rows+4; i++) {
                printf(cursup);
        }

}

void update(board b)
{
        int i,j;
        int a;
        board_state tmp;

        for (i=0; i < b->config->rows; i++) {
                for (j=0; j < b->config->cols; j++) {
                        a = live_neighbours(b, i,j);
                        /* live cell */
                        if (alive(b, i,j)) {
                                (b->next_state)[i][j] = a == 2 || a == 3 ? 1 : 0;
                        /* dead cell */
                        } else {
                                (b->next_state)[i][j] = a == 3 ? 1 : 0;
                        }
                }
        }

        tmp = b->state;
        b->state = b->next_state;
        b->next_state = tmp;

}

int alive(board b, int i, int j)
{
        return (b->state)[i][j];
}

int live_neighbours(board b, int i, int j)
{
        int sum = 0;
        int rows = b->config->rows;
        int cols = b->config->cols;

        /* top */
        sum += b->state[i-1 < 0 ? rows-1 : i-1][j];
        /* top right */
        sum += b->state[i-1 < 0 ? rows-1 : i-1][j+1 >= cols ? 0 : j+1];
        /* right */
        sum += b->state[i][j+1 >= cols ? 0 : j+1];
        /* bottom right */
        sum += b->state[i+1 >= rows ? 0 : i+1][j+1 >= cols ? 0 : j+1];
        /* bottom */
        sum += b->state[i+1 >= rows ? 0 : i+1][j];
        /* bottom left */
        sum += b->state[i+1 >= rows ? 0 : i+1][j-1 < 0 ? cols-1 : j-1];
        /* left */
        sum += b->state[i][j-1 < 0 ? cols-1 : j-1];
        /* top left */
        sum += b->state[i-1 < 0 ? rows-1 : i-1][j-1 < 0 ? cols-1 : j-1];

        return sum;
        
}