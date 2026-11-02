#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

// Testing and logging module
// Operating Systems Assignment 2
// Cigarette Smokers Problem


int smoker_count [3] = {0,0,0};

pthread_mutex_t  count_mutex = PTHREAD_MUTEX_INITIALIZER;

void record_smoking(int smoker_id)
{
	if (smoker_id >= 0 && smoker_id < 3)
	{
	
	pthread_mutex_lock(&count_mutex);
	
	smoker_count[smoker_id]++;
	
	printf("Smoker %d completed smoking.\n",

         	smoker_id +1);
	
	pthread_mutex_unlock(&count_mutex);

	}
}
 void print_results(void)
{
	printf("\n-- Testing Results --\n");
 	
	for (int i = 0; i < 3; i ++)
	{
	printf("Smoker %d smoked %d times.\n",

	i + 1, smoker_count[i]);
}
}
