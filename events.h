#ifndef EVENTS_H
#define EVENTS_H

#include <pthread.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "main.h"

typedef void(*evt_EventFn_t)(char **);

typedef struct Event_ {
	evt_EventFn_t fnptr;
	char **args;
	struct Event_ *next;
} evt_Event_t; //TODO uneeded typedef?

struct evt_EventQueue {
	int size;
	SDL_mutex *mutex;
	evt_Event_t *head;
	evt_Event_t *tail;
};

void evt_add_event(evt_EventFn_t fnptr, void *args);
evt_Event_t *evt_get_event(void);
void evt_init(void);
void evt_process(void);
void evt_get_input(void);

#endif /* EVENTS_H */
