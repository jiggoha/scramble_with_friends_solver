typedef struct letter Letter;

Letter *create_trie();

void add_word(Letter *trie, char *word);

/* returns 0 or 1 */
int word_exist(Letter *trie, char *word);

void destroy_trie(Letter *trie);
