#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

using namespace std;

#define N 5

sem_t forks[N];

void* philosopher(void* arg) {
    int id = *(int*)arg;

    for (int i = 0; i < 3; i++) {
        cout << "Philosopher " << id << " is thinking" << endl;
        sleep(1);

        sem_wait(&forks[id]);
        sem_wait(&forks[(id + 1) % N]);

        cout << "Philosopher " << id << " is eating" << endl;
        sleep(1);

        sem_post(&forks[id]);
        sem_post(&forks[(id + 1) % N]);
    }

    return NULL;
}

int main() {
    pthread_t ph[N];
    int id[N];

    for (int i = 0; i < N; i++)
        sem_init(&forks[i], 0, 1);

    for (int i = 0; i < N; i++) {
        id[i] = i;
        pthread_create(&ph[i], NULL, philosopher, &id[i]);
    }

    for (int i = 0; i < N; i++)
        pthread_join(ph[i], NULL);

    return 0;
}
