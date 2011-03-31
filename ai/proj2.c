// Bruce C. Miller
// AI Project 2
// Bugs: Need to bound vertical heuristic with -1 on top in full columns.
//       Perhaps change >=, <= to >, < in min/max-value function calls.

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define do_error(s) { printf("%s\n", s); exit(1); }

void print_board(short **);
short horizontal_h(short **);
short vertical_h(short **);
short diagonal_h(short **);
short *** successor(short **, int);
short ** make_move(short **, short, int);
int alpha_beta_search(short **, int);
int terminal_test(short **, int);
int * max_value(short **, int, int, int, int);
int * first_max_value(short **, int, int, int);
int * min_value(short **, int, int, int, int);
int * max_v(int *, int *);
int * min_v(int *, int *);
int compare(short **, short**);
short eval_h(short *, int);

int turn = 0;           // current player turn 1 = computer, 2 = player
int n = 0;              // n-value

int main(int argc, char * argv[]) {
    int i, t, move;
    int d = 5;          // depth to search
    char resp[3];
    short **board;

    // elite ascii banner
    printf(" CCC  OOO  N   N N   N EEEEE  CCC TTTTT    N   N\n");
    printf("C    O   O NN  N NN  N E     C      T      NN  N\n");
    printf("C    O   O N N N N N N EEE   C      T   == N N N\n");
    printf("C    O   O N  NN N  NN E     C      T      N  NN\n");
    printf(" CCC  OOO  N   N N   N EEEEE  CCC   T      N   N\n");

    // get the n-value for the game of connect n
    printf("Enter N-value for game:\n");
    scanf("%s", resp);
    n = atoi(resp);
    if (n < 4 || n > 8) {
        do_error("n-value must be integer between 4 and 8");
    }

    // set the depth dynamically
    if (n < 6) {
        d = 5;
    } else {
        d = 5;
    }

    // who's turn is it first?
    printf("Let computer go first?\n");
    scanf("%s", resp);
    if (resp[0] == 'y' || resp[0] == 'Y') {
        turn = 1;
    } else if (resp[0] == 'n' || resp[0] == 'N') {
        turn = 2;
    } else {
        do_error("Response must be y/n");
    }

    // initialize board
    board = calloc((2 * n - 1), sizeof(short *));
    for (i = 0; i < (2 * n - 1); i++) {
        board[i] = calloc((2 * n - 2), sizeof(short));
    }

    while(1) {
        // check current state for end game
        t = terminal_test(board, d);
        if (t == -127) {
            printf("computer loses!\n");
            exit(0);
        } else if (t == 127) {
            printf("computer wins!\n");
            exit(0);
        } else if (t == -128) {
            printf("game ends in draw!\n");
            exit(0);
        }

        // turn handling
        if (turn == 1) {
            move = alpha_beta_search(board, d);
            printf("computer moves piece to column %d\n", move);
            board = make_move(board, move, 1);
            print_board(board);
            turn = 2;
        } else {
            printf("select column to drop piece\n");
            scanf("%s", resp);
            move = atoi(resp);

            while (move < 0 || move > (2 * n - 2) ||
                   board[move][2 * n - 3] != 0) {
                printf("invalid move\n");
                printf("select column to drop piece\n");
                scanf("%s", resp);
                move = atoi(resp);
            }

            printf("player moves piece to column %d\n", move);
            board = make_move(board, move, 2);
            print_board(board);
            turn = 1;
        }
    }
    return(0);
}

// print out current board
void print_board(short ** state) {
    int i, j;
    // print board
    for (j = (2 * n - 2) - 1; j >= 0; j--) {
        for (i = 0; i < (2 * n - 1); i++) {
            printf("%d ", state[i][j]);
        }
        printf("\n");
    }
}

