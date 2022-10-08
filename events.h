#ifndef EVENTS_H
#define EVENTS_H
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

typedef void(*Event_Function)(void *);

typedef struct Event_ {
	Event_Function fnptr;
	void *arg;
	struct Event_ *next;
} Event;

struct EventQueue {
	int size;
	pthread_mutex_t mutex;
	Event *head;
	Event *tail;
};

void evt_init(void);
void evt_add_event(Event_Function fnptr, void *args);
Event *evt_get_event(void);
#endif /* EVENTS_H */
