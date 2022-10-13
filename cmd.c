#include "cmd.h"

static struct cmd_Function cmd_function_table[MAX_CMDS] = {0};
static struct cmd_Variable cmd_variable_table[MAX_VARS] = {0};
static SDL_Thread *cli_thread;

int
cmd_cli_interactive(void *arg)
{
    /* parameter unused */
    (void)arg;

	/* prompt string */
	char *prompt;

	/* flag to exit main loop */
	int exited = 0;

	/* pointer to last command entered */
	char *cmd_line = NULL;
	
	/* default prompt */
	prompt = "$ ";

	/* array of pointers to each individual argument */
	char** args;

    evt_EventFn_t fn_ptr;

	/* main command-processing loop */
	while (!exited) {

		/* read next command prompt line */
		cmd_line = readline(prompt);

		/* NULL return indicates EOF */
		if (!cmd_line) {
			exited = 1;
			break;
		}

		args = cmd_tokenize(cmd_line);
        if (fn_ptr = cmd_get_function(args[1])) {
            evt_add_event(fn_ptr, args);
        } else {
            if (args[1] != NULL) {
                printf("Command \"%s\" not recognized.\n", args[1]);
            }
            free(args[0]);
            free(args);
        }
       
        
		args = NULL;
	}

	return 0;
}

char **
cmd_tokenize(char *str)
{
    char **token_list;
    struct cmd_Token *head;
    struct cmd_Token *ptr;
    head = malloc(sizeof(struct cmd_Token));
    head->string = str;
    head->next = NULL;
    ptr = head;
    int i;
    int in_token = 0;
    int token_count = 0;
    
    while (*str != '\0') {
        if (*str <= ' ') {
            if (in_token) {
                *str = '\0';
                in_token = 0;
            }
        } else if (!in_token) {
            token_count++;
            ptr->next = malloc(sizeof(struct cmd_Token));
            ptr = ptr->next;
            ptr->string = str;
            ptr->next = NULL;
            in_token = 1;
        }
        str++;
    }
    token_list = malloc(sizeof(char **) * (2 + token_count));
    i = 0;
    while(head != NULL) {
        token_list[i] = head->string;
        ptr = head;
        head = head->next;
        free(ptr);
        i++;
    }
    token_list[i] = NULL;

    return token_list;
}

evt_EventFn_t
cmd_get_function(char *name)
{
    if (name == NULL) {
        return NULL;
    }
    unsigned int hash = cmd_hash(name);
    int index = hash % MAX_CMDS;
    while (cmd_function_table[index].hash != hash && cmd_function_table[index].hash != 0) {
        index++;
    }
    return cmd_function_table[index].function_ptr;
}

unsigned int
cmd_hash(char *str)
{
    long hash = FNV_OFFSET;
    while (*str != '\0') {
        hash *= FNV_PRIME;
        hash ^= *(str++);
    }
    return hash;
}

void
cmd_init(void)
{
	cli_thread = SDL_CreateThread((SDL_ThreadFunction)cmd_cli_interactive, "CLI", NULL);
    cmd_register_command("exit", main_exit);
    cmd_register_command("load_shader", dbg_rnd_load_shader);
}

void
cmd_register_command(char *name, evt_EventFn_t function)
{
    unsigned int hash = cmd_hash(name);
    int index = hash % MAX_CMDS;
    while (cmd_function_table[index].hash != 0) {
        index++;
    }
    cmd_function_table[index].hash = hash;
    cmd_function_table[index].function_ptr = function;
}

char**
cmd_get_variable(char *name)
{
    unsigned int hash = cmd_hash(name);
    int index = hash % MAX_VARS;
    while (cmd_variable_table[index].hash != hash && cmd_variable_table[index].hash != 0) {
        index++;
    }
    return &cmd_variable_table[index].value;
}


void
cmd_register_variable(char *name, char *value)
{
    unsigned int hash = cmd_hash(name);
    int index = hash % MAX_VARS;
    while (cmd_variable_table[index].hash != 0) {
        index++;
    }
    cmd_variable_table[index].hash = hash;
    cmd_variable_table[index].value = value;
}
