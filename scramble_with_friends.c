#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
// #include "trie.h"

typedef struct letter {
  char content;
  struct letter *next;
  int end; // 1 or 0
} Letter;

Letter *create_trie() {
  Letter *trie;

  trie = malloc(sizeof(Letter) * 26);

  for (int i = 0; i < 26; i++) {
    trie[i].content = 'A' + i;
    trie[i].next = NULL;
    trie[i].end = 0;
  }

  return trie;
}

/*
 * If word doesn't exist, find the right Letter and mark that end = 1.
 */
void add_word(Letter *trie, char *word) {
  int length;
  int position;

  length = strlen(word);
  position = word[0] - 'A';

  if (length == 1) {
    trie[position].end = 1;
    return;
  }

  for (int i = 0; i < length; i++) {
    if (trie[position].next == NULL) {
      trie[position].next = create_trie();
    }
    add_word(trie[position].next, word + 1);
  }
}

/*
 * Cleanup.
 */
void destroy_trie(Letter *trie) {
  for (int i = 0; i < 26; i++) {
    if (trie[i].next != NULL) {
      destroy_trie(trie[i].next);
    } else if (i == 25) {
      free(trie);
    }
  }
}

/*
 * Returns 1 if word in trie, 0 otherwise.
 */
int word_exist(Letter *trie, char *word) {
  int length;
  int position;

  length = strlen(word);
  position = word[0] - 'A';

  if (length <= 1) {
    if (trie[position].end == 1) {
      return 1;
    } else {
      return 0;
    }
  }
  
  // dead end
  if (trie[position].next == NULL) {
    return 0;
  }
  // continue searching
  else {
    if (word_exist(trie[position].next, word + 1) == 1) {
      return 1;
    } else {
      return 0;
    }
  }
}


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

void print_board(char *board) {
  int num_chars;
  int length;

  num_chars = strlen(board);
  length = (int) sqrt(num_chars);

  for (int i = 0; i < num_chars; i++) {
    if (board[i] == 'Q') {
      printf("Qu ");
    } else {
      printf("%c  ", board[i]);
    }

    if ((i + 1) % length == 0) {
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

// add keeping track of used cubes
void find_words(char *board, Letter *dictionary, int last_square, char so_far[], int used[]) {
  int next_cubes[8];

  if (word_exist(dictionary, so_far) == 1) {
    puts(so_far);
  }

  next_cubes[0] = last_square - BOARD_LENGTH - 1;
  next_cubes[1] = last_square - BOARD_LENGTH;
  next_cubes[2] = last_square - BOARD_LENGTH + 1;
  next_cubes[3] = last_square - 1;
  next_cubes[4] = last_square + 1;
  next_cubes[5] = last_square + BOARD_LENGTH - 1;
  next_cubes[6] = last_square + BOARD_LENGTH;
  next_cubes[7] = last_square + BOARD_LENGTH + 1;

  char *next_word;
  int end_index;
  
  end_index = strlen(so_far);
  next_word = malloc(sizeof(char) * 17);

  for (int i = 0; i < 8; i++) {
    if (next_cubes[i] >= 0 && used[next_cubes[i]] == 0) {
      strcpy(next_word, &so_far[end_index - 1]);
      next_word[end_index] = board[next_cubes[i]];
      next_word[end_index+1] = '\0';
      used[next_cubes[i]] = 1;
      find_words(board, dictionary, next_cubes[i], next_word, used);
    }
  }

  free(next_word);
}

void solve(char *board, Letter *dictionary) {
  char so_far[17];
  int used[16];

  for (int i = 0; i < 16; i++) {
    used[i] = 0;
  }

  for (int i = 0; i < 16; i++) {
    so_far[0] = board[i];
    so_far[1] = '\0';
    used[i] = 1;
    find_words(board, dictionary, i, so_far, used);
  }
}

int main(void) {
	char *board;

	// board = create_board(BOARD_LENGTH);
  // print_board(board);
  // destroy_board(board);

  // board = malloc(sizeof(char) * 17);
  // board[0] = 'A';
  // board[1] = 'T';
  // for (int i = 2; i < 16; i++) {
  //   board[i] = 'Z';
  // }
  // board[16] = '\0';

  board = malloc(sizeof(char) * 4);
  board[0] = 'A';
  board[1] = 'T';
  board[2] = 'Z';
  board[3] = '\0';

  Letter *dict;
  dict = initialize_dictionary();

  solve(board, dict);

  destroy_dictionary(dict);
  free(board);

	return 0;
}
