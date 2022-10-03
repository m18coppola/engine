#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <readline/readline.h>

struct cmd_Token {
	char *string;
	struct cmd_Token *next;
};

struct cmd_TokenList {
	int size;
	struct cmd_Token *head;
};

void *cmd_cli_interactive(void *arg);
char **cmd_disassembleCommand(char* command);
struct cmd_TokenList *cmd_new_TokenList(void);
struct cmd_Token *cmd_new_Token(char *str);
void cmd_TokenList_append(struct cmd_TokenList *tl, char *str);
void cmd_TokenList_free(struct cmd_TokenList *tl);
char ** cmd_TokenList_vector(struct cmd_TokenList *tl);
