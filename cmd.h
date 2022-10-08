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

struct cmd_Token {
	char *string;
	struct cmd_Token *next;
};

struct cmd_Function {
    unsigned int hash;
    evt_EventFn_t function_ptr;
};

int cmd_cli_interactive(void *arg);
evt_EventFn_t cmd_get_function(char *name);
unsigned int cmd_hash_command(char *str);
void cmd_init(void);
void cmd_register_command(char *name, evt_EventFn_t function);
char **cmd_tokenize(char *string);

#endif /* CMD_H */
