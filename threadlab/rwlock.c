#include <stdio.h>
#include <time.h>
#include <errno.h>
#include <pthread.h>
#include <assert.h>
#include <stdlib.h>
#include "rwlock.h"

/* rwl implements a reader-writer lock.
 * A reader-write lock can be acquired in two different modes, 
 * the "read" (also referred to as "shared") mode,
 * and the "write" (also referred to as "exclusive") mode.
 * Many threads can grab the lock in the "read" mode.  
 * By contrast, if one thread has acquired the lock in "write" mode, no other 
 * threads can acquire the lock in either "read" or "write" mode.
 */

//helper function
static inline int
cond_timedwait(pthread_cond_t *c, pthread_mutex_t *m, const struct timespec *expire)
{
	int r; 
	if (expire != NULL)  {
		r = pthread_cond_timedwait(c, m, expire);
	} else
		r = pthread_cond_wait(c, m);
	assert(r == 0 || r == ETIMEDOUT);
	//printf("cond_timedwait() %d\n", r);
	return r;
}

//rwl_init initializes the reader-writer lock 
void
rwl_init(rwl *l)
{
	//printf("rwl_init()\n");
	l = (rwl*)malloc(sizeof(rwl));
	pthread_mutex_init(&l->mutex, NULL);
	pthread_cond_init(&l->r_cond, NULL);
	pthread_cond_init(&l->w_cond, NULL);
	l->readers = 0;
	l->writer = 0;
	l->r_waiters = 0; 
	l->w_waiters =0;
}

//rwl_nwaiters returns the number of threads *waiting* to acquire the lock
//Note: it should not include any thread who has already grabbed the lock
int
rwl_nwaiters(rwl *l) 
{
	//printf("rwl_nwaiters()\n");
	pthread_mutex_lock(&l->mutex);
	int r=l->r_waiters + l->w_waiters;
	pthread_mutex_unlock(&l->mutex);
	return r;
}

//rwl_rlock attempts to grab the lock in "read" mode
//if lock is not grabbed before absolute time "expire", it returns ETIMEDOUT
//else it returns 0 (when successfully grabbing the lock)
int
rwl_rlock(rwl *l, const struct timespec *expire)
{
	//printf("rwl_rlock()\n");
	pthread_mutex_lock(&l->mutex);
	l->r_waiters++;
	while(l->writer > 0 || l->w_waiters > 0){
		int r = cond_timedwait(&l->r_cond, &l->mutex, expire);
		if(r==ETIMEDOUT){
			l->r_waiters--;
			pthread_mutex_unlock(&l->mutex);
			return r;
		}
    }
    l->r_waiters--;
    l->readers++;
    pthread_mutex_unlock(&l->mutex);
    return 0;
}

//rwl_runlock unlocks the lock held in the "read" mode
void
rwl_runlock(rwl *l)
{
	//printf("rwl_runlock()\n");
	pthread_mutex_lock(&l->mutex);
	if((l->readers--)==0)
		pthread_cond_broadcast(&l->w_cond);
	else
		pthread_cond_signal(&l->w_cond);
	pthread_mutex_unlock(&l->mutex);
}

//rwl_wlock attempts to grab the lock in "write" mode
//if lock is not grabbed before absolute time "expire", it returns ETIMEDOUT
//else it returns 0 (when successfully grabbing the lock)
int
rwl_wlock(rwl *l, const struct timespec *expire)
{
	//printf("rwl_wlock()\n");
	pthread_mutex_lock(&l->mutex);
	l->w_waiters++;
	while(l->writer == 1 || l->readers > 0){
		int r = cond_timedwait(&l->w_cond, &l->mutex, expire);
		if(r==ETIMEDOUT){
			l->w_waiters--;
			pthread_mutex_unlock(&l->mutex);
			return r;
		}
	}
	l->w_waiters--;
	l->writer=1;
	pthread_mutex_unlock(&l->mutex);
	return 0;
}

//rwl_wunlock unlocks the lock held in the "write" mode
void
rwl_wunlock(rwl *l)
{
	//printf("rwl_wunlock()\n");
	pthread_mutex_lock(&l->mutex);
	l->writer--;
	pthread_cond_signal(&l->w_cond);
	pthread_cond_broadcast(&l->r_cond);
	pthread_mutex_unlock(&l->mutex);
}