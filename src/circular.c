#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

#define DELAY 900
/* Queue structure */
#define QUEUE_SIZE 10
#define MAX_BUF_LEN 1024
#define MAX_BUF_QUEUE QUEUE_SIZE

char buf[MAX_BUF_QUEUE][MAX_BUF_LEN];

typedef struct _ring_st {
	int in;
	int out;
	int size;
	char* element[QUEUE_SIZE];
}Ring_st;

void QueueInit(Ring_st * ring)
{
	int i;
	ring->in = 0;
	ring->out = 0;
	ring->size = QUEUE_SIZE;
	for(i=0;i<QUEUE_SIZE;i++)
		ring->element[i] = buf[i];
}

char* QueuePut(Ring_st * ring)
{
	char* ptr = NULL;
	if(ring->in == (( ring->out - 1 + ring->size) % ring->size))
	{
		return ptr;
	}

	ptr = ring->element[ring->in];

	return ptr; // No errors
}

void QueuePutUpdate(Ring_st * ring)
{
	ring->in = (ring->in + 1) % ring->size;
}

char* QueueGet(Ring_st * ring)
{
	char* ptr = NULL;
	if(ring->in == ring->out)
	{
		return ptr; /* Queue Empty - nothing to get*/
	}

	ptr = ring->element[ring->out];

	return ptr; // No errors
}

void QueueGutUpdate(Ring_st * ring)
{
	ring->out = (ring->out + 1) % ring->size;
}

void *producer(void *ptr)
{
	int i = 0;
	char *tmp;
	char data[MAX_BUF_LEN];
	Ring_st *ring = (Ring_st *)ptr;
	printf("I'm a producer\n");
	while (1)
	{
		pthread_mutex_lock(&mutex);
		usleep(2*DELAY);
		tmp = (char *) QueuePut(ring);
		if(tmp != NULL)
		{
			sprintf(data,"%02x %02x %02x %02x %02x %02x %02x %02x %02x %02x",i ,i ,i ,i ,i, i ,i ,i ,i ,i);
			memcpy(tmp, data, strlen(data));
			printf("\x1B[34min  :Queue[%d] =%s\033[0m\r\n", ring->in, ring->element[ring->in]); fflush(stdout); 
			QueuePutUpdate(ring);
		}
		pthread_mutex_unlock(&mutex);
		i = i + 1;
	}
	pthread_exit(NULL);
}

void *consumer(void *ptr)
{
	char *tmp;
	Ring_st *ring = (Ring_st *)ptr;
	printf("I'm a consumer\n");
	while(1)
	{
		pthread_mutex_lock(&mutex);
		usleep(DELAY);
		tmp = (char *) QueueGet(ring);
		if(tmp != NULL)
		{
			printf("\x1B[32mout :Queue[%d] =%s\033[0m\r\n", ring->out, ring->element[ring->out]);fflush(stdout); 
			QueueGutUpdate(ring);
		}
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
