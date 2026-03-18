#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

sem_t mutex, full;
int shared_data = 0;

void* producer(void* arg) {
    int id = *((int*)arg);

    while (1) {
        int item;

        printf("\n[MAIN INPUT] Enter value to produce: ");
        scanf("%d", &item);

        sem_wait(&mutex);

        printf(">>> Producer %d ENTERED CS\n", id);

        shared_data = item;
        printf("Producer %d produced: %d\n", id, shared_data);

        printf("<<< Producer %d EXITING CS\n", id);

        sem_post(&mutex);
        sem_post(&full);  // signal consumer

        sleep(1);
    }
}

void* consumer(void* arg) {
    int id = *((int*)arg);

    while (1) {
        sem_wait(&full);  // wait for item
        sem_wait(&mutex);

        printf(">>> Consumer %d ENTERED CS\n", id);

        printf("Consumer %d consumed: %d\n", id, shared_data);

        printf("<<< Consumer %d EXITING CS\n", id);

        sem_post(&mutex);

        sleep(2);
    }
}

int main() {
    int p, c;

    printf("Enter number of producers: ");
    scanf("%d", &p);

    printf("Enter number of consumers: ");
    scanf("%d", &c);

    pthread_t prod[p], cons[c];
    int prod_id[p], cons_id[c];

    sem_init(&mutex, 0, 1);
    sem_init(&full, 0, 0);  // initially empty

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

    return 0;
}