#include "events.h"

static struct evt_EventQueue eq;

void
evt_add_event(evt_EventFn_t new_function, void *args)
{
	SDL_LockMutex(eq.mutex);
	evt_Event_t *new_event;
	new_event = malloc(sizeof(evt_Event_t));
	new_event->fnptr = new_function;
	new_event->args = args;
	new_event->next = NULL;
	if (eq.tail == NULL) {
		eq.head = new_event;
		eq.tail = new_event;
	} else {
		eq.tail->next = new_event;
		eq.tail = new_event;
	}
	SDL_UnlockMutex(eq.mutex);
}

evt_Event_t *
evt_get_event(void)
{
	SDL_LockMutex(eq.mutex);
	evt_Event_t *old_event;
	if (eq.head == NULL) {
		SDL_UnlockMutex(eq.mutex);
		return NULL;
	} else {
		old_event = eq.head;
		eq.head = eq.head->next;
		if (eq.head == NULL) {
			eq.tail = NULL;
		}
		SDL_UnlockMutex(eq.mutex);
		return old_event;
	}
}

void
evt_init(void)
{
	eq.mutex = SDL_CreateMutex();
}

void
evt_process(void)
{
        evt_Event_t *event;

		evt_get_input();
        while((event = evt_get_event()) != NULL) {
            (*event->fnptr)(event->args);
            if (event->args) {
                free(event->args[0]);
                free(event->args);
            }
            free(event);
        }
}

void
evt_get_input(void)
{
	SDL_Event e;
	while (SDL_PollEvent(&e) != 0) {
		switch (e.type) {
			case SDL_QUIT:
				evt_add_event((evt_EventFn_t)main_exit, NULL);
				break;
			default:
				break;
		}
	}
}
