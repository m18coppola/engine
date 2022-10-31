#ifndef RENDER_H
#define RENDER_H

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <stdio.h>

void render(unsigned long time);
void rnd_close(void);
int rnd_init(int width, int height);
GLuint rnd_load_shader(char *filename, GLenum type);
GLuint rnd_create_shader_program(char *vshader_path, char *fshader_path);
GLuint rnd_create_vbo_from_obj(char *obj_path, char *texture_path); 

#endif /* RENDER_H */
