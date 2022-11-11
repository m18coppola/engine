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
    (void)args;
    engine_exited = 1;
}

int
main(int argc, char **argv)
{
    /* parameters unused */
    (void)argc;
    (void)argv;

    unsigned long time;

    printf("Starting Engine.\nPID:%d\n", getpid());
    if(init() < 0) {
        fprintf(
                stderr,
                "MAIN: Engine initialization failed. Exiting.\n");
        exit(-1);
    }

	while (!engine_exited) {
        time = SDL_GetTicks();
        evt_process();
		render(time);
	}
	rnd_close();
	return 0;
}

int
init()
{
    evt_init();
    cmd_init();

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(
                stderr,
                "MAIN: SDL2 failed to initialize. SDL_Error: %s\n",
                SDL_GetError());
        goto STAGE1_ERROR;
    }

    if(rnd_init(720, 480) < 0) {
        fprintf(
                stderr,
                "MAIN: rnd system failed to initialize.\n");
        goto STAGE2_ERROR;
    }

    return 0;
STAGE2_ERROR:
    SDL_Quit();
STAGE1_ERROR:
    return -1;
}
