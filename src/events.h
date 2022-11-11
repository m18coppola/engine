#ifndef EVENTS_H
#define EVENTS_H

#include <pthread.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "main.h"

typedef void(*evt_EventFn_t)(char **);

struct evt_Event {
	evt_EventFn_t fnptr;
	char **args;
	struct evt_Event *next;
}; 

struct evt_EventQueue {
	int size;
	SDL_mutex *mutex;
	struct evt_Event *head;
	struct evt_Event *tail;
};

void evt_add_event(evt_EventFn_t fnptr, void *args);
struct evt_Event *evt_get_event(void);
void evt_init(void);
void evt_process(void);
void evt_get_input(void);

#endif /* EVENTS_H */
