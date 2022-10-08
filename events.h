#ifndef EVENTS_H
#define EVENTS_H

#include <pthread.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

typedef void(*evt_EventFn_t)(void *);

typedef struct Event_ {
	evt_EventFn_t fnptr;
	void *arg;
	struct Event_ *next;
} evt_Event_t;

struct evt_EventQueue {
	int size;
	pthread_mutex_t mutex;
	evt_Event_t *head;
	evt_Event_t *tail;
};

void evt_add_event(evt_EventFn_t fnptr, void *args);
evt_Event_t *evt_get_event(void);
void evt_init(void);

#endif /* EVENTS_H */
