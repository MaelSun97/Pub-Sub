#include <pthread.h>
#include <queue>
#include <stdio.h>
#include <stdlib.h>

#include "../../include/ps_client/client.h"
#include "../../include/ps_client/macros.h"

Queue::Queue() {
	int rc;
	Pthread_mutex_init(&lock, NULL);
	Pthread_cond_init(&cond, NULL);
}

void Queue::push(const Message &value) {
	int rc;
	Pthread_mutex_lock(&lock);

	data.push(value);
	Pthread_cond_signal(&cond);
	Pthread_mutex_unlock(&lock);
}

Message Queue::pop() {
	int rc;
	Pthread_mutex_lock(&lock);
	while (data.empty()) {
		Pthread_cond_wait(&cond, &lock);
	}

	Message value = data.front();
	data.pop();
	Pthread_cond_signal(&cond);
	Pthread_mutex_unlock(&lock);
	return value;
}

