#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <fcntl.h>

#define SEM_NAME "/binary_semaphore"

int main() {
    sem_t *mutex;

    mutex = sem_open(SEM_NAME, O_CREAT, 0666, 1);

    int item = 1;

    while (1) {
        sem_wait(mutex);  // ENTER critical section

        printf("Producer: Producing item %d\n", item);
        sleep(2);  // simulate work

        printf("Producer: Leaving critical section\n\n");

        sem_post(mutex);  // EXIT critical section

        item++;
        sleep(1);
    }

    sem_close(mutex);
    sem_unlink(SEM_NAME);
    return 0;
}