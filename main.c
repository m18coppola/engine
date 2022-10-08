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
#include "main.h"

int engine_exited = 0;

void
main_exit(void *arg) {
    engine_exited = 1;
}

int
main(int argc, char **argv)
{
    Event *event;
	printf("Starting Engine.\nPID:%d\n", getpid());
    evt_init();
    cmd_init();
	while (!engine_exited) {
        while((event = evt_get_event()) != NULL) {
            (*event->fnptr)(event->arg);
        }
	}
	return 0;
}
