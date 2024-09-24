//threads e variáveis de condição POSIX

#include <stdio.h>
#include <pthread.h>

#define BUFFER_SIZE 10

int buffer[BUFFER_SIZE];
int in = 0, out = 0;
int count = 0;

pthread_mutex_t mutex;
pthread_cond_t not_full, not_empty;

void* producer(void* arg) {
    int item;
    while (1) {
        item = produzir_item();

        pthread_mutex_lock(&mutex);

        while (count == BUFFER_SIZE)
            pthread_cond_wait(&not_full, &mutex);

        buffer[in] = item;
        in = (in + 1) % BUFFER_SIZE;
        count++;

        pthread_cond_signal(&not_empty);
        pthread_mutex_unlock(&mutex);
    }
}

void* consumer(void* arg) {
    int item;
    while (1) {
        pthread_mutex_lock(&mutex);

        while (count == 0) 
            pthread_cond_wait(&not_empty, &mutex);

        item = buffer[out];
        out = (out + 1) % BUFFER_SIZE;
        count--;

        pthread_cond_signal(&not_full); 
        pthread_mutex_unlock(&mutex);  

        consumir_item(item);
    }
}

int main() {
    pthread_t prod, cons;

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&not_full, NULL);
    pthread_cond_init(&not_empty, NULL);

    pthread_create(&prod, NULL, producer, NULL);
    pthread_create(&cons, NULL, consumer, NULL);

    pthread_join(prod, NULL);
    pthread_join(cons, NULL);

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&not_full);
    pthread_cond_destroy(&not_empty);

    return 0;
}
