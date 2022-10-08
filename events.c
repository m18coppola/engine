#include "events.h"

static struct EventQueue eq;

void
evt_init(void)
{
    pthread_mutex_init(&eq.mutex, NULL);
}

void
evt_add_event(Event_Function new_function, void *arg)
{
	pthread_mutex_lock(&eq.mutex);
	Event *new_event;
	new_event = malloc(sizeof(Event));
	new_event->fnptr = new_function;
	new_event->arg = arg;
	if (eq.tail == NULL) {
		eq.head = new_event;
		eq.tail = new_event;
	} else {
		eq.tail->next = new_event;
		eq.tail = new_event;
	}
	pthread_mutex_unlock(&eq.mutex);
}

Event *
evt_get_event(void)
{
	pthread_mutex_lock(&eq.mutex);
	Event *old_event;
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
