#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

int cnt = 0;
int NITER;
sem_t mutex;

void usage(char * message)
{
    //program name in argv[0] passed as message
    fprintf(stderr, "usage: %s NumIncrements\n", message);
    exit(EXIT_FAILURE);
}

void * Count(void * a)
{
    int i, tmp;
    for(i = 0; i < NITER; i++)
    {
        sem_wait(&mutex);/* wait if semaphore is in use */
        tmp = cnt;       /* copy the global cnt locally */
        tmp = tmp+1;     /* increment the local copy */
        cnt = tmp;       /* store the local value into the global cnt */
        sem_post(&mutex);/* update semaphore to let other threads use cnt */
    }
}

int main(int argc, char * argv[])
{
    pthread_t tid1, tid2;

    //must have exactly one argument
    if(argc != 2)
    {
        usage(argv[0]);
    } else
    {
        //check if argument is an integer
        if(sscanf(argv[1], "%d", &NITER) != 1)
        {
            usage(argv[0]);
        }
    }

    //verify that argument is positive
    if(NITER < 0)
    {
        fprintf(stderr, "usage: %s NumIncrements\nNumIncrements must be a positive integer\n", argv[0]);
        exit(EXIT_FAILURE);
    }

// PLEASE DO NOT remove or modify the following code
	printf("2*NITER is [%d]\n", 2*NITER);
// End of code section

    sem_init(&mutex, 0, 1);

	// creating Thread 1
	if(pthread_create(&tid1, NULL, Count, NULL))
	{
		printf("\n ERROR creating thread 1");
		exit(1);
	}

	// creating Thread 2
	if(pthread_create(&tid2, NULL, Count, NULL))
	{
		printf("\n ERROR creating thread 2");
		exit(1);
	}

	if(pthread_join(tid1, NULL))
	{
		printf("\n ERROR joining thread");
		exit(1);
	}

	if(pthread_join(tid2, NULL))
	{
		printf("\n ERROR joining thread");
		exit(1);
	}

// PLEASE DO NOT remove or modify the following code
	printf("\nCounter is [%d]\n", cnt);
	if (cnt == 2 * NITER)
		printf("\nPASS\n");
	else
		printf("\nFAIL\n");
// End of code section

	pthread_exit(NULL);
    sem_destroy(&mutex);
}
