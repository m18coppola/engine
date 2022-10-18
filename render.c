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

int
rnd_init(int width, int height)
{

    /* set openGL version for SDL2 */
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    /* create window */
    window = SDL_CreateWindow(
            "rinse",
            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            width, height,
            SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    if (window == NULL) {
        fprintf(
                stderr,
                "RND: Failed to create window. Exiting.\nSDL_Error: %s\n",
                SDL_GetError());
        goto STAGE2_ERROR;
    }
    /* create graphics context */
    SDL_GLContext g_context = SDL_GL_CreateContext(window);
    if (g_context == NULL) {
        fprintf(
                stderr,
                "RND: Failed to create graphics context. Exiting.\nSDL_Error: %s\n",
                SDL_GetError());
        goto STAGE3_ERROR;
    }

    /* initialize GLEW */
    glewExperimental = GL_TRUE;
    GLenum glewError = glewInit();
    if (glewError != GLEW_OK) {
        fprintf(stderr, "RND: Error initializing GLEW. Exiting.\nGLEW_Error: %s\n", glewGetErrorString(glewError));
        fprintf(
                stderr,
                "RND: Error initializing GLEW. Exiting.\nGLEW_Error: %s\n",
                glewGetErrorString(glewError));
        goto STAGE3_ERROR;
    }

    return 0;

STAGE3_ERROR:
    SDL_DestroyWindow(window);
STAGE2_ERROR:
    return -1;
}

GLuint
rnd_load_shader(char *filename, GLenum type)
{
    SDL_RWops *io;
    char *source;
    size_t filesize;
    GLuint shader_id;
    GLint gl_status;

    io = SDL_RWFromFile(filename, "r");
    filesize = SDL_RWsize(io);
    source = malloc(filesize + 1);
    if (SDL_RWread(io, source, filesize, 1) != 0) {
        fprintf(
                stderr,
                "RND: Failed to load file \"%s\". Exiting.\nSDL_Error: %s\n",
                filename,
                SDL_GetError());
                return (GLuint) 0;
    }
    SDL_RWclose(io);
    shader_id = glCreateShader(type);
    glShaderSource(shader_id, 1, (const GLchar * const *)&source, NULL);
    free(source);
    glCompileShader(shader_id);
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &gl_status);
    if (gl_status != GL_TRUE) {
        char errlog[512];
        glGetShaderInfoLog(shader_id, 512, NULL, errlog);
        fprintf(stderr,
                "RND: Failed to compile \"%s\"\n%s:\n%s\n",
                filename, source, errlog);
    }
    return shader_id;
}

GLuint
rnd_create_shader_program(char *vshader_path, char *fshader_path)
{
    GLuint vshader_id, fshader_id;
    GLuint shader_program_id;

    vshader_id = rnd_load_shader(vshader_path, GL_VERTEX_SHADER);
    fshader_id = rnd_load_shader(fshader_path, GL_FRAGMENT_SHADER);
    shader_program_id = glCreateProgram();
    glAttachShader(shader_program_id, vshader_id);
    glAttachShader(shader_program_id, fshader_id);
    glLinkProgram(shader_program_id);
    GLint linked;
    glGetProgramiv(shader_program_id, GL_LINK_STATUS, &linked);
    if (!linked) {
        char errlog[512];
        glGetProgramInfoLog(shader_program_id, 512, NULL, errlog);
        fprintf(stderr, "RND: Shader program failed to link:\n%s", errlog);
    }
    return shader_program_id;
}
