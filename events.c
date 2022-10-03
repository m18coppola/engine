#include "events.h"

void
add_event(struct EventQueue *eq, event_action new_action, void *arg)
{
	pthread_mutex_lock(&eq->mutex);
	node *new_node;
	new_node = malloc(sizeof(node));
	new_node->action = new_action;
	new_node->arg = arg;
	if (eq->tail == NULL) {
		eq->head = new_node;
		eq->tail = new_node;
	} else {
		eq->tail->next = new_node;
		eq->tail = new_node;
	}
	pthread_mutex_unlock(&eq->mutex);
}

node *
get_event(struct EventQueue *eq)
{
	pthread_mutex_lock(&eq->mutex);
	node *old_node;
	if (eq->head == NULL) {
		pthread_mutex_unlock(&eq->mutex);
		return NULL;
	} else {
		old_node = eq->head;
		eq->head = eq->head->next;
		if (eq->head == NULL) {
			eq->tail = NULL;
		}
		pthread_mutex_unlock(&eq->mutex);
		return old_node;
	}
}
