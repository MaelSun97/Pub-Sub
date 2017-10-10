// macros.h

#pragma once
#include <string.h>


#define PTHREAD_CHECK(f) \
    if ((rc = (f)) != 0) { \
	fprintf(stderr, "%s\n", strerror(rc)); \
	exit(EXIT_FAILURE); \
    }

#define Pthread_mutex_init(l, a) \
    PTHREAD_CHECK(pthread_mutex_init(l, a));

#define Pthread_mutex_lock(l) \
    PTHREAD_CHECK(pthread_mutex_lock((pthread_mutex_t *)(l)));

#define Pthread_mutex_unlock(l) \
    PTHREAD_CHECK(pthread_mutex_unlock((pthread_mutex_t *)(l)));

#define Pthread_cond_init(c, a) \
    PTHREAD_CHECK(pthread_cond_init((c), (a)));

#define Pthread_cond_wait(c, l) \
    PTHREAD_CHECK(pthread_cond_wait((c), (l)));

#define Pthread_cond_signal(c) \
    PTHREAD_CHECK(pthread_cond_signal((pthread_cond_t *)(c)));

