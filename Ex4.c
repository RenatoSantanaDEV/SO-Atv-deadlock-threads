#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_READERS 5
#define NUM_WRITERS 2

pthread_rwlock_t rwlock; // RWLock para acesso à área compartilhada

void *reader(void *arg) {
    int reader_id = *((int *)arg);
    while (1) {
        pthread_rwlock_rdlock(&rwlock); // Obtém o lock de leitura

        // Seção crítica (leitura)
        printf("Leitor %d está lendo\n", reader_id);
        sleep(1); // Simula tempo de leitura

        pthread_rwlock_unlock(&rwlock); // Libera o lock de leitura

        sleep(1); // Simula tempo fora da seção crítica
    }
    pthread_exit(NULL);
}

void *writer(void *arg) {
    int writer_id = *((int *)arg);
    while (1) {
        pthread_rwlock_wrlock(&rwlock); // Obtém o lock de escrita

        // Seção crítica (escrita)
        printf("Escritor %d está escrevendo\n", writer_id);
        sleep(2); // Simula tempo de escrita

        pthread_rwlock_unlock(&rwlock); // Libera o lock de escrita

        sleep(2); // Simula tempo fora da seção crítica
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t readers[NUM_READERS], writers[NUM_WRITERS];
    int reader_ids[NUM_READERS], writer_ids[NUM_WRITERS];

    pthread_rwlock_init(&rwlock, NULL); // Inicializa o RWLock

    for (int i = 0; i < NUM_READERS; i++) {
        reader_ids[i] = i + 1;
        pthread_create(&readers[i], NULL, reader, &reader_ids[i]);
    }

    for (int i = 0; i < NUM_WRITERS; i++) {
        writer_ids[i] = i + 1;
        pthread_create(&writers[i], NULL, writer, &writer_ids[i]);
    }

    for (int i = 0; i < NUM_READERS; i++) {
        pthread_join(readers[i], NULL);
    }

    for (int i = 0; i < NUM_WRITERS; i++) {
        pthread_join(writers[i], NULL);
    }

    pthread_rwlock_destroy(&rwlock); // Destroi o RWLock

    return 0;
}