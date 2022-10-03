#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <pthread.h>
#include <readline/readline.h>
#include <unistd.h>
#include "window.h"
#include "events.h"
#include "cmd.h"

pthread_t cli_thread;
struct EventQueue *eq;

int
main(int argc, char **argv)
{
	int engine_exited = 0;
	eq = malloc(sizeof(struct EventQueue));
	eq->head = NULL;
	eq->tail = NULL;
	node *action;
	pthread_mutex_init(&eq->mutex, NULL);
	pthread_create(&cli_thread, NULL, cmd_cli_interactive, NULL);
	printf("Starting Engine.\nPID:%d\n", getpid());
	while (!engine_exited) {
		action = get_event(eq);
		while (action != NULL) {
			(*action->action)(action->arg);
			action = get_event(eq);
		}
	}
	return 0;
}
