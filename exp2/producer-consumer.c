#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#include <string.h>
#include <stdbool.h>

#define DEFAULT_PRODUCER_COUNT 3
#define DEFAULT_CONSUMER_COUNT 4
#define DEFAULT_BUFFER_SIZE 5
#define MAX_LINE_LEN 1024

int *producerId;
int *consumerId;

FILE *inputStream;
pthread_mutex_t mutex;
sem_t input, output;
char **buffer;
int producerCount, consumerCount, bufferSize;
int inputIndex = 0, outputIndex = 0;
bool finished = false;
void *handleProducer(void *id);
void *handleConsumer(void *id);

int main(int argc, char const *argv[])
{
    producerCount = DEFAULT_PRODUCER_COUNT;
    consumerCount = DEFAULT_CONSUMER_COUNT;
    bufferSize = DEFAULT_BUFFER_SIZE;

    if (argc == 4)
    {
        producerCount = atoi(argv[1]);
        consumerCount = atoi(argv[2]);
        bufferSize = atoi(argv[3]);
    }
    else
    {
        printf("usage: %s <producerCount> <consumerCount> <buffer_size>\n", argv[0]);
        printf("executing with default profile ");
        fflush(stdout);
        for (int i = 0; i < 6; i++)
        {
            printf(".");
            fflush(stdout);
            usleep(500 * 1000);
        }
        printf("\n\n");
    }

    // 初始化
    inputStream = fopen("./input.txt", "r");
    pthread_mutex_init(&mutex, NULL);
    sem_init(&input, 0, bufferSize);
    sem_init(&output, 0, 0);
    pthread_t *producers = (pthread_t *)malloc(sizeof(pthread_t) * producerCount);
    pthread_t *consumers = (pthread_t *)malloc(sizeof(pthread_t) * consumerCount);
    buffer = (char **)malloc(sizeof(char *) * bufferSize);
    producerId = (int *)malloc(sizeof(int) * producerCount);
    consumerId = (int *)malloc(sizeof(int) * consumerCount);

    for (int i = 0; i < bufferSize; i++)
    {
        // 为缓冲区的单元分配空间
        buffer[i] = (char *)malloc(sizeof(char) * MAX_LINE_LEN);
        memset(buffer[i], '\0', MAX_LINE_LEN);
    }

    // 创建线程
    for (int i = 0; i < producerCount; i++)
    {
        // 为生产者线程分配id
        producerId[i] = i + 1;
        pthread_create(&producers[i], NULL, handleProducer, &producerId[i]);
    }
    for (int i = 0; i < consumerCount; i++)
    {
        // 为消费者线程分配id
        consumerId[i] = i + 1;
        pthread_create(&consumers[i], NULL, handleConsumer, &consumerId[i]);
    }

    // 等待所有消费者线程结束
    for (int i = 0; i < consumerCount; i++)
    {
        pthread_join(consumers[i], NULL);
    }
    

    pthread_mutex_destroy(&mutex);
    sem_close(&input);
    sem_close(&output);
    fclose(inputStream);
    return 0;
}

void *handleProducer(void *id)
{
    for (;;)
    {
        sem_wait(&input);
        pthread_mutex_lock(&mutex);
        if(finished)
        {
            pthread_mutex_unlock(&mutex);
            printf("\nproducer %d exited\n", *(int *)id);
            pthread_exit(NULL);
        }
        if (fgets(buffer[inputIndex], MAX_LINE_LEN, inputStream) == NULL)
        {
            finished = true;
            sem_post(&output);
            pthread_mutex_unlock(&mutex);
            printf("\nproducer %d exited\n", *(int *)id);
            pthread_exit(NULL);
        }

        printf("producer %d: successfully fetched string: %s\n", *(int *)id, buffer[inputIndex]);
        inputIndex = (inputIndex + 1) % bufferSize;
        sem_post(&output);
        pthread_mutex_unlock(&mutex);
    }
}

void *handleConsumer(void *id)
{
    for (;;)
    {
        sem_wait(&output);
        pthread_mutex_lock(&mutex);
        if (finished && (inputIndex == outputIndex))
        {
            pthread_mutex_unlock(&mutex);
            // 防止消费者线程一直等待output而无法退出
            sem_post(&output);
            printf("\nconsumer %d exited\n", *(int *)id);
            pthread_exit(NULL);
        }

        printf("consumer %d: %s\n", *(int *)id, buffer[outputIndex]);
        outputIndex = (outputIndex + 1) % bufferSize;
        sem_post(&input);
        pthread_mutex_unlock(&mutex);
    }
}