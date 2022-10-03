#ifndef EVENTS_H
#define EVENTS_H
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

typedef void(*event_action)(void *);

typedef struct node_ {
	event_action action;
	void *arg;
	struct node_ *next;
} node;

struct EventQueue {
	int size;
	pthread_mutex_t mutex;
	node *head;
	node *tail;
};

void add_event(struct EventQueue *eq, event_action action, void *args);
node *get_event(struct EventQueue *eq);
#endif /* EVENTS_H */
