#include "render.h"

static SDL_Window *window = NULL;
GLuint current_shader_program;
static int rnd_index_count;

void
render(unsigned long time)
{
    (void)time;
    if (window != NULL) {
        glClearColor(1.0, 1.0, 1.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(current_shader_program);
        glDrawElements(GL_TRIANGLES, rnd_index_count, GL_INT, (void *)0);
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

    //load test shader
    current_shader_program = rnd_create_shader_program("vshader.glsl", "fshader.glsl");
    
    //options
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

    rnd_create_vbo_from_obj("pyr.obj", "nop");

    return 0;

STAGE3_ERROR:
    SDL_DestroyWindow(window);
STAGE2_ERROR:
    return -1;
}

GLuint
rnd_load_shader(char *filename, GLenum type)
{
    char *source;
    char errlog[512];
    GLuint shader_id;
    GLint gl_status;

    source = utl_file_to_string(filename);
    shader_id = glCreateShader(type);
    if (shader_id == 0) {
        glGetShaderInfoLog(shader_id, 512, NULL, errlog);
        fprintf(
                stderr,
                "RND: OpenGL failed to allocate vram for \"%s\". Exiting.\nGL Error Log:\n%s\n",
                filename,
                errlog);
                return (GLuint) 0;
    }
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
    if (shader_program_id == 0) {
        fprintf(
                stderr,
                "RND: OpenGL failed to allocate vram for shader program (using \"%s\" and \"%s\"). Exiting.\n",
                vshader_path, fshader_path);
                return (GLuint) 0;
    }
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

GLuint
rnd_create_vbo_from_obj(char *obj_path, char *texture_path)
{

    //TODO: texture loading
    (void)texture_path;

    
    struct rnd_BufferedFloat *v_stack = NULL;
    struct rnd_BufferedFloat *vt_stack = NULL;
    struct rnd_BufferedFloat *vn_stack = NULL;
    struct rnd_BufferedInt *i_stack = NULL;
    int v_ct, vt_ct, vn_ct, i_ct;
    v_ct = vt_ct = vn_ct = i_ct = 0;
    union rnd_VecBuffer v_buf;
    union rnd_TCoordBuffer vt_buf;
    union rnd_VecBuffer vn_buf;
    union rnd_IndexBuffer i_buf;

    /* load from obj file */
    char *source;
    source = utl_file_to_string(obj_path);
    /* read source line-by-line, dump into temp buffers */
    char current_line[128];
    char *ptr;
    char *dest;

    ptr = source;
    while (*ptr != '\0') {
        dest = current_line;
        while (*ptr != '\n' && *ptr != '\0') {
            *(dest++) = *(ptr++);
        }
        *dest = '\0';
        ptr++;
        if (current_line[0] == 'v') {
            switch (current_line[1]) {
                case ' ':
                    rnd_load_vdata_into_buffer(&v_stack, current_line, &v_ct);
                    break;
                case 't':
                    rnd_load_vdata_into_buffer(&vt_stack, current_line, &vt_ct);
                    break;
                case 'n':
                    rnd_load_vdata_into_buffer(&vn_stack, current_line, &vn_ct);
                    break;
            }
        } else if (current_line[0] == 'f') {
            rnd_load_idata_into_buffer(&i_stack, current_line, &i_ct);
        }
    }
    free(source);
    v_buf.raw = rnd_dump_float_buffer(&v_stack, v_ct);
    vt_buf.raw = rnd_dump_float_buffer(&vt_stack, vt_ct);
    vn_buf.raw = rnd_dump_float_buffer(&vn_stack, vn_ct);
    i_buf.raw = rnd_dump_int_buffer(&i_stack, i_ct);

    struct rnd_Vertex *vertex_buffer;
    int *index_buffer;
    vertex_buffer = malloc(sizeof(struct rnd_Vertex) * v_ct);
    rnd_index_count = i_ct / 3;
    printf("INDEX_COUNT: %d\n", rnd_index_count);
    index_buffer = malloc(sizeof(int) * (i_ct / 3));
    for (int i = 0; i < i_ct / 3; i++) {
        vertex_buffer[i_buf.index[i].v].v[0] = v_buf.vecs[i_buf.index[i].v][0];
        vertex_buffer[i_buf.index[i].v].v[1] = v_buf.vecs[i_buf.index[i].v][1];
        vertex_buffer[i_buf.index[i].v].v[2] = v_buf.vecs[i_buf.index[i].v][2];
        vertex_buffer[i_buf.index[i].v].vt[0] = vt_buf.coords[i_buf.index[i].vt][0];
        vertex_buffer[i_buf.index[i].v].vt[1] = vt_buf.coords[i_buf.index[i].vt][1];
        vertex_buffer[i_buf.index[i].v].vn[0] = vn_buf.vecs[i_buf.index[i].vn][0];
        vertex_buffer[i_buf.index[i].v].vn[1] = vn_buf.vecs[i_buf.index[i].vn][1];
        vertex_buffer[i_buf.index[i].v].vn[2] = vn_buf.vecs[i_buf.index[i].vn][2];
        index_buffer[i] = i_buf.index[i].v;
    }
    
    /* create VAO for object */
    GLuint vao;
    glGenVertexArrays(1, &vao);

    /* bind it (makes it the current vbo state context) */
    glBindVertexArray(vao);

    GLuint vbos[2];
    glGenBuffers(2, vbos);

    glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(struct rnd_Vertex) * v_ct, vertex_buffer, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(struct rnd_Vertex), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(struct rnd_Vertex), (void *)3);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(struct rnd_Vertex), (void *)5);
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbos[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * (i_ct / 3), index_buffer, GL_STATIC_DRAW);

    
    ///* temp buffers to vbo */
    ///* pos data */
    //glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(float) * v_ct, v_buf, GL_STATIC_DRAW);
    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    //glEnableVertexAttribArray(0);

    ///* texture data */
    //glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vt_ct, vt_buf, GL_STATIC_DRAW);
    //glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
    //glEnableVertexAttribArray(1);

    ///* normal data */
    //glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vn_ct, vn_buf, GL_STATIC_DRAW);
    //glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
    //glEnableVertexAttribArray(2);

    ///* index data */
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[3]);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * vn_ct, vn_buf, GL_STATIC_DRAW);
    //glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
    //glEnableVertexAttribArray(2);

    free(v_buf.raw);
    free(vt_buf.raw);
    free(vn_buf.raw);
    free(i_buf.raw);
    return (GLuint) vao;
}

