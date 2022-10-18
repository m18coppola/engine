#ifndef RENDER_H
#define RENDER_H

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <stdio.h>

void render(void);
void rnd_close(void);
int rnd_init(int width, int height);
GLuint rnd_load_shader(char *filename, GLenum type);
GLuint rnd_create_shader_program(char *vshader_path, char *fshader_path);

#endif /* RENDER_H */
