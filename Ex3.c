#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_READERS 5
#define NUM_WRITERS 2

// Estrutura que representa um RV
typedef struct {
    sem_t sem1;
    sem_t sem2;
} rv_t;

// Operador de espera no RV
void rv_wait(rv_t *rv) {
    sem_wait(&rv->sem1);
    sem_post(&rv->sem2);
}

// Inicialização do RV
void rv_init(rv_t *rv) {
    sem_init(&rv->sem1, 0, 0);
    sem_init(&rv->sem2, 0, 0);
}

sem_t rw_mutex; // Semáforo para acesso exclusivo à área compartilhada
sem_t mutex;    // Semáforo para proteger a contagem de leitores
int read_count = 0; // Contador de leitores

rv_t rv; // Estrutura RV para sincronização

void *reader(void *arg) {
    int reader_id = *((int *)arg);
    while (1) {
        sem_wait(&mutex);
        read_count++;
        if (read_count == 1) {
            sem_wait(&rw_mutex); // Primeiro leitor bloqueia os escritores
        }
        sem_post(&mutex);

        // Seção crítica (leitura)
        printf("Leitor %d está lendo\n", reader_id);
        sleep(1); // Simula tempo de leitura

        sem_wait(&mutex);
        read_count--;
        if (read_count == 0) {
            sem_post(&rw_mutex); // Último leitor libera os escritores
        }
        sem_post(&mutex);

        rv_wait(&rv); // Espera no RV
        sleep(1); // Simula tempo fora da seção crítica
    }
    pthread_exit(NULL);
}

void *writer(void *arg) {
    int writer_id = *((int *)arg);
    while (1) {
        sem_wait(&rw_mutex); // Escritor bloqueia leitores e outros escritores

        // Seção crítica (escrita)
        printf("Escritor %d está escrevendo\n", writer_id);
        sleep(2); // Simula tempo de escrita

        sem_post(&rw_mutex); // Escritor libera a área compartilhada

        rv_wait(&rv); // Espera no RV
        sleep(2); // Simula tempo fora da seção crítica
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t readers[NUM_READERS], writers[NUM_WRITERS];
    int reader_ids[NUM_READERS], writer_ids[NUM_WRITERS];

    sem_init(&rw_mutex, 0, 1);
    sem_init(&mutex, 0, 1);
    rv_init(&rv); // Inicializa o RV

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

    sem_destroy(&rw_mutex);
    sem_destroy(&mutex);
    sem_destroy(&rv.sem1);
    sem_destroy(&rv.sem2);

    return 0;
}