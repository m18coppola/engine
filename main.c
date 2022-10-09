#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <pthread.h>
#include <readline/readline.h>
#include <unistd.h>
#include "render.h"
#include "events.h"
#include "cmd.h"
#include "main.h"

static int engine_exited = 0;

void
main_exit(char **args)
{
    engine_exited = 1;
}

int
main(int argc, char **argv)
{
    /* parameters unused */
    (void)argc;
    (void)argv;

	printf("Starting Engine.\nPID:%d\n", getpid());
    evt_init();
    cmd_init();
	rnd_init(720, 480);
	while (!engine_exited) {
        evt_process();
		render();
	}
	rnd_close();
	return 0;
}
