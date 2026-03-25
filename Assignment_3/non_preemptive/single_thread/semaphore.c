#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

int *buffer;
int BUFFER_SIZE;

// For case 3
int *inputArray;
int inputSize;

int in = 0, out = 0;

sem_t mutex, empty, full;

// ----------- CLEAR INPUT BUFFER -----------
void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// ---------------- SINGLE PRODUCER ----------------
void* producer(void* arg) {
    int item;

    printf("\n[Producer] Enter ONE item: ");
    scanf("%d", &item);
    clearInputBuffer();

    if (sem_trywait(&empty) != 0) {
        printf("[Producer] Buffer FULL → cannot produce\n");
        return NULL;
    }

    sem_wait(&mutex);

    printf(">>> Producer ENTERED critical section\n");

    buffer[in] = item;
    printf("Produced: %d at index %d\n", item, in);

    in = (in + 1) % BUFFER_SIZE;

    printf("<<< Producer EXITING critical section\n");

    sem_post(&mutex);
    sem_post(&full);

    return NULL;
}

// ---------------- SINGLE CONSUMER ----------------
void* consumer(void* arg) {
    if (sem_trywait(&full) != 0) {
        printf("[Consumer] Buffer EMPTY → cannot consume\n");
        return NULL;
    }

    sem_wait(&mutex);

    printf(">>> Consumer ENTERED critical section\n");

    int item = buffer[out];
    printf("Consumed: %d from index %d\n", item, out);

    buffer[out] = -1;

    out = (out + 1) % BUFFER_SIZE;

    printf("<<< Consumer EXITING critical section\n");

    sem_post(&mutex);
    sem_post(&empty);

    return NULL;
}

// ---------------- PRODUCER (ARRAY) ----------------
void* producer_array(void* arg) {
    for (int i = 0; i < inputSize; i++) {

        if (sem_trywait(&empty) != 0) {
            printf("\n[Producer] Buffer FULL → waiting...\n");
            sem_wait(&empty);
        }

        sem_wait(&mutex);

        printf(">>> Producer ENTERED critical section\n");

        buffer[in] = inputArray[i];
        printf("Produced: %d at index %d\n", inputArray[i], in);

        in = (in + 1) % BUFFER_SIZE;

        printf("<<< Producer EXITING critical section\n");

        sem_post(&mutex);
        sem_post(&full);

        sleep(1);  // slow for visibility
    }

    return NULL;
}

// ---------------- CONSUMER (LOOP) ----------------
void* consumer_loop(void* arg) {
    for (int i = 0; i < inputSize; i++) {

        if (sem_trywait(&full) != 0) {
            printf("\n[Consumer] Buffer EMPTY → waiting...\n");
            sem_wait(&full);
        }

        sem_wait(&mutex);

        printf(">>> Consumer ENTERED critical section\n");

        int item = buffer[out];
        printf("Consumed: %d from index %d\n", item, out);

        buffer[out] = -1;

        out = (out + 1) % BUFFER_SIZE;

        printf("<<< Consumer EXITING critical section\n");

        sem_post(&mutex);
        sem_post(&empty);

        sleep(2);  // slower → shows FULL condition
    }

    return NULL;
}

// ---------------- DISPLAY BUFFER ----------------
void displayBuffer() {
    printf("\nBuffer State: ");
    for (int i = 0; i < BUFFER_SIZE; i++) {
        if (buffer[i] == -1)
            printf("[ ] ");
        else
            printf("[%d] ", buffer[i]);
    }
    printf("\n");
}

// ---------------- MAIN ----------------
int main() {
    pthread_t p, c;
    int choice;

    printf("Enter buffer size: ");
    scanf("%d", &BUFFER_SIZE);
    clearInputBuffer();

    buffer = (int*)malloc(sizeof(int) * BUFFER_SIZE);

    for (int i = 0; i < BUFFER_SIZE; i++)
        buffer[i] = -1;

    sem_init(&mutex, 0, 1);
    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);

    while (1) {
        printf("\n===== MENU =====\n");
        printf("1. Produce\n");
        printf("2. Consume\n");
        printf("3. Both (Simultaneously)\n");
        printf("4. Exit\n");
        printf("Enter choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input! Enter number only.\n");
            clearInputBuffer();
            continue;
        }
        clearInputBuffer();

        switch (choice) {
            case 1:
                pthread_create(&p, NULL, producer, NULL);
                pthread_join(p, NULL);
                break;

            case 2:
                pthread_create(&c, NULL, consumer, NULL);
                pthread_join(c, NULL);
                break;

            case 3:
                printf("Enter number of items: ");
                scanf("%d", &inputSize);

                inputArray = (int*)malloc(sizeof(int) * inputSize);

                printf("Enter %d items: ", inputSize);
                for (int i = 0; i < inputSize; i++) {
                    scanf("%d", &inputArray[i]);
                }
                clearInputBuffer();

                pthread_create(&p, NULL, producer_array, NULL);
                pthread_create(&c, NULL, consumer_loop, NULL);

                pthread_join(p, NULL);
                pthread_join(c, NULL);

                free(inputArray);
                break;

            case 4:
                printf("Exiting...\n");
                exit(0);

            default:
                printf("Invalid choice!\n");
        }

        displayBuffer();
    }

    return 0;
}