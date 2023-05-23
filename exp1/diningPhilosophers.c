#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

#define NUM_OF_PHILOSOPHERS 5
#define MAX_CYCLE 3000
#define MAX_BREAKTIME 1
#define msleep(milliseconds) usleep(milliseconds * 1000)

pthread_mutex_t chopsticks[NUM_OF_PHILOSOPHERS];
int philosopherId[NUM_OF_PHILOSOPHERS];

void shuffle(int *array, int size);
void createPhilosophers(pthread_t *philosophers);
void *startLifeCycle(void *idArg);
void eat(int id);
void think(int id);
void haveABreak();

int main(void)
{
    srand((unsigned int)time(NULL));

    // initialize philosophers id array and chopsticks array
    for (int i = 0; i < NUM_OF_PHILOSOPHERS; i++)
    {
        pthread_mutex_init(&chopsticks[i], NULL);
        philosopherId[i] = i;
    }

    // randomize the thread creation process
    shuffle(philosopherId, NUM_OF_PHILOSOPHERS);

    pthread_t philosophers[NUM_OF_PHILOSOPHERS];

    createPhilosophers(philosophers);

    for (int i = 0; i < NUM_OF_PHILOSOPHERS; i++)
    {
        pthread_join(philosophers[i], NULL);
    }

    for (int i = 0; i < NUM_OF_PHILOSOPHERS; i++)
    {
        pthread_mutex_destroy(&chopsticks[i]);
    }
    return 0;
}

void createPhilosophers(pthread_t *philosophers)
{

    for (int i = 0; i < NUM_OF_PHILOSOPHERS; i++)
    {
        pthread_create(&philosophers[i], NULL, startLifeCycle, &philosopherId[i]);
    }
}

void *startLifeCycle(void *idArg)
{
    int id = *(int *)idArg;
    for (int i = 0; i < MAX_CYCLE; i++)
    {
        think(id);
        eat(id);
    }
    printf("\nPhilosopher #%d has thinked %d times and eaten %d times\n", id, MAX_CYCLE, MAX_CYCLE);
}

void eat(int id)
{
    if (id & 0x1) // if the sequence number of the philosopher attempting to eat is odd, pick up the chopstick on the left first
    {
        pthread_mutex_lock(&chopsticks[id]);
        printf("\nPhilosopher #%d has picked up the chopstick on his left\n", id);
        pthread_mutex_lock(&chopsticks[(id + 1) % NUM_OF_PHILOSOPHERS]);
        printf("\nPhilosopher #%d has picked up the chopstick on his right\n", id);
    }
    else // even num: pick up the chopstick on the right first
    {
        pthread_mutex_lock(&chopsticks[(id + 1) % NUM_OF_PHILOSOPHERS]);
        printf("\nPhilosopher #%d has picked up the chopstick on his right\n", id);
        pthread_mutex_lock(&chopsticks[id]);
        printf("\nPhilosopher #%d has picked up the chopstick on his left\n", id);
    }
    printf("\nPhilosopher #%d is now eating\n", id);
    haveABreak();

    pthread_mutex_unlock(&chopsticks[id]);
    printf("\nPhilosopher #%d has put down the chopstick on his left\n", id);
    pthread_mutex_unlock(&chopsticks[(id + 1) % NUM_OF_PHILOSOPHERS]);
    printf("\nPhilosopher #%d has put down the chopstick on his right\n", id);
}

void think(int id)
{

    printf("\nPhilosopher #%d is now thinking\n", id);
    haveABreak();
}

void haveABreak()
{
    msleep(rand() % (MAX_BREAKTIME + 1));
}

void shuffle(int *array, int size)
{
    for (int i = size - 1; i > 0; i--)
    {
        int j = rand() % (i + 1);
        int temp = array[i];
        array[i] = array[j];
        array[j] = temp;
    }
}