// successor function
short *** successor(short ** state, int next_piece) {
    short *** list;
    int i, j, k = 0, l;
    // alloc mem for state list, size = board width
    list = calloc((2 * n - 1), sizeof(short *));
    // loop through the board x axis
    for (i = 0; i < (2 * n - 1); i++) {
        // if the x-val slot isn't full
        if (state[i][2 * n - 3] == 0) {
            // alloc mem for a state space
            list[k] = calloc((2 * n - 1), sizeof(short *));
            for (j = 0; j < (2 * n - 1); j++) {
                list[k][j] = calloc((2 * n - 2),
                                    sizeof(short *));
            }
            // create new copy of state
            for (j = 0; j < (2 * n - 2); j++) {
                for (l = 0; l < (2 * n - 1); l++) {
                    list[k][l][j] = state[l][j];
                }
            }
            // append the next successor to the list
            list[k] = make_move(list[k], i, next_piece);
            // increment list elem counter
            k++;
        }
    }
    // return list of successors
    return(list);
}

// put the current player's piece in the specified collumn
short ** make_move(short ** state, short move, int next_piece) {
    int i;
    // find lowest empty position
    for (i = 0; i < 2 * n - 2; i++) {
        if (state[move][i] == 0) {
            state[move][i] = next_piece;
            break;
        }
    }
    return(state);
}

// alpha-beta search implementation
// returns the collumn a-b chooses to make next move
int alpha_beta_search(short ** state, int d) {
    int move;
    short *** successors;
    int * ab_data;              // [0] = h, [1] = move
    ab_data = malloc(sizeof(int) * 2);
    ab_data[0] = -1;
    ab_data[1] = -1;

    ab_data = first_max_value(state, -126, 126, d);

    successors = successor(state, 1);

    move = compare(state, successors[ab_data[1]]);

    if (ab_data[1] >= 0 && ab_data[1] < (2 * n - 1)) {
        return(move);
    }

    // couldn't find matching v-value
    move = rand() % (2 * n - 1);
    printf("random move being made\n");

    return(move);
}

// same as max_value(), except it retains the successor index (needed for
// alpha_beta_search()
int * first_max_value(short ** state, int a, int b, int d) {
    int t, i, j, all_zero;
    int * ab_data;
    short *** successors;
    ab_data = malloc(sizeof(int) * 2);
    all_zero = 1;

    // set v min poss max
    ab_data[0] = -126;
    ab_data[1] = n;
    t = terminal_test(state, d);

    if (abs(t) == 127 || t == -128) {    // +/- 127 win/loss -128 full board
        ab_data[0] = t;
        return(ab_data);
    } else if (t == 128) {               // max depth reached
        ab_data[0] = horizontal_h(state) + vertical_h(state) +
            diagonal_h(state);
        return(ab_data);
    }

    successors = successor(state, 1);
    // iterate thru the successors
    for (i = 0; i < (2 * n - 1); i++) {
        if (successors[i] != NULL && successors[i] != 0) {
            for (j = 0; j < (2 * n - 1); j++) {
                if (successors[i][j][0] != 0) {
                    all_zero = 0;
                    break;
                }
            }
            if (all_zero) {
                break;
            }
            ab_data = max_v(ab_data,
                            min_value(successors[i], a, b, (d - 1), i));
            if (ab_data[0] >= b) {
                return(ab_data);
            }
            if (a < ab_data[0]) {
                a = ab_data[0];
            }
        } else {
            break;
        }
        all_zero = 1;
    }
    return(ab_data);
}

