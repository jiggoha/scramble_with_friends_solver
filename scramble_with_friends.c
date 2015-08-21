#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include "trie.h"

#define BOARD_LENGTH (4)
#define NUM_CUBES (BOARD_LENGTH * BOARD_LENGTH)

/*
 * Initializes the Scramble board with a given length.
 */
char *create_board(int length) {
  char *board;

  board = malloc(sizeof(char) * 17);
  board[16] = '\0';

  // "official" distribution of letters from cubes
  if (length == 4) {
    char die[16][7];

    strcpy(die[0], "AAEEGN");
    strcpy(die[1], "ELRTTY");
    strcpy(die[2], "AOOTTW");
    strcpy(die[3], "ABBJOO");
    strcpy(die[4], "EHRTVW");
    strcpy(die[5], "CIMOTU");
    strcpy(die[6], "DISTTY");
    strcpy(die[7], "EIOSST");
    strcpy(die[8], "DELRVY");
    strcpy(die[9], "ACHOPS");
    strcpy(die[10], "HIMNQU");
    strcpy(die[11], "EEINSU");
    strcpy(die[12], "EEGHNW");
    strcpy(die[13], "AFFKPS");
    strcpy(die[14], "HLNNRZ");
    strcpy(die[15], "DEILRX");

    time_t t;
    srand((unsigned) time(&t));
    for (int i = 0; i < 16; i++) {
      board[i] = die[i][rand() % 6];
    }
  }

  return board;
}

void destroy_board(char *board) {
  free(board);
}

void print_board(char *board, int num_cubes, int board_length) {
  for (int i = 0; i < num_cubes; i++) {
    if (board[i] == 'Q') {
      printf("Qu ");
    } else {
      printf("%c  ", board[i]);
    }

    if ((i + 1) % board_length == 0) {
      putchar('\n');
    }
  }
}

Letter *initialize_dictionary() {
  Letter *dictionary;
  char *word;

  dictionary = create_trie();
  word = malloc(sizeof(char) * 17);

  while (scanf("%s\n", word) != EOF) {
    add_word(dictionary, word);
  }

  free(word);

  return dictionary;
}

void destroy_dictionary(Letter *dictionary) {
  destroy_trie(dictionary);
}

/*
 * Returns 1 or 0.
 */
int out_of_bounds(char *board, int index) {
  if (index < 0 || index >= NUM_CUBES) {
    return 1;
  } else {
    return 0;
  }
}

/*
 * Denotes if two vertices are an edge in a scramble board. Returns 0 or 1.
 */ 
int is_edge(int a, int b, int board_length) {
  // top left corner
  if (a == 0) {
    if (b == 1 || b == board_length || b == board_length + 1) {
      return 1;
    } else {
      return 0;
    }
  }

  // top right corner
  if (a == board_length - 1) {
    if (b == a - 1 || b == a + board_length || b == a + board_length - 1) {
      return 1;
    } else {
      return 0;
    }
  }

  // bottom right corner
  if (a == board_length * board_length - 1) {
    if (b == a - 1 || b == a - board_length || b == a - board_length - 1) {
      return 1;
    } else {
      return 0;
    }
  }

  // bottom left corner
  if (a == board_length * (board_length - 1)) {
    if (b == a + 1 || b == a - board_length || b == a - board_length + 1) {
      return 1;
    } else {
      return 0;
    }
  }

  // top row
  if (a < board_length) {
    // same row
    if (b == a - 1 || b == a + 1)
      return 1;

    // row below
    if (b == a + board_length - 1 || b == a + board_length || b == a + board_length + 1)
      return 1;

    return 0;
  }

  // left column
  if (a % board_length == 0) {
    // row above
    if (b == a - board_length || b == a - board_length + 1)
      return 1;
    // same row
    if (b == a + 1)
      return 1;
    // row below
    if (b == a + board_length || b == a + board_length + 1)
      return 1;

    return 0;
  }

  // right column
  if (a % board_length == board_length - 1) {
    // top row
    if (b == a - board_length || b == a - board_length - 1)
      return 1;
    // same row
    if (b == a - 1)
      return 1;
    // bottom row
    if (b == a + board_length || b == a + board_length - 1)
      return 1;

    return 0;
  }

  // bottom row
  if (a >= board_length * (board_length - 1)) {
    // top row
    if (b == a - board_length - 1 || b == a - board_length || b == a - board_length + 1)
      return 1;

    // same row
    if (b == a - 1 || b == a + 1)
      return 1;

    return 0;
  }


  // center
  // in the row above
  else {
    int row_above = a - board_length;
    if (row_above - 1 == b || row_above == b || row_above + 1 == b)
      return 1;

    // in the same row
    if (a - b == 1 || a + b == 1)
      return 1;

    // in the row below
    int row_below = a + board_length;
    if (row_below - 1 == b || row_below == b || row_below + 1 == b)
      return 1;

    return 0;
  }
}

