#ifndef FILE_READER_H__
#define FILE_READER_H__

#include "tree.h"
#include <stdio.h>

int file_load(FILE *fp, tree_t **tree);
int file_load_ex(const char *filename, tree_t **tree);

void file_save(FILE *fp, const tree_t *tree);

int file_search_symbol(FILE *fp, char symbol, size_t *cnt);
int file_search_symbol_ex(const char *filename, char symbol, size_t *cnt);

#endif // FILE_READER_H__
