#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

using namespace std;

sem_t wrt;
pthread_mutex_t mutex1;
int readcount = 0;

void* reader(void* arg) {
    int id = *(int*)arg;

    pthread_mutex_lock(&mutex1);
    readcount++;
    if (readcount == 1)
        sem_wait(&wrt);
    pthread_mutex_unlock(&mutex1);

    cout << "Reader " << id << " is reading" << endl;
    sleep(1);

    pthread_mutex_lock(&mutex1);
    readcount--;
    if (readcount == 0)
        sem_post(&wrt);
    pthread_mutex_unlock(&mutex1);

    return NULL;
}

void* writer(void* arg) {
    int id = *(int*)arg;

    sem_wait(&wrt);
    cout << "Writer " << id << " is writing" << endl;
    sleep(1);
    sem_post(&wrt);

    return NULL;
}

int main() {
    pthread_t r[5], w[2];
    int rid[5], wid[2];

    sem_init(&wrt, 0, 1);
    pthread_mutex_init(&mutex1, NULL);

    for (int i = 0; i < 5; i++) {
        rid[i] = i + 1;
        pthread_create(&r[i], NULL, reader, &rid[i]);
    }

    for (int i = 0; i < 2; i++) {
        wid[i] = i + 1;
        pthread_create(&w[i], NULL, writer, &wid[i]);
    }

    for (int i = 0; i < 5; i++)
        pthread_join(r[i], NULL);

    for (int i = 0; i < 2; i++)
        pthread_join(w[i], NULL);

    return 0;
}
