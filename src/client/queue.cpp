#include <pthread.h>
#include <queue>
#include <stdio.h>
#include <stdlib.h>

#include "../../lib/client.h"
#include "../../lib/macros.h"

Queue::Queue(/*Message s*/) /*: sentinel(s)*/ {
	int rc;
	Pthread_mutex_init(&lock, NULL);
	Pthread_cond_init(&fill, NULL);
	Pthread_cond_init(&empty, NULL);
}

void Queue::push(const Message &value) {
	int rc;
	Pthread_mutex_lock(&lock);
	while (data.size() >= QUEUE_MAX) {
		Pthread_cond_wait(&empty, &lock);
	}

	data.push(value);
	Pthread_cond_signal(&fill);
	Pthread_mutex_unlock(&lock);
}

Message Queue::pop() {
	int rc;
	Pthread_mutex_lock(&lock);
	while (data.empty()) {
		Pthread_cond_wait(&fill, &lock);
	}

	Queue value = data.front();
	if (value != sentinel) {
		data.pop();
		Pthread_cond_signal(&empty);
	} else {
		Pthread_cond_signal(&fill);
	}
	Pthread_cond_signal(&empty);
	Pthread_mutex_unlock(&lock);
	return value;
}

