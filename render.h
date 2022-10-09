#ifndef RENDER_H
#define RENDER_H

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <stdio.h>

typedef GLuint rnd_Shader_t;

void render(void);
void rnd_close(void);
int rnd_init(int width, int height);
rnd_Shader_t rnd_load_shader(char *filename);

#endif /* RENDER_H */
