// thread.cpp

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "../../lib/client.h"
#include "../../lib/macros.h"

void Thread::start(thread_func func, void *arg) {
	int rc;
	PTHREAD_CHECK(pthread_create(&thread, NULL, func, arg));
}

void Thread::join(void **result) {
	int rc;
	PTHREAD_CHECK(pthread_join(thread, result));
}

void Thread::detach() {
	int rc;
	PTHREAD_CHECK(pthread_detach(thread));
}
