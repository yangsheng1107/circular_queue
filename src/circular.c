#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

#define DELAY 900
/* Queue structure */
#define QUEUE_SIZE (10)

typedef struct _ring_st {
	int in;
	int out;
	int size;
	int element[QUEUE_SIZE];
}Ring_st;

void QueueInit(Ring_st * ring)
{
    ring->in = 0;
    ring->out = 0;
    ring->size = QUEUE_SIZE;
}

int QueuePut(Ring_st * ring, int value)
{
    if(ring->in == (( ring->out - 1 + ring->size) % ring->size))
    {
        return -1; /* Queue Full*/
    }

    ring->element[ring->in] = value;
	printf("\x1B[34min  :Queue[%d] =%d\033[0m\r\n", ring->in, ring->element[ring->in]); fflush(stdout); 
    ring->in = (ring->in + 1) % ring->size;

    return 0; // No errors
}

int QueueGet(Ring_st * ring, int *old)
{
    if(ring->in == ring->out)
    {
        return -1; /* Queue Empty - nothing to get*/
    }

    *old = ring->element[ring->out];
	printf("\x1B[32mout :Queue[%d] =%d\033[0m\r\n", ring->out, ring->element[ring->out]);fflush(stdout); 
    ring->out = (ring->out + 1) % ring->size;

    return 0; // No errors
}

void *producer(void *ptr)
{
	int i = 0;
	Ring_st *ring = (Ring_st *)ptr;
	printf("I'm a producer\n");
	while (1)
	{
		pthread_mutex_lock(&mutex);
		usleep(2*DELAY);
		QueuePut(ring, i);
		pthread_mutex_unlock(&mutex);
		i = i + 1;
	}
	pthread_exit(NULL);
}

void *consumer(void *ptr)
{
	int value;
	Ring_st *ring = (Ring_st *)ptr;
	printf("I'm a consumer\n");
	while(1)
	{
		pthread_mutex_lock(&mutex);
		usleep(DELAY);
		QueueGet(ring, &value);
		pthread_mutex_unlock(&mutex);
	}
	pthread_exit(NULL);
}

int main() {
	pthread_t producer_thread, consumer_thread;
	Ring_st ring;

	QueueInit(&ring);

	pthread_create(&consumer_thread,NULL,consumer,(void *)&ring);
	pthread_create(&producer_thread,NULL,producer,(void *)&ring);
	pthread_join(consumer_thread,NULL);
	return 0;
}
