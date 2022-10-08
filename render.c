#include "render.h"

static SDL_Window *window = NULL;

void
render(void)
{
	if (window != NULL) {
		glClearColor(1.0, 1.0, 1.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);
		SDL_GL_SwapWindow(window);
	}
}

void
rnd_close(void)
{
	SDL_DestroyWindow(window);
}

void
rnd_init(int width, int height)
{
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
            "rinse",
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

rnd_Shader_t
rnd_load_shader(char *filename)
{
    SDL_RWops *io;
    char *source;
    size_t filesize;

    io = SDL_RWFromFile(filename, "r");
    filesize = SDL_RWsize(io);
    source = malloc(filesize);
    if (SDL_RWread(io, source, filesize, 1) > 0) {
        printf("%s\n", source);
    }
    SDL_RWclose(io);
    return (rnd_Shader_t)0;
}