int **create_adj_matrix(int board_length, int adj_length) {
  int **adj_matrix;

  adj_matrix = malloc(sizeof(int *) * adj_length);
  for (int i = 0; i < adj_length; i++) {
    adj_matrix[i] = malloc(sizeof(int) * adj_length);
  }

  for (int i = 0; i < adj_length; i++) {
    for (int j = 0; j < adj_length; j++) {
      adj_matrix[i][j] = is_edge(i, j, board_length);
    }
  }

  return adj_matrix;
}

void destroy_adj_matrix(int **adj_matrix, int length) {
  for (int i = 0; i < length; i++) {
    free(adj_matrix[i]);
  }
  free(adj_matrix);
}

int *copy_array(int *old_array, int length) {
  int *new_array;
  new_array = malloc(sizeof(int) * length);
  
  for (int i = 0; i < length; i++) {
    new_array[i] = old_array[i];
  }

  return new_array;
}

char *to_word(char *board, int *so_far, int so_far_length) {
  char *word;

  word = malloc(sizeof(char) * (so_far_length + 1));
  for (int i = 0; i < so_far_length; i++) {
    word[i] = board[so_far[i]];
  }
  word[so_far_length] = '\0';

  return word;
}

int not_in_array(int *array, int length, int value) {
  for (int i = 0; i < length; i++) {
    if (value == array[i])
      return 0;
  }
  return 1;
}

void find_words(Letter *dictionary, char *board, int board_length, int current_cube, int **edges, int *so_far, int so_far_length, int num_cubes) {
  char *word;
  word = to_word(board, so_far, so_far_length);
  if (word_exist(dictionary, word))
    puts(word);
  free(word);

  int *to_explore;
  to_explore = malloc(sizeof(int) * num_cubes);
  for (int i = 0; i < num_cubes; i++) {
    to_explore[i] = -1;
  }

  int to_explore_length = 0;
  for (int i = 0; i < num_cubes; i++) {
    if (is_edge(current_cube, i, board_length) == 1 && not_in_array(so_far, so_far_length, i)) {
      to_explore[to_explore_length] = i;
      to_explore_length++;
    }
  }

  int *new_so_far;
  new_so_far = (int *) realloc(so_far, so_far_length + 1);
  so_far_length += 1;

  // "pop" a cube to explore
  if (to_explore_length == 0)
    return;
  to_explore_length -= 1;

  for (int i = 0; i <= to_explore_length; i++) {
    int next_cube;

    next_cube = to_explore[i];
    new_so_far[so_far_length - 1] = next_cube;

    int *new_so_far_copy;
    new_so_far_copy = copy_array(new_so_far, so_far_length);
    find_words(dictionary, board, board_length, next_cube, edges, new_so_far_copy, so_far_length, num_cubes);
    free(new_so_far_copy);
  }

  // if (so_far != NULL)
  //   free(so_far);

  // free(new_so_far);
  // free(to_explore);
}

void solve(char *board, Letter *dictionary, int board_length, int num_cubes) {
  int **edges;
  edges = create_adj_matrix(board_length, num_cubes);

  int *so_far;
  so_far = malloc(sizeof(int));
  int so_far_length = 1;
  for (int i = 0; i < num_cubes; i++) {
    so_far[0] = i;
    find_words(dictionary, board, board_length, i, edges, so_far, so_far_length, num_cubes);
  }
}

int main(void) {
  char *board;

  board = create_board(BOARD_LENGTH);
  print_board(board, NUM_CUBES, BOARD_LENGTH);

  puts("Loading dictionary...");
  Letter *dict;
  dict = initialize_dictionary();
  
  puts("Searching for words...");
  solve(board, dict, BOARD_LENGTH, NUM_CUBES);

  destroy_dictionary(dict);
  destroy_board(board);

  return 0;
}
