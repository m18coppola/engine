#ifndef WINDOW_H
#define WINDOW_H
struct cl_res {
	int width, height;
};

void open_client(void *args);
void close_client(void *args);
void render_client(void);
#endif /* WINDOW_H */
