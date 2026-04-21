#include <iostream>
#include <pthread.h>
#include <semaphore.h>
using namespace std;
sem_t wrt;
pthread_mutex_t mutex;
int readcount = 0;
// -------- READER --------
void* reader(void* arg)
{
int id = *((int*)arg);
pthread_mutex_lock(&mutex);
readcount++;
if(readcount == 1)
sem_wait(&wrt);
pthread_mutex_unlock(&mutex);
cout << "Reader " << id << " is reading\n";
pthread_mutex_lock(&mutex);
readcount--;
if(readcount == 0)
sem_post(&wrt);
pthread_mutex_unlock(&mutex);
return NULL;
}
// -------- WRITER --------

void* writer(void* arg)
{
int id = *((int*)arg);
sem_wait(&wrt);
cout << "Writer " << id << " is writing\n";
sem_post(&wrt);
return NULL;
}
int main()
{
int r, w;
cout << "Enter number of readers: ";
cin >> r;
cout << "Enter number of writers: ";
cin >> w;
pthread_t readers[r], writers[w];
int rid[r], wid[w];
sem_init(&wrt, 0, 1);
pthread_mutex_init(&mutex, NULL);
// Create readers
for(int i = 0; i < r; i++)
{
rid[i] = i + 1;
pthread_create(&readers[i], NULL, reader, &rid[i]);
}
// Create writers
for(int i = 0; i < w; i++)
{
wid[i] = i + 1;

pthread_create(&writers[i], NULL, writer, &wid[i]);
}
// Join readers
for(int i = 0; i < r; i++)
pthread_join(readers[i], NULL);
// Join writers
for(int i = 0; i < w; i++)
pthread_join(writers[i], NULL);
return 0;
}
