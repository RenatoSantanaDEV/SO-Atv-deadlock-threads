#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  // Para usleep (micro sleep)

#define BUFFER_SIZE 10  // Tamanho do buffer compartilhado

// Definições das variáveis de semáforos e buffer
sem_t empty;
sem_t full;
pthread_mutex_t mutex;

int buffer[BUFFER_SIZE];
int in = 0;
int out = 0;

// Função de produção de itens
int produce_item() {
    static int item = 0;
    return item++;
}

// Função do produtor
void* producer(void* arg) {
    int item;
    while (1) {
        item = produce_item();  // Produz um item
        sem_wait(&empty);       // Decrementa o semáforo de espaços vazios
        pthread_mutex_lock(&mutex);  // Entra na região crítica

        // Adiciona o item produzido ao buffer
        buffer[in] = item;
        in = (in + 1) % BUFFER_SIZE;
        printf("Produced: %d\n", item);

        pthread_mutex_unlock(&mutex);  // Sai da região crítica
        sem_post(&full);  // Incrementa o semáforo de espaços cheios

        usleep(100000);  // Dorme por 100ms para simular trabalho
    }
    return NULL;
}

// Função do consumidor
void* consumer(void* arg) {
    int item;
    while (1) {
        sem_wait(&full);  // Decrementa o semáforo de espaços cheios
        pthread_mutex_lock(&mutex);  // Entra na região crítica

        // Remove um item do buffer
        item = buffer[out];
        out = (out + 1) % BUFFER_SIZE;
        printf("Consumed: %d\n", item);

        pthread_mutex_unlock(&mutex);  // Sai da região crítica
        sem_post(&empty);  // Incrementa o semáforo de espaços vazios

        usleep(150000);
    }
    return NULL;
}

int main() {
    pthread_t prod, cons;

    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);
    pthread_mutex_init(&mutex, NULL);

    pthread_create(&prod, NULL, producer, NULL);
    pthread_create(&cons, NULL, consumer, NULL);

    pthread_join(prod, NULL);
    pthread_join(cons, NULL);

    pthread_mutex_destroy(&mutex);
    sem_destroy(&empty);
    sem_destroy(&full);

    return 0;
}
