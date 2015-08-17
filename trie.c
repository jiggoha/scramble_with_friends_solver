#include <stdlib.h>
#include <string.h>
#include <stdio.h>
// #include "trie.h"

typedef struct letter {
	char content;
	struct letter *next;
	int end; // 1 or 0
} Letter;

/*
 * Initializes a 26 length array.
 */
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
