#ifndef RENDER_H
#define RENDER_H
struct cl_res {
	int width, height;
};

void open_client(void *args);
void close_client(void *args);
void render_client(void);
#endif /* RENDER_H */
