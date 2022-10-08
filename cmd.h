#ifndef CMD_H
#define CMD_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <readline/readline.h>
#include "events.h"
#include "main.h"

#define MAX_CMDS 64
#define FNV_OFFSET 0xcbf29ce484222325
#define FNV_PRIME 0x100000001b3

typedef void (*cmd_FnGeneric_t)(void *);

struct cmd_Token {
	char *string;
	struct cmd_Token *next;
};

struct cmd_TokenList {
	struct cmd_Token *head;
	int size;
};

struct cmd_Function {
    unsigned int hash;
    cmd_FnGeneric_t function_ptr;
};

int cmd_cli_interactive(void *arg);
char **cmd_disassembleCommand(char* command);
cmd_FnGeneric_t cmd_get_function(char *name);
unsigned int cmd_hash_command(char *str);
void cmd_init(void);
struct cmd_Token *cmd_new_Token(char *str);
struct cmd_TokenList *cmd_new_TokenList(void);
void cmd_register_command(char *name, cmd_FnGeneric_t function);
void cmd_TokenList_append(struct cmd_TokenList *tl, char *str);
void cmd_TokenList_free(struct cmd_TokenList *tl);
char ** cmd_TokenList_vector(struct cmd_TokenList *tl);

#endif /* CMD_H */
