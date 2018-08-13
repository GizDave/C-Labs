#ifndef RWLOCK_H
#define RWLOCK_H

#include <pthread.h>

typedef struct {
	pthread_mutex_t mutex;
	pthread_cond_t r_cond;
	pthread_cond_t w_cond;
	int readers;
	int writer;
	int r_waiters;
	int w_waiters;
}rwl;

void rwl_init(rwl *l);
int rwl_nwaiters(rwl *l);
int rwl_rlock(rwl *l, const struct timespec *expire);
void rwl_runlock(rwl *l);
int rwl_wlock(rwl *l, const struct timespec *expire);
void rwl_wunlock(rwl *l);

#endif