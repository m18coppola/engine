#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <SDL2/SDL.h>

struct utl_Token {
	char *string;
	struct utl_Token *next;
};

char *utl_file_to_string(char *filepath);
char **utl_tokenize(char *string);

#endif /* UTIL_H */
