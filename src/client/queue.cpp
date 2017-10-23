#include <pthread.h>
#include <queue>
#include <stdio.h>
#include <stdlib.h>

#include "../../include/ps_client/client.h"
#include "../../include/ps_client/macros.h"

Queue::Queue() {
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

	Message value = data.front();
	data.pop();
	Pthread_cond_signal(&empty);
	Pthread_cond_signal(&empty);
	Pthread_mutex_unlock(&lock);
	return value;
}

