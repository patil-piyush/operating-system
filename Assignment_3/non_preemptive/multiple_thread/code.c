#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

sem_t mutex;
int shared_data = 0;

void* producer(void* arg) {
    int id = *((int*)arg);

    while (1) {
        int item;
        printf("Producer Thread %d: Enter value to produce: ", id);
        scanf("%d", &item);

        sem_wait(&mutex);

        printf(">>> Producer %d ENTERED critical section\n", id);

        shared_data = item;
        printf("Producer %d produced: %d\n", id, shared_data);

        printf("<<< Producer %d EXITING critical section\n\n", id);

        sem_post(&mutex);

        sleep(1);
    }
}

void* consumer(void* arg) {
    int id = *((int*)arg);

    while (1) {
        sem_wait(&mutex);

        printf(">>> Consumer %d ENTERED critical section\n", id);

        printf("Consumer %d consumed: %d\n", id, shared_data);

        printf("<<< Consumer %d EXITING critical section\n\n", id);

        sem_post(&mutex);

        sleep(2);
    }
}

int main() {
    int p, c;

    printf("Enter number of producer threads: ");
    scanf("%d", &p);

    printf("Enter number of consumer threads: ");
    scanf("%d", &c);

    pthread_t prod[p], cons[c];
    int prod_id[p], cons_id[c];

    sem_init(&mutex, 0, 1);

    for (int i = 0; i < p; i++) {
        prod_id[i] = i + 1;
        pthread_create(&prod[i], NULL, producer, &prod_id[i]);
    }

    for (int i = 0; i < c; i++) {
        cons_id[i] = i + 1;
        pthread_create(&cons[i], NULL, consumer, &cons_id[i]);
    }

    for (int i = 0; i < p; i++)
        pthread_join(prod[i], NULL);

    for (int i = 0; i < c; i++)
        pthread_join(cons[i], NULL);

    sem_destroy(&mutex);

    return 0;
}