void
rnd_load_vdata_into_buffer(struct rnd_BufferedFloat **buf_ptr, char* vdata, int *count)
{
    int i = 2;
    char **tokens = utl_tokenize(vdata);
    while (tokens[i] != NULL) {
        struct rnd_BufferedFloat *new_buf_float = malloc(sizeof(struct rnd_BufferedFloat));
        new_buf_float->value = atof(tokens[i]);
        new_buf_float->next = *buf_ptr;
        *buf_ptr = new_buf_float;
        i++;
        (*count)++;
    }
    free(tokens);
}

void
rnd_load_idata_into_buffer(struct rnd_BufferedInt **buf_ptr, char* idata, int *count)
{
    int i = 2;
    char **tokens = utl_tokenize(idata);
    struct rnd_BufferedInt *new_buf_int;
    int values[3];
    int j;
    while (tokens[i] != NULL) {
        sscanf(tokens[i], "%d/%d/%d", &values[0], &values[1], &values[2]);
        for (j = 0; j < 3; j++) {
            new_buf_int = malloc(sizeof(struct rnd_BufferedInt));
            new_buf_int->value = values[j];
            new_buf_int->next = *buf_ptr;
            *buf_ptr = new_buf_int;
            (*count)++;
        }
        i++;
    }
    free(tokens);
}

float *
rnd_dump_float_buffer(struct rnd_BufferedFloat **stack_ptr, int count)
{
    float *buffer = malloc(sizeof(float) * count);
    struct rnd_BufferedFloat *new_root;

    int i = count - 1;
    while (i >= 0) {
        buffer[i] = (*stack_ptr)->value;
        new_root = (*stack_ptr)->next;
        free(*stack_ptr);
        *stack_ptr = new_root;
        i--;
    }
    return buffer;
}

int *
rnd_dump_int_buffer(struct rnd_BufferedInt **stack_ptr, int count)
{
    int *buffer = malloc(sizeof(int) * count);
    struct rnd_BufferedInt *new_root;

    int i = count - 1;
    while (i >= 0) {
        buffer[i] = (*stack_ptr)->value;
        new_root = (*stack_ptr)->next;
        free(*stack_ptr);
        *stack_ptr = new_root;
        i--;
    }
    return buffer;
}
