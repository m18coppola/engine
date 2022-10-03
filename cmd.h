#ifndef CMD_H
#define CMD_H
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
	struct cmd_Token *head;
	int size;
};

typedef void (*cmd_function_generic)(char **);

struct cmd_Function {
    unsigned int hash;
    cmd_function_generic function_ptr;
};

#define MAX_CMDS 64
#define FNV_OFFSET 0xcbf29ce484222325
#define FNV_PRIME 0x100000001b3

void *cmd_cli_interactive(void *arg);
char **cmd_disassembleCommand(char* command);
struct cmd_TokenList *cmd_new_TokenList(void);
struct cmd_Token *cmd_new_Token(char *str);
void cmd_TokenList_append(struct cmd_TokenList *tl, char *str);
void cmd_TokenList_free(struct cmd_TokenList *tl);
char ** cmd_TokenList_vector(struct cmd_TokenList *tl);

void cmd_register_command(char *name, cmd_function_generic function);
cmd_function_generic cmd_get_function(char *name);
unsigned int cmd_hash_command(char *str);

#endif /* CMD_H */

/*
cmd_function_t    *cmd_functions     // A linked list, each element contains a string name and a function pointer: void (*)() .
cvar_t            *cvar_vars         // A linked list, each element contains a string name and a string value.
cmdalias_t        *cmd_alias         // A linked list, each element contains a string name and a string alias.

hastable needs: insert, delete, search
*/
