#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "snake_utils.h"
#include "state.h"

const int BOARD_ROW = 10;
const int BOARD_COL = 14;

/* Helper function definitions */
static char get_board_at(game_state_t* state, int x, int y);
static void set_board_at(game_state_t* state, int x, int y, char ch);
static bool is_tail(char c);
static bool is_snake(char c);
static char body_to_tail(char c);
static int incr_x(char c);
static int incr_y(char c);
static void find_head(game_state_t* state, int snum);
static char next_square(game_state_t* state, int snum);
static void update_tail(game_state_t* state, int snum);
static void update_head(game_state_t* state, int snum);

/* Helper function to get a character from the board (already implemented for you). */
static char get_board_at(game_state_t* state, int x, int y) {
  return state->board[y][x];
}

/* Helper function to set a character on the board (already implemented for you). */
static void set_board_at(game_state_t* state, int x, int y, char ch) {
  state->board[y][x] = ch;
}

/* Task 1 */
game_state_t* create_default_state() {
  // TODO: Implement this function.
  game_state_t *default_state = (game_state_t *)malloc(sizeof(game_state_t));

  default_state->x_size = BOARD_COL;
  default_state->y_size = BOARD_ROW;
  default_state->num_snakes = 1;

  //malloc for the row pointer
  default_state->board = (char **)malloc(sizeof(char*) * BOARD_ROW);

  //malloc for each line and draw the board
  for (int i = 0; i < BOARD_ROW; i ++) {
    default_state->board[i] = (char *)malloc(sizeof(char) * BOARD_COL);
    if (i == 0 || i == BOARD_ROW - 1) strcpy(default_state->board[i], "##############");
    else strcpy(default_state->board[i], "#            #");
  }

  //malloc for the snake, and initial the snake
  default_state->snakes = (snake_t *)malloc(sizeof(snake_t));
  default_state->snakes->head_x = 5;
  default_state->snakes->head_y = 4;
  default_state->snakes->tail_x = 4;
  default_state->snakes->tail_y = 4;
  default_state->snakes->live = true;

  //then comes the fruit
  set_board_at(default_state, 9, 2, '*');

  set_board_at(default_state, default_state->snakes->head_x,
                 default_state->snakes->head_y, '>');
  set_board_at(default_state, default_state->snakes->tail_x,
                 default_state->snakes->tail_y, 'd');

  return default_state;
}

/* Task 2 */
void free_state(game_state_t* state) {
  // TODO: Implement this function.
  //free the board
  for (int i = 0; i < state->y_size; i ++) free(state->board[i]);
  free(state->board);
  free(state->snakes);
  free(state);

  return;
}

/* Task 3 */
void print_board(game_state_t* state, FILE* fp) {
  // TODO: Implement this function.

  //loop for each row and col
  for (int i = 0; i < state->y_size; i ++) {
    for (int j = 0; j < state->x_size; j ++) {
      fprintf(fp, "%c", state->board[i][j]);
    }
    fprintf(fp, "\n");
  }

  return;
}

/* Saves the current state into filename (already implemented for you). */
void save_board(game_state_t* state, char* filename) {
  FILE* f = fopen(filename, "w");
  print_board(state, f);
  fclose(f);
}

/* Task 4.1 */
static bool is_tail(char c) {
  // TODO: Implement this function.
  return (c == 'w' || c == 'a' 
          || c == 's' || c == 'd');
}

static bool is_snake(char c) {
  // TODO: Implement this function.
  return is_tail(c) || (c == 'x')
          || (c == '<') || (c == '>')
          || (c == 'v') || (c == '^');
}

static char body_to_tail(char c) {
  // TODO: Implement this function.
  if (c == '^') return 'w';
  if (c == '<') return 'a';
  if (c == 'v') return 's';
  if (c == '>') return 'd';
  else return ' ';
}

static int incr_x(char c) {
  // TODO: Implement this function.
  if (c == '>' || c == 'd') return 1;
  else if (c == '<' || c == 'a') return -1;
  else return 0;
}

static int incr_y(char c) {
  // TODO: Implement this function.
  if (c == 'v' || c == 's') return 1;
  else if (c == '^' || c == 'w') return -1;
  else return 0;
}

/* Task 4.2 */
static char next_square(game_state_t* state, int snum) {
  // TODO: Implement this function.
  if (state->snakes[snum].live == false) return 'x';

  int head_X = state->snakes[snum].head_x;
  int head_Y = state->snakes[snum].head_y;

  int inc_X = incr_x(get_board_at(state, head_X, head_Y));
  int inc_Y = incr_y(get_board_at(state, head_X, head_Y));

  return get_board_at(state, head_X + inc_X, head_Y + inc_Y);
}

