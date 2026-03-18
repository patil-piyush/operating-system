#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>

sem_t mutex;
int shared_data = 0;

void producer() {
    int item;

    printf("\n[Producer] Enter value: ");
    scanf("%d", &item);

    printf("Producer trying to enter CS...\n");
    sem_wait(&mutex);

    printf(">>> Producer ENTERED critical section\n");

    shared_data = item;
    printf("Producer produced: %d\n", shared_data);

    sleep(3);  // 🔴 simulate preemption

    printf("<<< Producer EXITING critical section\n");

    sem_post(&mutex);
}

void consumer() {
    printf("Consumer trying to enter CS...\n");

    sem_wait(&mutex);

    printf(">>> Consumer ENTERED critical section\n");

    printf("Consumer consumed: %d\n", shared_data);

    sleep(2);

    printf("<<< Consumer EXITING critical section\n");

    sem_post(&mutex);
}

int main() {
    sem_init(&mutex, 0, 1);

    while (1) {
        producer();
        consumer();

        sleep(1);
    }

    sem_destroy(&mutex);
    return 0;
}