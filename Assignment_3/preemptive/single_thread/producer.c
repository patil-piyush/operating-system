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
        printf("Producer: Trying to enter critical section...\n");
        sem_wait(mutex);

        printf("Producer: ENTERED critical section\n");
        printf("Producer: Producing item %d\n", item);

        sleep(5);  // 🔴 Simulate preemption (long hold)

        printf("Producer: EXITING critical section\n\n");

        sem_post(mutex);

        item++;
        sleep(1);
    }

    sem_close(mutex);
    return 0;
}