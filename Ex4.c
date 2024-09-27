#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_READERS 5
#define NUM_WRITERS 3

pthread_rwlock_t lock;
int data = 0;  // Recurso compartilhado

void* reader(void* arg) {
    int id = *((int*) arg);
    while (1) {
        pthread_rwlock_rdlock(&lock);
        printf("Leitor %d lendo o valor %d\n", id, data);
        pthread_rwlock_unlock(&lock);
        sleep(rand() % 5 + 1);  // Simula o tempo de leitura
    }
    return NULL;
}

void* writer(void* arg) {
    int id = *((int*) arg);
    while (1) {
        pthread_rwlock_wrlock(&lock);
        data++;  // Modifica o recurso compartilhado
        printf("Escritor %d escreveu o valor %d\n", id, data);
        pthread_rwlock_unlock(&lock);
        sleep(rand() % 5 + 2);  // Simula o tempo de escrita
    }
    return NULL;
}

int main() {
    pthread_t r[NUM_READERS], w[NUM_WRITERS];
    int i, ids[NUM_READERS + NUM_WRITERS];

    pthread_rwlock_init(&lock, NULL);

    for (i = 0; i < NUM_READERS; i++) {
        ids[i] = i + 1;
        pthread_create(&r[i], NULL, reader, &ids[i]);
    }

    for (i = 0; i < NUM_WRITERS; i++) {
        ids[i + NUM_READERS] = i + 1;
        pthread_create(&w[i], NULL, writer, &ids[i + NUM_READERS]);
    }

    for (i = 0; i < NUM_READERS; i++) {
        pthread_join(r[i], NULL);
    }

    for (i = 0; i < NUM_WRITERS; i++) {
        pthread_join(w[i], NULL);
    }

    pthread_rwlock_destroy(&lock);

    return 0;
}
