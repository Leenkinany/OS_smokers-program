

#include <stdio.h>
#include <pthread.h>

void record_smoking(int smoker_id);
void print_results(void);


#define REPEATS 100


void *test_smoker(void *arg)
{

	int smoker_id = *(int *)arg;

	for (int i =0; i < REPEATS; i++)
	{
		record_smoking(smoker_id);
	}
	
	return NULL;
}

int main (void)
{

	pthread_t threads[3];
	int ids[3] = {0, 1, 2};

	for(int i = 0; i < 3; i++)
 	{
		if( pthread_create(&threads[i], NULL,

			test_smoker, &ids[i]) != 0)
	{
		fprintf(stderr, "Thread creation failed.\n");
		return 1;
}
}
 	for (int i = 0; i < 3; i++)
	{
		pthread_join(threads[i], NULL);
	}
 	
	print_results();

	return 0;
}

