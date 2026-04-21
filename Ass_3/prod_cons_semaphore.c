#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h> // for sleep

#define SIZE 5

int buffer[SIZE];
int in = 0, out = 0;

sem_t empty;
sem_t full;
pthread_mutex_t mutex;

int item_value = 0;

// Producer thread function
void* producer(void* arg)
{
    while(1)
    {
        sem_wait(&empty);          // Wait if buffer is full
        pthread_mutex_lock(&mutex);

        buffer[in] = item_value;
        printf("Producer produced %d\n", item_value);
        item_value++;
        in = (in + 1) % SIZE;

        pthread_mutex_unlock(&mutex);
        sem_post(&full);           // Signal that buffer has a new item

        usleep(100000); // 100ms delay to simulate work
    }
    return NULL;
}

// Consumer thread function
void* consumer(void* arg)
{
    while(1)
    {
        sem_wait(&full);           // Wait if buffer is empty
        pthread_mutex_lock(&mutex);

        int item = buffer[out];
        printf("Consumer got %d\n", item);
        out = (out + 1) % SIZE;

        pthread_mutex_unlock(&mutex);
        sem_post(&empty);          // Signal that buffer has space

        usleep(1000000); // 1000ms delay to simulate slower consumer
    }
    return NULL;
}

int main()
{
    pthread_t p, c;

    // Initialize semaphores and mutex
    sem_init(&empty, 0, SIZE); // buffer empty slots
    sem_init(&full, 0, 0);     // buffer filled slots
    pthread_mutex_init(&mutex, NULL);

    // Create threads
    pthread_create(&p, NULL, producer, NULL);
    pthread_create(&c, NULL, consumer, NULL);

    // Wait for threads to finish (they won’t, infinite loop)
    pthread_join(p, NULL);
    pthread_join(c, NULL);

    // Cleanup (never reached in this infinite example)
    sem_destroy(&empty);
    sem_destroy(&full);
    pthread_mutex_destroy(&mutex);

    return 0;
}