// finds the max of the min values
int * max_value(short ** state, int a, int b, int d, int index) {
    int t, i, j, all_zero;
    int * ab_data, * tmp;
    short *** successors;
    ab_data = malloc(sizeof(int) * 2);
    tmp = malloc(sizeof(int) * 2);
    all_zero = 1;

    // set v min poss max
    ab_data[0] = -126;
    ab_data[1] = index;
    t = terminal_test(state, d);
    if (abs(t) == 127 || t == -128) {   // +/- 127 win/loss -128 full board
        ab_data[0] = t;
        return(ab_data);
    } else if (t == 128) {              // max depth reached
        ab_data[0] = horizontal_h(state) + vertical_h(state) +
            diagonal_h(state);
        return(ab_data);
    }

    successors = successor(state, 1);
    // iterate thru the successors
    for (i = 0; i < (2 * n - 1); i++) {
        if (successors[i] != NULL && successors[i] != 0) {
            for (j = 0; j < (2 * n - 1); j++) {
                if (successors[i][j][0] != 0) {
                    all_zero = 0;
                    break;
                }
            }
            if (all_zero) {
                break;
            }
            tmp = max_v(ab_data,
                        min_value(successors[i], a, b, (d - 1), i));
            ab_data[0] = tmp[0];
            if (ab_data[0] >= b) {
                return(ab_data);
            }
            if (a < ab_data[0]) {
                a = ab_data[0];
            }
        } else {
            break;
        }
        all_zero = 1;
    }
    return(ab_data);
}

// finds the min of the max values
int * min_value(short ** state, int a, int b, int d, int index) {
    int t, i, j, all_zero;
    int * ab_data, * tmp;
    short *** successors;
    ab_data = malloc(sizeof(int) * 2);
    tmp = malloc(sizeof(int) * 2);
    all_zero = 1;

    // set v max poss min
    ab_data[0] = 126;
    ab_data[1] = index;
    t = terminal_test(state, d);
    if (abs(t) == 127 || t == -128) {   // +/- 127 win/loss -128 full board
        ab_data[0] = t;
        return(ab_data);
    } else if(t == 128) {
        ab_data[0] = horizontal_h(state) + vertical_h(state) +
            diagonal_h(state);
        return(ab_data);
    }

    successors = successor(state, 2);
    // iterate thru the successors
    for (i = 0; i < (2 * n - 1); i++) {
        if (successors[i] != NULL && successors[i] != 0) {
            for (j = 0; j < (2 * n - 1); j++) {
                if (successors[i][j][0] != 0) {
                    all_zero = 0;
                    break;
                }
            }
            if (all_zero) {
                break;
            }
            tmp = min_v(ab_data,
                        max_value(successors[i], a, b, (d - 1), i));
            ab_data[0] = tmp[0];
            if (ab_data[0] <= a) {
                return(ab_data);
            }
            if (b < ab_data[0]) {
                b = ab_data[0];
            }
        } else {
            break;
        }
        all_zero = 1;
    }
    return(ab_data);
}

// test state for either a state at max search depth or an end (win/loss)
// state
// returns WIN/LOSS or -128 for full board or 128 for max depth
int terminal_test(short ** state, int d) {
    int h = 0;
    int h1, h2, h3, i;

    if (d == 0) {
        return(128);
    }

    h1 = horizontal_h(state);
    h2 = vertical_h(state);
    h3 = diagonal_h(state);

    // first test for full board
    // assigns -128 if not full board
    for (i = 0; i < (2 * n - 1); i++) {
        if (state[i][2 * n - 3] != 0) {
            h = -128;
        } else {
            h = 0;
            break;
        }
    }
    if (h == -128) {
        return(h);
    }

    // test for win/loss
    // perhaps add move assignments later should the heuristic be
    // modified to return them
    if (abs(h1) == 127) {
        h = h1;
    } else if (abs(h2) == 127) {
        h = h2;
    } else if (abs(h3) == 127) {
        h = h3;
    } else {
        h = h2 + h2 + h3;
    }

    return(h);
}

// returns the max of type int *, comparing elem 0
int * max_v(int * a, int * b) {
    if (a[0] > b[0]) {
        return(a);
    } else {
        return(b);
    }
}

// returns the min of type int *, comparing elem 0
int * min_v(int * a, int * b) {
    if (a[0] < b[0]) {
        return(a);
    } else {
        return(b);
    }
}

