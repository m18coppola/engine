#include "events.h"

static struct evt_EventQueue eq;

void
evt_add_event(evt_EventFn_t new_function, void *arg)
{
	pthread_mutex_lock(&eq.mutex);
	evt_Event_t *new_event;
	new_event = malloc(sizeof(evt_Event_t));
	new_event->fnptr = new_function;
	new_event->arg = arg;
	new_event->next = NULL;
	if (eq.tail == NULL) {
		eq.head = new_event;
		eq.tail = new_event;
	} else {
		eq.tail->next = new_event;
		eq.tail = new_event;
	}
	pthread_mutex_unlock(&eq.mutex);
}

evt_Event_t *
evt_get_event(void)
{
	pthread_mutex_lock(&eq.mutex);
	evt_Event_t *old_event;
	if (eq.head == NULL) {
		pthread_mutex_unlock(&eq.mutex);
		return NULL;
	} else {
		old_event = eq.head;
		eq.head = eq.head->next;
		if (eq.head == NULL) {
			eq.tail = NULL;
		}
		pthread_mutex_unlock(&eq.mutex);
		return old_event;
	}
}

void
evt_init(void)
{
    pthread_mutex_init(&eq.mutex, NULL);
}
