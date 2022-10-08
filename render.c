#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <stdio.h>
#include "render.h"

SDL_Window *window = NULL;

void
open_client(void *args)
{

	int width, height;
	struct cl_res *in = (struct cl_res *)args;
	width = in->width;
	height = in->height;
	free(args);

    /* begin SDL2 */
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(
                stderr,
                "SDL2 failed to initialize. Exiting.\nSDL_Error: %s\n",
                SDL_GetError()
               );
        goto STAGE1_ERROR;
    }

    /* set openGL version for SDL2 */
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    /* create window */
    window = SDL_CreateWindow(
            "cgfw",
            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            width, height,
            SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN
            );
    if (window == NULL) {
        fprintf(
                stderr,
                "Failed to create window. Exiting.\nSDL_Error: %s\n",
                SDL_GetError(
                    ));
        goto STAGE2_ERROR;
    }
    /* create graphics context */
    SDL_GLContext g_context = SDL_GL_CreateContext(window);
    if (g_context == NULL) {
        fprintf(stderr, "Failed to create graphics context. Exiting.\nSDL_Error: %s\n", SDL_GetError());
        goto STAGE3_ERROR;
    }

    /* initialize GLEW */
    glewExperimental = GL_TRUE;
    GLenum glewError = glewInit();
    if (glewError != GLEW_OK) {
        fprintf(stderr, "Error initializing GLEW. Exiting.\nGLEW_Error: %s\n", glewGetErrorString(glewError));
        goto STAGE3_ERROR;
    }

	return;

STAGE3_ERROR:
    SDL_DestroyWindow(window);
STAGE2_ERROR:
    SDL_Quit();
STAGE1_ERROR:
}

void
close_client(void *args)
{
	SDL_DestroyWindow(window);
}

void
render_client(void)
{
	if (window != NULL) {
		glClearColor(1.0, 1.0, 1.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);
		SDL_GL_SwapWindow(window);
	}
}


