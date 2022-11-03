#ifndef RENDER_H
#define RENDER_H

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <stdio.h>
#include "util.h"

struct rnd_BufferedInt {
    int value;
    struct rnd_BufferedInt *next;
};
struct rnd_BufferedFloat {
    float value;
    struct rnd_BufferedFloat *next;
};
struct rnd_Vertex {
    float v[3];
    float vt[2];
    float vn[3];
};

union rnd_VecBuffer {
    float *raw;
    float (*vecs)[3];
};

union rnd_TCoordBuffer {
    float *raw;
    float (*coords)[2];
};

union rnd_IndexBuffer {
    int *raw;
    struct {
        int v;
        int vt;
        int vn;
    } *index;
};

void render(unsigned long time);
void rnd_close(void);
int rnd_init(int width, int height);
GLuint rnd_load_shader(char *filename, GLenum type);
GLuint rnd_create_shader_program(char *vshader_path, char *fshader_path);
GLuint rnd_create_vbo_from_obj(char *obj_path, char *texture_path); 
void rnd_load_vdata_into_buffer(struct rnd_BufferedFloat **buf_ptr, char* vdata, int *count);
void rnd_load_idata_into_buffer(struct rnd_BufferedInt **buf_ptr, char* idata, int *count);
float *rnd_dump_float_buffer(struct rnd_BufferedFloat **stack_ptr, int count);
int *rnd_dump_int_buffer(struct rnd_BufferedInt **stack_ptr, int count);

#endif /* RENDER_H */
