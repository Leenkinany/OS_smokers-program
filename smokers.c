#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include "testing.h"

#define ROUNDS 12

// The agent can place ingredients only when the table is free.
sem_t agentSem;
sem_t tobaccoSmokerSem;
sem_t paperSmokerSem;
sem_t matchSmokerSem;

// The shutdown flag is shared between the agent and smoker threads.
pthread_mutex_t shutdownMutex = PTHREAD_MUTEX_INITIALIZER;
int shutdownRequested = 0;

static int should_shutdown(void)
{
    int stop;
    pthread_mutex_lock(&shutdownMutex);
    stop = shutdownRequested;
    pthread_mutex_unlock(&shutdownMutex);
    return stop;
}

// Each smoker waits for precisely its missing pair of ingredients.
static void *smoker(void *arg)
{
    int smoker_id = *(int *)arg;
    sem_t *semaphores[3] = {
        &tobaccoSmokerSem, &paperSmokerSem, &matchSmokerSem
    };
    const char *ingredients[3] = {"tobacco", "paper", "matches"};

    for (;;) {
        sem_wait(semaphores[smoker_id]);

        if (should_shutdown())
            break;

        printf("Smoker %d (%s): making cigarette\n",
               smoker_id + 1, ingredients[smoker_id]);
        usleep(100000);

        // Teammate's logging module increments the count under its mutex.
        record_smoking(smoker_id);
        printf("Smoker %d: finished smoking\n\n", smoker_id + 1);

        // Free the table so the agent can start the next round.
        sem_post(&agentSem);
    }
    return NULL;
}

static void *agent(void *arg)
{
    (void)arg;

    for (int round = 1; round <= ROUNDS; ++round) {
        sem_wait(&agentSem);
        int choice = (round - 1) % 3;

        printf("=== Round %d ===\n", round);
        if (choice == 0) {
            printf("Agent placed PAPER + MATCHES\n");
            sem_post(&tobaccoSmokerSem);
        } else if (choice == 1) {
            printf("Agent placed TOBACCO + MATCHES\n");
            sem_post(&paperSmokerSem);
        } else {
            printf("Agent placed TOBACCO + PAPER\n");
            sem_post(&matchSmokerSem);
        }
    }

    // The final post is made by the smoker completing the last round.
    sem_wait(&agentSem);
    printf("Agent: all rounds completed.\n");

    pthread_mutex_lock(&shutdownMutex);
    shutdownRequested = 1;
    pthread_mutex_unlock(&shutdownMutex);

    // Wake waiting smokers to let them observe the shutdown flag.
    sem_post(&tobaccoSmokerSem);
    sem_post(&paperSmokerSem);
    sem_post(&matchSmokerSem);
    return NULL;
}

int main(void)
{
    pthread_t agentThread;
    pthread_t smokerThreads[3];
    int ids[3] = {0, 1, 2};

    if (sem_init(&agentSem, 0, 1) != 0 ||
        sem_init(&tobaccoSmokerSem, 0, 0) != 0 ||
        sem_init(&paperSmokerSem, 0, 0) != 0 ||
        sem_init(&matchSmokerSem, 0, 0) != 0) {
        perror("sem_init");
        return EXIT_FAILURE;
    }

    for (int i = 0; i < 3; ++i) {
        if (pthread_create(&smokerThreads[i], NULL, smoker, &ids[i]) != 0) {
            fprintf(stderr, "Could not create smoker thread %d\n", i + 1);
            return EXIT_FAILURE;
        }
    }
    if (pthread_create(&agentThread, NULL, agent, NULL) != 0) {
        fprintf(stderr, "Could not create agent thread\n");
        return EXIT_FAILURE;
    }

    pthread_join(agentThread, NULL);
    for (int i = 0; i < 3; ++i)
        pthread_join(smokerThreads[i], NULL);

    // Final output comes from the shared logging module.
    print_results();

    sem_destroy(&agentSem);
    sem_destroy(&tobaccoSmokerSem);
    sem_destroy(&paperSmokerSem);
    sem_destroy(&matchSmokerSem);
    pthread_mutex_destroy(&shutdownMutex);
    return EXIT_SUCCESS;
}
