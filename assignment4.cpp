#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

using namespace std;

#define SIZE 5

int buffer[SIZE];
int in = 0, out = 0;

pthread_mutex_t mutex1;
sem_t empty_sem, full;

void* producer(void* arg) {
    for (int i = 1; i <= 10; i++) {
        sem_wait(&empty_sem);
        pthread_mutex_lock(&mutex1);

        buffer[in] = i;
        cout << "Produced: " << i << endl;
        in = (in + 1) % SIZE;

        pthread_mutex_unlock(&mutex1);
        sem_post(&full);

        sleep(1);
    }
    return NULL;
}

void* consumer(void* arg) {
    for (int i = 1; i <= 10; i++) {
        sem_wait(&full);
        pthread_mutex_lock(&mutex1);

        int item = buffer[out];
        cout << "Consumed: " << item << endl;
        out = (out + 1) % SIZE;

        pthread_mutex_unlock(&mutex1);
        sem_post(&empty_sem);

        sleep(1);
    }
    return NULL;
}

int main() {
    pthread_t p, c;

    pthread_mutex_init(&mutex1, NULL);
    sem_init(&empty_sem, 0, SIZE);
    sem_init(&full, 0, 0);

    pthread_create(&p, NULL, producer, NULL);
    pthread_create(&c, NULL, consumer, NULL);

    pthread_join(p, NULL);
    pthread_join(c, NULL);

    return 0;
}

