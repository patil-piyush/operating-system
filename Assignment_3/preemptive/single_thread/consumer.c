#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <fcntl.h>

#define SEM_NAME "/binary_semaphore"

int main() {
    sem_t *mutex;

    mutex = sem_open(SEM_NAME, O_CREAT, 0666, 1);

    while (1) {
        printf("Consumer: Trying to enter critical section...\n");

        sem_wait(mutex);

        printf("Consumer: ENTERED critical section\n");
        printf("Consumer: Consuming item...\n");

        sleep(2);

        printf("Consumer: EXITING critical section\n\n");

        sem_post(mutex);

        sleep(1);
    }

    sem_close(mutex);
    return 0;
}