// compares 2 states one move apart and returns the column index changed
int compare(short ** state_orig, short ** state_changed) {
    int i, j;
    for (i = 0; i < (2 * n - 1); i++) {
        for (j = 0; j < (2 * n - 2); j++) {
            if (state_orig[i][j] != state_changed[i][j]) {
                return(i);
            }
        }
    }
    do_error("no change found\n");
    return(-1);
}

// takes an input array, and returns the heuristic for it
short eval_h(short * arr, int len) {
    short h, h1;
    int i;
    int f_edge = 0;
    int f_add = 1;
    short last_piece = 0;

    h = h1 = 0;

    if(arr[0] == -1) {
        f_edge = 1;
        last_piece = arr[1];
    } else {
        last_piece = arr[0];
    }

    for (i = f_edge; i < len; i++) {
        if (h1 == 0 - n) {
            return(-127);
        } else if (h1 == n) {
            return(127);
        }

        if (arr[i] == 1 && last_piece == 2 && !f_edge) {
            f_add = 1;
            h += h1;
            h1 = 1;
            last_piece = 1;
            f_edge = !f_edge;
        } else if (arr[i] == 2 && last_piece == 1 && !f_edge) {
            f_add = 1;
            h += h1;
            h1 = -1;
            last_piece = 2;
            f_edge = !f_edge;
        } else if (arr[i] == 1 && last_piece == 2 && f_edge) {
            f_add = 1;
            h1 = 1;
            last_piece = 1;
            f_edge = 1;
        } else if (arr[i] == 2 && last_piece == 1 && f_edge) {
            f_add = 1;
            h1 = -1;
            last_piece = 2;
            f_edge = 1;
        } else if (arr[i] == -1 && !f_edge) {
            f_add = 0;
            h += h1;
            break;
        } else if (arr[i] == -1 && f_edge) {
            f_add = 0;
            h1 = 0;
            break;
        } else if (arr[i] == 1) {
            f_add = 1;
            h1 += 1;
            last_piece = 1;
        } else if (arr[i] == 2) {
            f_add = 1;
            h1 -= 1;
            last_piece = 2;
        }
        arr[i] = 0;
    }
    if (f_add) {
        h += h1;
    }
    if (h1 <= (0 - n)) {
        return(-127);
    } else if (h1 >= n) {
        return(127);
    }
    return(h);
}

// heuristics for verticals in state
short vertical_h(short ** state) {
    int i, j, h, h1, k;
    short arr[2 * n];
    h = h1 = k = 0;
    for (i = 0; i < (2 * n - 1); i++) {
        k = 0;
        if (state[i][0] != 0) {
            arr[0] = -1;
            k++;
            for (j = 0; j < (2 * n - 2) && state[i][j] != 0; j++) {
                arr[k] = state[i][j];
                k++;
                if (j == (2 * n - 3)) {
                    arr[k] = -1;
                    k++;
                }
            }
            
            if (abs(h1 = eval_h(arr, k)) == 127) {
                return(h1);
            } else {
                h += h1;
            }
        }
    }
    return(h);
}

// heuristics for horizontals in state
short horizontal_h(short ** state) {
    int i, j, k, h, h1;
    short arr[2 * n + 1];
    h = h1 = 0;
    k = 0;
    for (i = 0; i < (2 * n - 2); i++) {
        k = 0;
        if (state[0][i] != 0) {
            arr[k] = -1;
            k++;
        }
        for (j = 0; j < (2 * n - 1); j++) {
            if (state[j][i] != 0) {
                arr[k] = state[j][i];
                k++;
            } else {
                if(abs(h1 = eval_h(arr, k)) == 127) {
                    return(h1);
                } else {
                    h += h1;
                }
                k = 0;
            }
            if (j == (2 * n - 2) && k > 0) {
                k++;
                arr[k] = -1;
                if (abs(h1 = eval_h(arr, k)) == 127) {
                    return(h1);
                } else {
                    h += h1;
                }
            }
        }
    }
    return(h);
}

