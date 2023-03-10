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

		args = utl_tokenize(cmd_line);
        if ((fn_ptr = cmd_get_function(args[1]))) {
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
    if (cmd_function_table[index].hash == 0) {
        return NULL;
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
    cmd_register_command("set", cmd_set_variable);
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
    if (cmd_variable_table[index].hash == 0) {
        return NULL;
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

void
cmd_set_variable(char **args)
{
    char **value;
    if (args[2] == NULL) {
        printf("usage: set varname [value]\n");
    } else if (args[3] == NULL) {
        value = cmd_get_variable(args[2]);
        if (value == NULL) {
            printf("Variable %s not set.\n", args[2]);
        } else {
            printf("%s\n", *value);
        }
    } else {
        value = cmd_get_variable(args[2]);
        int i = 0;
        char *ptr;
        ptr = args[3];
        while (!ptr[i++]);
        ptr = malloc(sizeof(char) * ++i);
        i = 0;
        while (args[3][i]) {
            ptr[i] = args[3][i];
            i++;
        }
        ptr[i] = '\0';
        if (value == NULL) {
            cmd_register_variable(args[2], ptr);
        } else {
            free(*value);
            *value = ptr;
        }
    }
    fflush(stdout);
}