/* Task 4.3 */
static void update_head(game_state_t* state, int snum) {
  // TODO: Implement this function.
  int head_X = state->snakes[snum].head_x;
  int head_Y = state->snakes[snum].head_y;

  char cur_head = get_board_at(state, head_X, head_Y);

  int inc_X = incr_x(cur_head);
  int inc_Y = incr_y(cur_head);

  set_board_at(state, head_X + inc_X, head_Y + inc_Y, cur_head);
  state->snakes[snum].head_x += inc_X;
  state->snakes[snum].head_y += inc_Y;

  return;
}

/* Task 4.4 */
static void update_tail(game_state_t* state, int snum) {
  // TODO: Implement this function.
  int tail_X = state->snakes[snum].tail_x;
  int tail_Y = state->snakes[snum].tail_y;

  char cur_tail = get_board_at(state, tail_X, tail_Y);

  int inc_X = incr_x(cur_tail);
  int inc_Y = incr_y(cur_tail);

  if (state->snakes[snum].head_x == inc_X + tail_X && state->snakes[snum].head_y == inc_Y + tail_Y) update_head(state, snum);
  char now_tail = body_to_tail(get_board_at(state, tail_X + inc_X, tail_Y + inc_Y));

  set_board_at(state, tail_X, tail_Y, ' ');
  set_board_at(state, tail_X + inc_X, tail_Y + inc_Y, now_tail);
  state->snakes[snum].tail_x += inc_X;
  state->snakes[snum].tail_y += inc_Y;

  return;
}

/* Task 4.5 */
void update_state(game_state_t* state, int (*add_food)(game_state_t* state)) {
  // TODO: Implement this function.
  for (int i = 0; i < state->num_snakes; i ++) {
    //first we move the snake head
    int head_X = state->snakes[i].head_x;
    int head_Y = state->snakes[i].head_y;
    int inc_X = incr_x(get_board_at(state, head_X, head_Y));
    int inc_Y = incr_y(get_board_at(state, head_X, head_Y));

    //case 1: the snake dies
    if (get_board_at(state, head_X + inc_X, head_Y + inc_Y) == '#' || is_snake(get_board_at(state, head_X + inc_X, head_Y + inc_Y))) {
      set_board_at(state, head_X, head_Y, 'x');
      state->snakes[i].live = false;
      return;
    } 

    //case 2: the fruit
    if (get_board_at(state, head_X + inc_X, head_Y + inc_Y) == '*') {
      update_head(state, i);
      add_food(state);
      return;
    }

    else {
      update_head(state, i);
      update_tail(state, i);
      return;
    }
  }
}

/* Task 5 */
game_state_t* load_board(char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening file: %s\n", filename);
        return NULL;
    }

    //determine board dimensions
    unsigned int row_size = 0, col_size = 0;
    char buffer[100];
    char** board_content = NULL;

    //read each line from the file
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        unsigned int len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') len--;
        if (col_size == 0) col_size = len;
        row_size++;
    }

    // allocate memory for the board content (array of strings)
    board_content = (char**)malloc(row_size * sizeof(char*));
    if (board_content == NULL) {
        fprintf(stderr, "Memory allocation failed for board content.\n");
        fclose(file);
        return NULL;
    }

    rewind(file);

    // allocate memory for each row in the board
    for (unsigned int i = 0; i < row_size; i++) {
        board_content[i] = (char*)malloc((col_size + 1) * sizeof(char));  // +1 for '\0'
  
        // read the line and remove newline character
        fgets(buffer, sizeof(buffer), file);
        buffer[strcspn(buffer, "\n")] = '\0';
        strcpy(board_content[i], buffer);
    }

    fclose(file);

    // create game_state_t struct and set x_size, y_size, and board
    game_state_t* state = (game_state_t*)malloc(sizeof(game_state_t));

    state->x_size = col_size;
    state->y_size = row_size;
    state->board = board_content;

    return state;
}

/* Task 6.1 */
static void find_head(game_state_t* state, int snum) {
  // TODO: Implement this function.
  int tail_X = state->snakes[snum].tail_x;
  int tail_Y = state->snakes[snum].tail_y;
  char tail = get_board_at(state, tail_X, tail_Y);

  while (is_snake(tail)) {
    int inc_X = incr_x(tail);
    int inc_Y = incr_y(tail);
    tail = get_board_at(state, tail_X + inc_X, tail_Y + inc_Y);
    if (is_snake(tail)) {
      tail_X += inc_X;
      tail_Y += inc_Y;
    } else {
      state->snakes[snum].head_x = tail_X;
      state->snakes[snum].head_y = tail_Y;
      return;
    }
  }

  return;
}

/* Task 6.2 */
game_state_t* initialize_snakes(game_state_t* state) {
  // TODO: Implement this function.
  int col_size = state->x_size;
  int row_size = state->y_size;
  int snake_num = 0;

  for (int i = 0; i < row_size; i ++) {
    for (int j = 0; j < col_size; j ++) {
      if (is_tail(get_board_at(state, j, i))) {
        state->snakes[snake_num].tail_x = j;
        state->snakes[snake_num].tail_y = i;
        find_head(state, snake_num); 
        snake_num ++;
      }
    }
  }

  state->num_snakes = snake_num;
  return state;
}
