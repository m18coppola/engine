#include "cmd.h"

void *
cmd_cli_interactive(void *arg) {
	/* prompt string */
	char *prompt;

	/* flag to exit main loop */
	int exited = 0;

	/* pointer to last command entered */
	char *cmd_line = NULL;
	
	/* default prompt */
	prompt = "[$) ";

	/* array of pointers to each individual argument */
	char** args;

	/* main command-processing loop */
	while (!exited) {

		/* read next command prompt line */
		if (cmd_line != NULL) {
			free(cmd_line);
		}
		cmd_line = readline(prompt);

		/* NULL return indicates EOF */
		if (!cmd_line) {
			exited = 1;
			break;
		}

		args = cmd_disassembleCommand(cmd_line);
		if (!strcmp(args[0],"exit")) {
			exited = 1;
		} else {
			printf("Command \"%s\" not recognized.\n", args[0]);
		}

		free(args);
		args = NULL;
	}

	/* clean up */
	if (args) free(args);
	if (cmd_line) free(cmd_line);

	return NULL;
}

char **
cmd_disassembleCommand(char* command)
{
	/* index: current character pointer, length: current character group length, i: index of copied string */
	int index, length, newGroupLength, i, j;
	/* string that points to the current chracter group */
	char* currentArgument;
	/* list of arguments */
	struct cmd_TokenList* args;
	/* argument vector */
	char** argv;

	args = cmd_new_TokenList();

	index = 0;
	/* while we haven't reached the end of the string */
	while (command[index] != '\0') {
		if (command[index] == ' ' || command[index] == '\t') {
			/* if we're currently on a whitespace, jump to next index */
			index++;
		} else {
			/* count the length of the group */
			length = 0;
			newGroupLength = 0;
			while (command[index + length] != ' '
					&& command[index + length] != '\t'
					&& command[index + length] != '\0'){
				if(command[index + length] == '\\')
				{
					length++;
				}
				length++;
				newGroupLength++;
			}
			/* copy the group */
			currentArgument = malloc(sizeof(char) * (newGroupLength + 1));
			j = 0;
			for (i = 0; i < length; i++) {
				if(command[index + i] != '\\'){
					currentArgument[j] = command[index + i];
					j++;
				}
			}
			currentArgument[newGroupLength] = '\0';

			cmd_TokenList_append(args, currentArgument);
			index += length;
		}
	}	
	
	argv = cmd_TokenList_vector(args);
	cmd_TokenList_free(args);

	return argv;	
}

struct cmd_TokenList *
cmd_new_TokenList(void)
{
	struct cmd_TokenList *tl;

	tl = malloc(sizeof(struct cmd_TokenList));
	tl->size = 0;
	tl->head = NULL;

	return tl;
}

struct cmd_Token *
cmd_new_Token(char *str)
{
	struct cmd_Token *t;

	t = malloc(sizeof(struct cmd_Token));
	t->string = str;
	t->next = NULL;

	return t;
}

void
cmd_TokenList_append(struct cmd_TokenList *tl, char *str)
{
	struct cmd_Token **t_ptr;
	struct cmd_Token *t;

	t = malloc(sizeof(struct cmd_Token));
	if (t == NULL) {
		fprintf(stderr, "malloc() returned NULL while adding to cmd_TokenList");
		return; 
	}
	t->string = str;
	t->next = NULL;

	t_ptr = &(tl->head);
	while ((*t_ptr) != NULL) {
		//node *finger = (*p);
		//finger = finger->next;
		//p = &(finger);
		t_ptr = &(*t_ptr)->next;
	}
	(*t_ptr) = t;
	tl->size++;
}

char **
cmd_TokenList_vector(struct cmd_TokenList *tl) {
	int i;
	struct cmd_Token **t_ptr;
	char **vec;

	vec = malloc((tl->size + 1) * sizeof(char *));
	t_ptr = &(tl->head);
	i = 0;
	while ((*t_ptr) != NULL) {
		vec[i++] = (*t_ptr)->string;
		t_ptr = &(*t_ptr)->next;
	}
	vec[i] = NULL;

	return vec;
}

void
cmd_TokenList_free(struct cmd_TokenList *tl)
{
	struct cmd_Token *t_ptr;
	struct cmd_Token *t_ptr_next;

	t_ptr = tl->head;
	t_ptr_next = t_ptr;
	while (t_ptr != NULL) {
		t_ptr_next = t_ptr->next;
		//free(t_ptr->string);
		free(t_ptr);
		t_ptr = t_ptr_next;
	}

	free(tl);
}