// heuristic for diagonals in state
short diagonal_h(short ** state) {
    int i, j, k, l, h, h1;
    short arr[24];
    h = l = j = k = h1 = 0;

    // diag for 1,0 -> 2*n-1,0
    for (i = 1; i < (2 * n - 1); i++) {
        k = 0;
        if (state[i][j] != 0) {
            arr[k] = -1;
            k++;
        }
        for (j = 0; j < (2 * n - 2); j++) {
            if (state[i + l][j] != 0) {
                arr[k] = state[i + l][j];
                k++;
            } else {
                if(abs(h1 = eval_h(arr, k)) == 127) {
                    return(h1);
                } else {
                    h += h1;
                }
                k = 0;
            }
            if ((i + l) == (2 * n - 2) && k > 0) {
                k++;
                arr[k] = -1;
                if (abs(h1 = eval_h(arr, k)) == 127) {
                    return(h1);
                } else {
                    h += h1;
                }
                break;
            } else if ((i + l) == (2 * n - 2)) {
                break;
            }
            l++;
        }
        l = 0;
    }

    // diag for 0,0 -> 0,2*n-2 (diagonal up)
    for (j = 0; j < (2 * n - 2); j++) {
        k = 0;
        if(state[0][j] != 0) {
            arr[k] = -1;
            k++;
        }
        for (i = 0; i < (2 * n - 1); i++) {
            if (state[i][j + l] != 0) {
                arr[k] = state[i][j + l];
                k++;
            } else {
                if (abs(h1 = eval_h(arr, k)) == 127) {
                    return(h1);
                } else {
                    h += h1;
                }
                k = 0;
            }
            if ((j + l) == (2 * n - 3) && k > 0) {
                k++;
                arr[k] = -1;

                if (abs(h1 = eval_h(arr, k)) == 127) {
                    return (h1);
                } else {
                    h += h1;
                }
                break;
            } else if ((j + l) == (2 * n - 3)) {
                break;
            }
            l++;
        }
        l = 0;
    }

    // diag for 0,2*n-3 -> 0,0 (diagonal down)
    for (j = (2 * n - 3); j >= 0; j--) {
        k = 0;
        if (state[0][j] != 0) {
            arr[k] = -1;
            k++;
        }
        for (i = 0; i < (2 * n - 1); i++) {
            if (state[i][j - l] != 0) {
                arr[k] = state[i][j - l];
                k++;
            } else {
                if (abs(h1 = eval_h(arr, k)) == 127) {
                    return(h1);
                } else {
                    h += h1;
                }
                k = 0;
            }
            if ((j - l) == 0 && k > 0) {
                k++;
                arr[k] = -1;
                if (abs(h1 = eval_h(arr, k)) == 127) {
                    return(h1);
                } else {
                    h += h1;
                }
                break;
            } else if ((j - l) == 0) {
                break;
            }
            l++;
        }
        l = 0;
    }

    // diag for 1,2*n-3 -> 2*n-2,2*n-3
    for (i = 1; i < (2 * n - 1); i++) {
        k = 0;
        if (state[i][2 * n - 3] != 0) {
            arr[k] = -1;
            k++;
        }
        for (j = (2 * n - 3); j >= 0; j--) {
            if (state[i + l][j] != 0) {
                arr[k] = state[i + l][j];
                k++;
            } else {
                if (abs(h1 = eval_h(arr, k)) == 127) {
                    return(h1);
                } else {
                    h += h1;
                }
                k = 0;
            }
            if ((i + l) == (2 * n - 2) && k > 0) {
                k++;
                arr[k] = -1;
                if (abs(h1 = eval_h(arr, k)) == 127) {
                    return(h1);
                } else {
                    h += h1;
                }
                break;
            } else if ((i + l) == (2 * n - 2)) {
                break;
            }
            l++;
        }
        l = 0;
    }
    return(h);
}
