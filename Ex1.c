#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h> 

#define BUFFER_SIZE 10
#define TRUE 1
#define FALSE 0

int buffer[BUFFER_SIZE];
int in = 0, out = 0;
int running = TRUE;

sem_t empty, full, mutex;

int produzir_item() {
    return rand() % 100;
}

void* producer(void* arg) {
    int item;
    while (running) {
        item = produzir_item();
        sem_wait(&empty);  
        sem_wait(&mutex);

        // Inserção no buffer
        buffer[in] = item;
        in = (in + 1) % BUFFER_SIZE;

        sem_post(&mutex);
        sem_post(&full); 

        printf("Produziu: %d\n", item);
        sleep(1);
    }
    return NULL;
}

void* consumer(void* arg) {
    int item;
    while (running) {
        sem_wait(&full);       
        sem_wait(&mutex);     

        item = buffer[out];
        out = (out + 1) % BUFFER_SIZE;

        sem_post(&mutex);      
        sem_post(&empty);      

        printf("Consumiu: %d\n", item);
        sleep(2); 
    }
    return NULL;
}

int main() {
    pthread_t prod, cons;

    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);
    sem_init(&mutex, 0, 1);           

    pthread_create(&prod, NULL, producer, NULL);
    pthread_create(&cons, NULL, consumer, NULL);

    sleep(10);
    running = FALSE;

    pthread_join(prod, NULL);
    pthread_join(cons, NULL);

    sem_destroy(&empty);
    sem_destroy(&full);
    sem_destroy(&mutex);

    return 0;
}
