#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define SIZE 5

int buffer[SIZE];
int in = 0, out = 0;

sem_t empty, full;
pthread_mutex_t mutex;

int item = 0;

void* producer(void* arg)
{
    int id = *(int*)arg;

    for(int i = 0; i < 10; i++)
    {
        sem_wait(&empty);
        pthread_mutex_lock(&mutex);

        buffer[in] = item;
        printf("Producer %d produced %d\n", id, item);

        item++;
        in = (in + 1) % SIZE;

        pthread_mutex_unlock(&mutex);
        sem_post(&full);

        sleep(1);
    }
    return NULL;
}

void* consumer(void* arg)
{
    int id = *(int*)arg;

    for(int i = 0; i < 10; i++)
    {
        sem_wait(&full);
        pthread_mutex_lock(&mutex);

        int val = buffer[out];
        printf("Consumer %d consumed %d\n", id, val);

        out = (out + 1) % SIZE;

        pthread_mutex_unlock(&mutex);
        sem_post(&empty);

        sleep(2);
    }
    return NULL;
}

int main()
{
    int np, nc;

    printf("Enter number of Producers: ");
    scanf("%d", &np);

    printf("Enter number of Consumers: ");
    scanf("%d", &nc);

    pthread_t p[np], c[nc];
    int pid[np], cid[nc];

    sem_init(&empty, 0, SIZE);
    sem_init(&full, 0, 0);
    pthread_mutex_init(&mutex, NULL);

    for(int i = 0; i < np; i++)
    {
        pid[i] = i + 1;
        pthread_create(&p[i], NULL, producer, &pid[i]);
    }

    for(int i = 0; i < nc; i++)
    {
        cid[i] = i + 1;
        pthread_create(&c[i], NULL, consumer, &cid[i]);
    }

    for(int i = 0; i < np; i++)
        pthread_join(p[i], NULL);

    for(int i = 0; i < nc; i++)
        pthread_join(c[i], NULL);

    return 0;
}
